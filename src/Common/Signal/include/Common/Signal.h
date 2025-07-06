#pragma once

#include <functional>
#include <vector>

namespace Common
{
template<typename... Args>
class Signal
{
public:
  using Handler = std::function<void (Args...)>;

  class TemporaryDisconnector;
  class Connection
  {
  public:
    Connection () noexcept = default;
    Connection (Connection &&other) noexcept;
    ~Connection () noexcept;

    Connection &operator= (Connection &&) noexcept;

    [[nodiscard]] TemporaryDisconnector temporaryDisconnect () noexcept;

  private:
    Connection (const Connection &) = delete;
    Connection &operator= (const Connection &) = delete;

    friend class Signal;
    Connection (Handler &&handler, Signal &s) noexcept;
    Connection (const Handler &handler, Signal &s) noexcept;

  private:
    Handler mHandler = [] (Args...) {};
    Signal *mSignal = nullptr;
  };

  class TemporaryDisconnector
  {
  public:
    TemporaryDisconnector (const TemporaryDisconnector &) = delete;
    TemporaryDisconnector (TemporaryDisconnector &&) = delete;
    ~TemporaryDisconnector () { mTargetHandler = std::move (mHandler); }

    TemporaryDisconnector &operator= (const TemporaryDisconnector &) = delete;
    TemporaryDisconnector &operator= (TemporaryDisconnector &&) = delete;

  private:
    friend class Connection;
    TemporaryDisconnector (Handler &handler) : mTargetHandler (handler), mHandler (std::move (handler))
    {
      mTargetHandler = [] (Args...) {};
    }

  private:
    Handler &mTargetHandler;
    Handler mHandler;
  };

  Signal ();
  ~Signal ();

  Signal (const Signal &) = delete;
  Signal (Signal &&) = delete;

  Signal &operator= (const Signal &) = delete;
  Signal &operator= (Signal &&) = delete;

  [[nodiscard]] Connection connect (Handler &&);
  [[nodiscard]] Connection connect (const Handler &);
  void notify (Args... args);

private:
  std::vector<Connection *> mConnections;
};

//----------------------------------------------

template<typename... Args>
Signal<Args...>::Connection::Connection (Signal<Args...>::Connection &&other) noexcept
{
  mHandler = std::move (other.mHandler);
  mSignal = other.mSignal;
  other.mSignal = nullptr;

  if (mSignal)
    {
      for (Connection *&connection : mSignal->mConnections)
        {
          if (connection == &other)
            {
              connection = this;
              return;
            }
        }
    }
}

template<typename... Args>
Signal<Args...>::Connection &Signal<Args...>::Connection::operator= (Connection &&rhs) noexcept
{
  if (this == &rhs)
    return *this;

  if (mSignal)
    std::erase (mSignal->mConnections, this);

  mHandler = std::move (rhs.mHandler);
  mSignal = rhs.mSignal;
  rhs.mSignal = nullptr;

  if (mSignal)
    {
      for (Connection *&connection : mSignal->mConnections)
        {
          if (connection == &rhs)
            {
              connection = this;
              break;
            }
        }
    }

  return *this;
}

template<typename... Args>
Signal<Args...>::Connection::~Connection () noexcept
{
  if (mSignal)
    std::erase (mSignal->mConnections, this);
}

template<typename... Args>
Signal<Args...>::TemporaryDisconnector Signal<Args...>::Connection::temporaryDisconnect () noexcept
{
  return TemporaryDisconnector (mHandler);
}

template<typename... Args>
Signal<Args...>::Connection::Connection (Signal<Args...>::Handler &&handler, Signal<Args...> &s) noexcept : mHandler (std::move (handler)), mSignal (&s)
{
}

template<typename... Args>
Signal<Args...>::Connection::Connection (const Signal<Args...>::Handler &handler, Signal &s) noexcept : mHandler (handler), mSignal (&s)
{
}

//----------------------------------------------

template<typename... Args>
Signal<Args...>::Signal ()
{
}

template<typename... Args>
Signal<Args...>::~Signal ()
{
  for (Connection *con : mConnections)
    {
      con->mSignal = nullptr;

      /// \note Clear Handler
      con->mHandler = {};
    }
}

template<typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::connect (const Handler &handler)
{
  Connection con (handler, *this);
  mConnections.emplace_back (&con);
  return con;
}

template<typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::connect (Handler &&handler)
{
  Connection con (std::move (handler), *this);
  mConnections.emplace_back (&con);
  return con;
}

template<typename... Args>
void Signal<Args...>::notify (Args... args)
{
  for (Connection *con : mConnections)
    con->mHandler (args...);
}
}  // namespace utils
