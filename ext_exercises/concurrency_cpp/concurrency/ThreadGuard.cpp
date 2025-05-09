#include "ThreadGuard.h"

ThreadGuard::ThreadGuard(std::thread& t)
: m_t{ t }
{
}

ThreadGuard::~ThreadGuard()
{
  if (m_t.joinable())
    m_t.join();
}
