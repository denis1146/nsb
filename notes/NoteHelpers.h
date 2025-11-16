#pragma once

#include <format>
#include <string_view>
#include <iostream>

namespace nsb {

inline void showNoteTitle(std::string_view note) noexcept
{
  constexpr size_t maxSz{ 48 };
  size_t sz = note.size() >= maxSz ? 0 : (maxSz - note.size()) / 2;
  std::cout << std::format("{0} {1} {0}{2}\n", std::string(sz, '='), 
    note.data(), (note.size() % 2 ? '=' : ' '));
}

};
