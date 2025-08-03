#include <stdexcept>
#include <algorithm>

namespace nsb::containers {

template<class T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::operator[](size_type pos)
{
  return arr[pos];
}

template<class T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::operator[](size_type pos) const
{
  return arr[pos];
}

template<class T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::at(size_type pos)
{
  if (pos >= std::size(arr))
    throw std::out_of_range("Array::at: pos is not within container range.");
  return arr[pos];
}

template<class T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::at(size_type pos) const
{
  if (pos >= std::size(arr))
    throw std::out_of_range("Array::at: pos is not within container range.");
  return arr[pos];
}

template<class T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::front()
{
  return arr[0];
}

template<class T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::front() const
{
  return arr[0];
}

template<class T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::back()
{
  return arr[N - 1];
}

template<class T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::back() const
{
  return arr[N - 1];
}

template<class T, std::size_t N>
T* Array<T, N>::data() noexcept
{
  return arr;
}

template<class T, std::size_t N>
const T* Array<T, N>::data() const noexcept
{
  return arr;
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::iterator Array<T, N>::begin()
{
  return iterator(data());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_iterator Array<T, N>::begin() const
{
  return const_iterator(data());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_iterator Array<T, N>::cbegin()
{
  return const_iterator(data());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::iterator Array<T, N>::end()
{
  return iterator(data() + N);
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_iterator Array<T, N>::end() const
{
  return const_iterator(data() + N);
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_iterator Array<T, N>::cend()
{
  return const_iterator(data() + N);
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::reverse_iterator Array<T, N>::rbegin()
{
  return reverse_iterator(end());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_reverse_iterator Array<T, N>::rbegin() const
{
  return const_reverse_iterator(end());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_reverse_iterator Array<T, N>::crbegin()
{
  return const_reverse_iterator(end());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::reverse_iterator Array<T, N>::rend()
{
  return reverse_iterator(begin());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_reverse_iterator Array<T, N>::rend() const
{
  return const_reverse_iterator(begin());
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::const_reverse_iterator Array<T, N>::crend()
{
  return const_reverse_iterator(begin());
}

template<class T, std::size_t N>
constexpr bool Array<T, N>::empty() noexcept
{
  return N == 0;
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::size_type Array<T, N>::size() noexcept
{
  return N;
}

template<class T, std::size_t N>
constexpr typename Array<T, N>::size_type Array<T, N>::max_size() noexcept
{
  return size();
}

template<class T, std::size_t N>
constexpr void Array<T, N>::fill(const T& value)
{
  for (auto& item : arr)
    item = value;
}

template<class T, std::size_t N>
void Array<T, N>::swap(Array& other)
{
  nsb::containers::swap(*this, other);
}

template<typename T, std::size_t N>
constexpr bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
  return std::equal(std::cbegin(lhs), std::cend(lhs), std::cbegin(rhs));
}

template<typename T, std::size_t N>
constexpr bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return !(lhs == rhs);
}

template<typename T, std::size_t N>
constexpr bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
  return std::lexicographical_compare(std::cbegin(lhs), std::cend(lhs)
  , std::cbegin(rhs), std::cend(rhs));
}

template<typename T, std::size_t N>
constexpr bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
  return !(rhs < lhs);
}

template<typename T, std::size_t N>
constexpr bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
  return rhs < lhs;
}

template<typename T, std::size_t N>
constexpr bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
  return !(lhs < rhs);
}

template<typename T, std::size_t N>
constexpr auto operator<=>(const Array<T, N>& lhs, const Array<T, N>& rhs)
-> decltype(lhs[0] <=> rhs[0])
{
  return std::lexicographical_compare_three_way(std::cbegin(lhs), std::cend(lhs)
  , std::cbegin(rhs), std::cend(rhs));
}

template<typename T, std::size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs)
{
  std::swap(lhs.arr, rhs.arr);  
}

} // nsb::containers
