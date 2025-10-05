#pragma once

class ScopeExit {
public:
  void run();

private:
  const char* const m_se =
    "================== ScopeExit ==================";
};
