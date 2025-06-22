#pragma once

#include <functional>
#include <vector>

namespace utils
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
    Connection (const Connection &) = delete;
    Connection (Connection &&) = default;
    ~Connection ()
    {
      if (m_signal)
        std::erase (m_signal->m_connections, this);
    }

    Connection &operator= (const Connection &) = delete;
    Connection &operator= (Connection &&) = delete;

    [[nodiscard]] TemporaryDisconnector temporaryDisconnect () { return TemporaryDisconnector (m_handler); }

  private:
    friend class Signal;
    Connection (Handler &&handler, Signal &s) : m_handler (std::move (handler)), m_signal (&s) {}
    Connection (const Handler &handler, Signal &s) : m_handler (handler), m_signal (&s) {}

  private:
    Handler m_handler = [] (Args...) {};
    Signal *m_signal = nullptr;
  };

  class TemporaryDisconnector
  {
  public:
    TemporaryDisconnector (const TemporaryDisconnector &) = delete;
    TemporaryDisconnector (TemporaryDisconnector &&) = delete;
    ~TemporaryDisconnector () { m_targetHandler = std::move (m_handler); }

    TemporaryDisconnector &operator= (const TemporaryDisconnector &) = delete;
    TemporaryDisconnector &operator= (TemporaryDisconnector &&) = delete;

  private:
    friend class Connection;
    TemporaryDisconnector (Handler &handler) : m_targetHandler (handler), m_handler (std::move (handler))
    {
      m_targetHandler = [] (Args...) {};
    }

  private:
    Handler &m_targetHandler;
    Handler m_handler;
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
  std::vector<Connection *> m_connections;
};

//----------------------------------------------

template<typename... Args>
Signal<Args...>::Signal ()
{
}

template<typename... Args>
Signal<Args...>::~Signal ()
{
  for (Connection *con : m_connections)
    {
      con->m_signal = nullptr;

      /// \note Clear Handler
      con->m_handler = {};
    }
}

template<typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::connect (const Handler &handler)
{
  Connection con (handler, *this);
  m_connections.emplace_back (&con);
  return con;
}

template<typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::connect (Handler &&handler)
{
  Connection con (std::move (handler), *this);
  m_connections.emplace_back (&con);
  return con;
}

template<typename... Args>
void Signal<Args...>::notify (Args... args)
{
  for (Connection *con : m_connections)
    con->m_handler (args...);
}
}  // namespace utils
