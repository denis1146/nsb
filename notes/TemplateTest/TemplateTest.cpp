#include "TemplateTest.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <type_traits>
#include <vector>
#include <cstdint>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/has_pre_increment.hpp>
#include <boost/type_traits/has_post_increment.hpp>
#include <boost/type_traits/has_plus.hpp>
#include <boost/type_traits/has_plus_assign.hpp>
#include <boost/scope_exit.hpp>

#include "NoteHelpers.h"

namespace
{

// === 1
template<class T>
using my_add_const_t = typename std::add_const<T>::type;

template<class T>
inline constexpr bool my_is_const_v = std::is_const<T>::value;

// === 2
template<class T>
std::enable_if_t<!std::is_floating_point_v<T>>
disable(const T& v) {
  static_assert(!std::is_floating_point_v<T>);
  std::cout << static_cast<int>(v) << (" (int)\t");
}

template<class T>
std::enable_if_t<std::is_floating_point_v<T>>
disable(const T& v) {
  static_assert(std::is_floating_point_v<T>);
  std::cout << static_cast<float>(v) << " (Float)\t";
}

// === 3
template<class T, class EnableT = void>
struct Enable {
  void foo(const T&) { /*static_assert(false);*/ assert(false);}
};

template<class T>
struct Enable<T, std::enable_if_t<std::is_floating_point_v<T>>> {
    void foo(const T& v) { std::cout << v << " (Float)\t"; }
};

template<class T>
struct Enable<T, std::enable_if_t<std::is_integral_v<T>>> {
    void foo(const T& v) { std::cout << v << " (Int)\t"; }
};

template<class T>
void enable_func(const T& v) {
  Enable<T>{}.foo(v);
}

// === 4
template<class T, class Tag>
T calc_impl(const T& /*v*/ , Tag /*ignore*/) {
  // static_assert(sizeof(v) == 1);
  return 42;
}

template<class T>
T calc_impl(const T& v, boost::mpl::int_<1>) {
  return v;
}

template<class T>
T calc(const T& v) {
  return calc_impl(v, boost::mpl::int_<sizeof(T)>{});
}

// === 5
template<class T>
struct is_vector: std::false_type{};

template<class T, class Allocator>
struct is_vector<std::vector<T, Allocator>>: std::true_type{};

// === 6
struct pre_inc {
  template<class T>
  void operator()(T& v) const { ++v; }
};

struct post_inc {
  template<class T>
  void operator()(T& v) const { v++; }
};

struct plus {
  template<class T>
  void operator()(T& v) const { v = v + static_cast<T>(1); }
};

struct plus_assign {
  template<class T>
  void operator()(T& v) const { v += static_cast<T>(1); }
};

template<class T>
void inc(T& v) {
  // ++v
  // v++
  // v += 1
  // v = v + 1

  using step_0_t = plus;
  using step_1_t = std::conditional_t<boost::has_plus_assign<T>::value, plus_assign, step_0_t>;
  using step_2_t = std::conditional_t<boost::has_post_increment<T>::value, post_inc, step_1_t>;
  using step_3_t = std::conditional_t<boost::has_pre_increment<T>::value, pre_inc, step_2_t>;
  static_assert(!std::is_same_v<step_3_t, plus> || boost::has_plus<T>::value,
    "The type does not have a + operator.");
  step_3_t()(v);

  // if constexpr (boost::has_pre_increment<T>::value)
  //   ++v;
  // else if constexpr (boost::has_post_increment<T>::value)
  //   v++;
  // else if constexpr (boost::has_plus_assign<T>::value)
  //   v += static_cast<T>(1);
  // else if constexpr (boost::has_plus<T>::value)
  //   v = v + static_cast<T>(1);
  // // else 
  // //   static_assert(false);
}

}

void TemplateTest::run()
{
  constexpr char noteTitle[] = "TemplateTest";
  nsb::showNoteTitle(noteTitle);
  return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  // === 1
  int nonConst = 42;
  std::cout << "1. Const: " << my_is_const_v<decltype(nonConst)> << " " <<
    my_is_const_v<my_add_const_t<decltype(nonConst)>> << std::endl;

  // === 2
  std::cout << "2. Disabel (Function): ";
  disable(42);
  disable(0.42);
  std::cout << "\n";

  // === 3
  std::cout << "3. Enable  (Class)   : ";
  enable_func(42);
  enable_func(0.42);
  std::cout << "\n";

  // === 4
  std::cout << "4. Calc              : "
    << (int)calc(static_cast<uint8_t>(255)) << "(int_<1>); "
    << calc(static_cast<uint16_t>(100)) << std::endl;

  // === 5
  std::cout << "5. Is_Vector         : " << is_vector<int>::value
    << " " << is_vector<std::vector<int>>::value << std::endl;

  // === 6
  int inc_v = 41;
  inc(inc_v);
  std::cout << "6. Has_Increment     : " << inc_v << std::endl;
}

