#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "Luma/Signal/Signal_fwd.h"

namespace Luma::Signal
{

class Connection
{
public:
  Connection () noexcept = default;
  Connection (const Connection &) noexcept = default;
  Connection (Connection &&) noexcept = default;
  ~Connection () noexcept { mDisconnectFunc (this); }

  void setEnabled (bool enabled) { *mEnabled = enabled; }
  bool isEnabled () const { return *mEnabled; }

  Connection &operator= (const Connection &) noexcept;
  Connection &operator= (Connection &&) noexcept;

private:
  template<typename... Args>
  friend class Luma::Signal::Signal;
  friend class Luma::Signal::Slots;

  void setDisconnectFunc (std::function<void (const Connection *)> &&disconnectFunc)
  {
    mDisconnectFunc = std::move (disconnectFunc);
  }

  void setEnabledFlag (bool &enabledFlag)
  {
    mEnabled = &enabledFlag;
  }

  std::function<void (const Connection *)> mDisconnectFunc;
  bool *mEnabled = nullptr;
};

class Slots
{
public:
  inline Slots () noexcept = default;
  inline ~Slots () noexcept = default;

  template<typename... Args>
  inline std::weak_ptr<Connection> connect (Signal<Args...> &signal, Signal<Args...>::Handler handler) noexcept;
  inline void disconnectAll () noexcept;
  inline void setConnetionsEnabled (bool enabled);

private:
  Slots (const Slots &) = delete;
  Slots (Slots &&) = delete;

  Slots &operator= (const Slots &) = delete;
  Slots &operator= (Slots &&) = delete;

private:
  std::vector<std::shared_ptr<Connection>> mConnections;
};

template<typename... Args>
class Signal
{
public:
  using Handler = std::function<void (Args...)>;

  inline Signal () noexcept = default;
  inline ~Signal () noexcept = default;

  inline void notify (Args... args);


private:
  friend class Slots;
  class ConnectionHandler
  {
  public:
    Connection *connectionPtr;
    Handler handler;
    bool enabled = true;

    ConnectionHandler (Connection *connectionPtr, Handler &&handler) noexcept: connectionPtr (connectionPtr), handler (std::move (handler))
    {
      connectionPtr->setEnabledFlag (enabled);
    }
    ConnectionHandler (const ConnectionHandler &) = delete;
    ConnectionHandler (ConnectionHandler &&other) noexcept: connectionPtr (other.connectionPtr), handler (std::move (other.handler)), enabled (other.enabled)
    {
      connectionPtr->setEnabledFlag (enabled);
      other.connectionPtr = nullptr;
    }

    ConnectionHandler &operator= (const ConnectionHandler &) = delete;
    ConnectionHandler &operator= (ConnectionHandler &&rhs)
    {
      connectionPtr->setEnabledFlag (enabled);
      connectionPtr = rhs.connectionPtr;
      handler = std::move (rhs.handler);
      enabled = rhs.enabled;

      rhs.connectionPtr = nullptr;
      return *this;
    }

    ~ConnectionHandler ()
    {
      if (connectionPtr)
        connectionPtr->setDisconnectFunc ([] (const Connection *) {});
    }
  };

  std::shared_ptr<Connection> connect (Handler &&handler);

  Signal (const Signal &) = delete;
  Signal (Signal &&) = delete;

  Signal &operator= (const Signal &) = delete;
  Signal &operator= (Signal &&) = delete;

  std::vector<Luma::Signal::Signal<Args...>::ConnectionHandler> mConnectionHandlers;
};

//----------------------------------------------

template<typename... Args>
inline std::weak_ptr<Connection> Slots::connect (Signal<Args...> &signal, Signal<Args...>::Handler handler) noexcept
{
  mConnections.emplace_back (signal.connect (std::move (handler)));
  return mConnections.back ();
}

inline void Slots::disconnectAll () noexcept
{
  mConnections.clear ();
}

inline void Slots::setConnetionsEnabled (bool enabled)
{
  for (std::shared_ptr<Connection> &connectionSptr : mConnections)
    connectionSptr->setEnabled (enabled);
}

//----------------------------------------------

template<typename... Args>
std::shared_ptr<Connection> Signal<Args...>::connect (Handler &&handler)
{
  std::shared_ptr<Connection> connectionSptr = std::make_shared<Connection> ();
  mConnectionHandlers.emplace_back (connectionSptr.get (), std::move (handler));
  connectionSptr->setDisconnectFunc ([this] (const Connection *connection) {
    std::erase_if (mConnectionHandlers, [&] (const Luma::Signal::Signal<Args...>::ConnectionHandler &connectonHandler) {
      return connectonHandler.connectionPtr == connection;
    });

    (void) connection;
    return;
  });
  return connectionSptr;
}

template<typename... Args>
inline void Signal<Args...>::notify (Args... args)
{
  for (Luma::Signal::Signal<Args...>::ConnectionHandler &connectionHandler : mConnectionHandlers)
    {
      if (connectionHandler.enabled)
        connectionHandler.handler (args...);
    }
}
}  // namespace utils
