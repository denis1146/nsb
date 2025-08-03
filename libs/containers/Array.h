#pragma once

#include <cstddef>
#include <iterator>

namespace nsb::containers {

template<class T, std::size_t N>
struct Array
{
  using type_array = T[N];
  using value_type = T;
  using size_type	= std::size_t;
  using difference_type	= std::ptrdiff_t;
  using reference =	T&;
  using const_reference =	const T&;
  using pointer =	T*;
  using const_pointer =	const T*;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;

  reference at(size_type pos);
  const_reference at(size_type pos) const;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  
  T* data() noexcept;
  const T* data() const noexcept;

  constexpr iterator begin();
  constexpr const_iterator begin() const;
  constexpr const_iterator cbegin();
  constexpr iterator end();
  constexpr const_iterator end() const;
  constexpr const_iterator cend();

  constexpr reverse_iterator rbegin();
  constexpr const_reverse_iterator rbegin() const;
  constexpr const_reverse_iterator crbegin();
  constexpr reverse_iterator rend();
  constexpr const_reverse_iterator rend() const;
  constexpr const_reverse_iterator crend();

  static constexpr bool empty() noexcept;
  static constexpr size_type size() noexcept;
  static constexpr size_type max_size() noexcept;
  constexpr void fill(const T& value);
  void swap(Array& other);

  T arr[N];
  static constexpr size_type static_size = N;
};

template<class T>
struct Array<T, 0>
{
  using value_type = T;
  using size_type	= std::size_t;

  static constexpr size_type static_size = 0;

 
  static constexpr bool empty() noexcept
  { return true; }

  static constexpr size_type size() noexcept
  { return static_size; }

  static constexpr size_type max_size() noexcept
  { return static_size; }

};

template<typename T, std::size_t N>
constexpr bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs);

template<typename T, std::size_t N>
constexpr bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs);

template<typename T, std::size_t N>
constexpr bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs);

template<typename T, std::size_t N>
constexpr bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs);

template<typename T, std::size_t N>
constexpr bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs);

template<typename T, std::size_t N>
constexpr bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs);

template<typename T, std::size_t N>
constexpr auto operator<=>(const Array<T, N>& lhs, const Array<T, N>& rhs)
-> decltype(lhs[0] <=> rhs[0]);

template<typename T, std::size_t N>
void swap(T (&lhs)[N], T (&rhs)[N]) noexcept(noexcept(std::swap(*lhs, *rhs)));

template<typename T, std::size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(nsb::containers::swap(lhs.arr, rhs.arr)));

} // nsb::containers

#include "Array.hpp"
