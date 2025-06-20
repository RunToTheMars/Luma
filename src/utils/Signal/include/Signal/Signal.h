#pragma once

#include <functional>
#include <vector>

namespace utils
{

template<typename... Args>
class signal
{
public:
  class connection
  {
  public:
    connection (const connection &) = delete;
    connection (connection &&);
    ~connection () { disconnect (); }

    connection &operator= (const connection &) = delete;
    connection &operator= (connection &&);

    bool connected () const { return m_connected; }
    bool disconnected () const { return !connected (); }

    void disconnect ()
    {
      if (disconnected ())
        return;

      m_signal.m_connections.remove (this);
      m_connected = false;
    }

  private:
    friend class signal;
    connection (std::function<void (Args...)> &&handler, signal &s) : m_handler (std::move (handler)), m_signal (s) {}

    std::function<void (Args...)> m_handler;
    signal &m_signal;
    bool m_connected = true;
  };

  signal ();
  ~signal ();

  signal (const signal &) = delete;
  signal (signal &&) = delete;

  signal &operator= (const signal &) = delete;
  signal &operator= (signal &&) = delete;

  connection connect (std::function<void (Args...)>);
  void notify (Args... args);

private:
  std::vector<connection *> m_connections;
};

//----------------------------------------------

template<typename... Args>
signal<Args...>::signal ()
{
}

template<typename... Args>
signal<Args...>::~signal ()
{
  for (connection *con : m_connections)
    con->m_connected = false;
}

template<typename... Args>
typename signal<Args...>::connection signal<Args...>::connect (std::function<void (Args...)> handler)
{
  connection con (std::move (handler), *this);
  m_connections.emplace_back (&con);
  return con;
}

template<typename... Args>
void signal<Args...>::notify (Args... args)
{
  for (connection *con : m_connections)
    con->m_handler (args...);
}
}  // namespace utils
