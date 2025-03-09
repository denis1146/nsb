
#include <iostream>
#include <boost/type_index.hpp>


namespace unit_1
{
// ------------------------------------------------------ 1
template<class T>
void f_rT(T& param) {
  int i{};
  T t_type = i;
  param;
}

template<class T>
void f_crT(const T& param) {
  T t_type{};
  param;
}

template<class T>
void f_pT(T* param) {
  T t_type{};
  param;
}
// ------------------------------------------------------ 2
template<class T>
void f_urT(T&& param) {
  int i{};
  T t_type = i;
  param;
}
// ------------------------------------------------------ 3
template<class T>
void f_T(T param) {
  T t_type{};
  param;
}
// ------------------------------------------------------
template<class T>
void f_rmT(T& param) {
  T t_type = {};
  param;
}

template<class T, std::size_t Size = 0>
constexpr std::size_t arraySize(T(&a)[Size]) noexcept {
  return Size;
}
// ------------------------------------------------------

void test_type()
{
  int x = 42;
  const int cx = x;
  const int& rcx = cx;
  int& rx = x;
  const int* px = &x;
  const int* const pcrx = &x;

  // ------------------------------------------------------ 1 &
  f_rT(x);            // T = int;               param = int&
  f_rT(rx);           // T = int;               param = int&
  f_rT(cx);           // T = const int;         param = const int&
  f_rT(rcx);          // T = const int;         param = const int&
  // f_rT(42);  Erorr

  f_crT(x);           // T = int;               param = const int&
  f_crT(rx);          // T = int;               param = const int&
  f_crT(cx);          // T = int;               param = const int&
  f_crT(rcx);         // T = int;               param = const int&
  f_crT(42);          // T = int;               param = const int&

  f_pT(&x);           // T = int;               param = int*
  f_pT(px);           // T = const int;         param = const int*
  // ------------------------------------------------------ 2 &&
  f_urT(x);           // T = int&;              param = int&
  f_urT(rx);          // T = int&;              param = int&
  f_urT(cx);          // T = const int&;        param = const int&
  f_urT(rcx);         // T = const int&;        param = const int&
  f_urT(42);          // T = int;               param = int&&
  // ------------------------------------------------------ 3 T
  f_T(x);             // T = int;               param = int
  f_T(rx);            // T = int;               param = int
  f_T(cx);            // T = int;               param = int
  f_T(rcx);           // T = int;               param = int
  f_T(pcrx);          // T = const int*;        param = const int*
  f_T(42);            // T = int;               param = int
  // ------------------------------------------------------
}

// ------------------------------------------------------

void someFunc(int, double) {
}

template<class T>
void f_fT(T param) {
  T t_type{};
}

template<class T>
void f_rfT(T& param) {
  T* t_type{};
}


void test_array_and_func()
{
  const int array[] = {1, 2, 3}; // array type
  const int* ptrToArray = array; // different types

  f_T(array);         // T = const int*;        param = const int*
  f_T(ptrToArray);    // T = const int*;        param = const int*
  f_rmT(array);       // T = const int[3];      param = const (&)int[3]
  f_rmT(ptrToArray);  // T = const int*;        param = const int*(&)

  auto sz = arraySize(array);
  sz = sizeof(array)/sizeof(*array);
  sz = std::size(array);
  sz = std::ssize(array);

  // someFunc = void(int, double)
  f_fT(someFunc);     // param = void (*)(int, double);   T = void (*)(int, double)   
  f_rfT(someFunc);    // param = void (&)(int, double);   T = void (int, double);
}

// ------------------------------------------------------

template<class T>
void f_initListT(std::initializer_list<T> initList) {
  T t_type{};
  initList;
}

// Error
// auto createInitList() {
//   return {1, 2, 3};   // Error
// }

void test_auto()
{
  auto x = 42;            // int
  const auto cx = x;      // const int
  const auto& rcx = x;    // const int&

  // universal reference
  auto&& uref1 = x;       // int&
  auto&& uref2 = cx;      // const int&
  auto&& uref3 = 5;       // int&&

  const char st[] = "123";
  auto arr1 = st;         // const char*
  auto& arr2 = st;        // const char(&)[4]

  // someFunc = void(int, double)
  auto funct1 = someFunc; // void (*)(int, double)
  auto funct2 = someFunc; // void (&)(int, double)


  // int x1 = 27;         // c++98
  // int x2(27);          // c++98
  // int x3 = { 27 };     // c++11
  // int x4{ 27 };        // c++11
  
  auto x1 = 27;           // int
  auto x2(27);            // int
  auto x3 = { 27 };       // std::initializer_list<int>
  auto x4{ 27 };          // int  ???  std::initializer_list<int>
  
  // auto x5 = { 1, 2, 3.3 };     // Error
  // f_T({1, 2});                 // Eror
  f_initListT({1, 2});

  auto createInitL = [](const auto& v) -> auto {
    // return {1, 2, 3};                 // Error
    return v;
  };
  auto a = createInitL(42);
  // auto a = createInitL({1, 2, 3});     // Error
}

// ------------------------------------------------------

template<class Cont, class Index>
auto val_0(Cont& c, Index i)
{
  return c[i];
}

template<class Cont, class Index>
auto val_1(Cont& c, Index i)
-> decltype(c[i])
{
  return c[i];
}

template<class Cont, class Index>
decltype(auto) val_2(Cont& c, Index i)
{
  return c[i];
}

template<class Cont, class Index>
decltype(auto) val_uref(Cont&& c, Index i)
{
  return std::forward<Cont>(c)[i];
}


int gX = 555;
decltype(auto) fd1() {
  return gX;    // int
}

decltype(auto) fd2() {
  return (gX);  // int&
}

void test_decltype()
{
  int array[] = {1, 2, 3};
  auto&& a1 = val_0(array, 2);  // int&&
  a1 = 5;
  auto&& a2 = val_1(array, 2);  // int&
  a2 = 6;
  auto&& a3 = val_2(array, 2);  // int&
  a3 = 7;

  auto x1 = array[2];                 // int
  x1 = 5;
  decltype(array[2]) x2 = array[2];   // int&
  x2 = 6;
  decltype(auto) x3 = array[2];       // int&
  x3 = 7;

  int i = 42;                   // int
  decltype(i) di = i;           // int    <- only 
  decltype((i)) dri = i;        // int&   <- always for expressions
  auto&& y1 = fd1();    // int&&
  y1 = 42;
  auto&& y2 = fd2();    // int&
  y2 = 42;

}

// ------------------------------------------------------

template<class T>
class TD;

template<class T>
void f_td(const T& param)
{
  using std::cout;
  using std::endl;
  using boost::typeindex::type_id_with_cvr;

  cout << "T = "
       << type_id_with_cvr<T>().pretty_name()
       << endl;

  cout << "param = "
       << type_id_with_cvr<decltype(param)>().pretty_name()
       << endl;
}

void test_show_type()
{
  int a = 5;
  // TD<decltype(a)> t;

  f_td(a);


}

void test()
{
  test_type();
  test_array_and_func();
  test_auto();
  test_decltype();
  test_show_type();
  std::cout << std::endl;
}

}