#pragma once

#include <thread>

class ThreadGuard
{
public:
  explicit ThreadGuard(std::thread& t);
  ThreadGuard(const ThreadGuard&) = delete;
  ThreadGuard& operator=(const ThreadGuard&) = delete;
  ~ThreadGuard();

private:
  std::thread& m_t;
};
