#pragma once

namespace nsb {

template <class T, T v>
struct integral_constant
{
  using value_type = T;
  using type = integral_constant;

  static constexpr T value = v;

  constexpr value_type operator()() const noexcept
  { return value; }

  constexpr operator value_type() const noexcept
  { return value; }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <class T, class V>
struct is_same: nsb::false_type {};

template <class T>
struct is_same<T, T>: nsb::true_type {};

template <class T, class V>
constexpr bool is_same_v = is_same<T, V>::value;

} // ~nsb
