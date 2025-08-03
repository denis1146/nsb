#pragma once

#include <type_traits>
#include <iterator>

namespace nsb {

template<class Enum>
constexpr auto toUType (Enum value) noexcept
{
  return static_cast<std::underlying_type_t<Enum>>(value);
}

} // ~nsb
