#pragma once

#include <thread>

class JThread
{
public:
  JThread() noexcept = default;
  template <class Callable, class ...Args>
  explicit JThread(Callable&& func, Args&&... args)
  : m_t{ std::forward<Callable>(func), std::forward<Args>(args)... }
  {}
  explicit JThread(std::thread t) noexcept
  : m_t{ std::move(t) }
  {}
  JThread& operator=(std::thread t)
  {
    if (m_t.joinable())
      m_t.join();
    m_t = std::move(t);
    return *this;
  }


  JThread( JThread&& other) noexcept
  : m_t{ std::move(other.m_t) }
  {}
  JThread& operator=(JThread&& other) noexcept
  {
    if (m_t.joinable())
      m_t.join();
    m_t = std::move(other.m_t);
    return *this;
  }


  ~JThread()
  {
    if (m_t.joinable())
      m_t.join();
  }

  void swap(JThread& other) noexcept
  {
    m_t.swap(other.m_t);
  }

private:
  std::thread m_t;
};
