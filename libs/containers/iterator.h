#pragma once

namespace nsb {

template <class Iter>
using iterator_traits_t = typename std::iterator_traits<Iter>::value_type;

} // ~nsb

namespace nsb::containers {

template <class Container>
auto begin(Container& c) -> decltype(c.begin())
{
  return c.begin();
}

template <class Container>
auto begin(const Container& c) -> decltype(c.begin())
{
  return c.begin();
}

template <class Container>
auto cbegin(const Container& c) -> decltype(nsb::containers::begin(c))
{
  return nsb::containers::begin(c);
}

template <class Container>
auto end(Container& c) -> decltype(c.end())
{
  return c.end();
}

template <class Container>
auto end(const Container& c) -> decltype(c.end())
{
  return c.end();
}

template <class Container>
auto cend(const Container& c) -> decltype(nsb::containers::end(c))
{
  return nsb::containers::end(c);
}

template <class Container>
auto rbegin(Container& c) -> decltype(c.rbegin())
{
  return c.rbegin();
}

template <class Container>
auto rbegin(const Container& c) -> decltype(c.rbegin())
{
  return c.rbegin();
}

template <class Container>
auto crbegin(const Container& c) -> decltype(nsb::containers::rbegin(c))
{
  return nsb::containers::rbegin(c);
}

template <class Container>
auto rend(Container& c) -> decltype(c.rend())
{
  return c.rend();
}

template <class Container>
auto rend(const Container& c) -> decltype(c.rend())
{
  return c.rend();
}

template <class Container>
auto crend(const Container& c) -> decltype(nsb::containers::rend(c))
{
  return nsb::containers::rend(c);
}

} // ~nsb::containers
