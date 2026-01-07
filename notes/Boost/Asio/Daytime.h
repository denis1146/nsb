#pragma once

#include <string>
#include <ctime>

static std::string daytime()
{
  std::time_t now = std::time(nullptr);
  return std::ctime(&now);
}
