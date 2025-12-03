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

#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>

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

// === 7
// Getting the result of a function

  struct s1{};
  struct s2{};
  struct s3{};

  inline s3 operator+(const s1&, const s2&) {
    return s3();
  }
  inline s3 operator+(const s2&, const s1&) {
    return s3();
  }

  namespace result_of {

  template<class T1, class T2>
  struct res_of_cpp03 {
    typedef typename std::common_type<T1, T2>::type type;
  };

  template<>
  struct res_of_cpp03<s1, s2> {
    typedef s3 type;
  };

  template<>
  struct res_of_cpp03<s2, s1> {
    typedef s3 type;
  };

  }

  template<class T1, class T2>
  typename result_of::res_of_cpp03<T1, T2>::type
  my_add_cpp03(const T1& v1, const T2& v2) {
    return v1 + v2;
  }

  template<class T1, class T2>
  auto
  my_add_cpp11(const T1& v1, const T2& v2) -> decltype(v1 + v2) {
    return v1 + v2;
  }

// === 8
// Lazy evaluation of metafunctions
  template<class T>
  struct identity_t{
    using type = T;
  };

  struct fallback_t;

  template<class Cond, class Param, class Func, class Fallback = fallback_t>
  struct apply_if;

  template<class Cond, class Param, class Func, class Fallback>
  struct apply_if {
    using condition_t = boost::mpl::apply<Cond, Param>::type;
    using applied_t   = boost::mpl::apply<Func, Param>;

    using type = typename boost::mpl::
      eval_if_c<condition_t::value, applied_t, identity_t<Fallback>>::type;
  };

  void lazyEval() {
    using boost::mpl::_1;
    using boost::mpl::_2;

    using res1_t = apply_if<std::is_integral<_1>, int, std::make_unsigned<_1>>::type;
    static_assert(std::is_same_v<res1_t, unsigned int>);
    
    using res2_t = apply_if<std::is_integral<_1>, float, std::make_unsigned<_1>>::type;
    static_assert(std::is_same_v<res2_t, fallback_t>);
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

  // === 7
  // Getting the result of a function
  std::cout << "7. Getting the result of a function.\t";
  s1 v1;
  s2 v2;
  [[maybe_unused]] s3 v31_cpp03 = my_add_cpp03(v1, v2);
  [[maybe_unused]] s3 v32_cpp03 = my_add_cpp03(v2, v1);
  std::cout << my_add_cpp03('\5', 2) << "\t";

  [[maybe_unused]] s3 v31 = my_add_cpp11(v1, v2);
  [[maybe_unused]] s3 v32 = my_add_cpp11(v2, v1);
  std::cout << my_add_cpp11('\5', 2) << "\t";
  std::cout << my_add_cpp11(std::string{"4"}, "2") << std::endl;

  // === 8
  // Lazy evaluation of metafunctions
  lazyEval();
}
