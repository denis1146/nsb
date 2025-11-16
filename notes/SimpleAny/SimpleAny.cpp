#include "SimpleAny.h"

#include <memory>
#include <type_traits>
#include <iostream>
#include <boost/scope_exit.hpp>

#include "NoteHelpers.h"

namespace {

struct placeholder {
  virtual ~placeholder() = default;
  virtual const std::type_info& type() const = 0;
};

template <class ValueType>
struct holder: placeholder {
  holder() = default;

  template<class U, typename = std::enable_if_t<!std::is_base_of_v<holder, std::decay_t<U>>>>
  holder( U&& value ) : help{ std::forward<U>(value) } {
    static_assert(std::is_constructible_v<ValueType, U>, "Error: ValueType must be constructible from U");
  }

  const std::type_info& type() const override {
    return typeid(ValueType);
  };
  ValueType help;
};

struct Any {
  template<class T>
  Any(T&& rhs)
  : m_up{ std::make_unique<holder<T>>(std::forward<T>(rhs))}
  {}

  template <class T>
  void operator=(T&& rhs) {
    m_up = std::make_unique<holder<T>>(std::forward<T>(rhs));
  }

  template<class T>
  T& get()
  {
    if (!m_up || m_up->type() != typeid(T))
      throw std::runtime_error("Error: Invalid type"); 

    return static_cast<holder<T>*>(m_up.get())->help;
  }
  std::unique_ptr<placeholder> m_up;
};

}

void SimpleAny::run()
{
  constexpr char noteTitle[] = "SimpleAny";
  nsb::showNoteTitle(noteTitle);
  return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  Any any = std::string("123");
  auto& st = any.get<std::string>();
  st += " 456";
  std::cout << "st = " << any.get<std::string>() << std::endl;

  any = 555;
  auto& n = any.get<int>();
  n += 222;
  std::cout << "number = " << any.get<int>() << std::endl;

  try
  {
    any.get<double>();
  }
  catch(const std::runtime_error& e)
  {
    std::cerr << e.what() << '\n';
  }

  auto a1= holder<std::string>();
  auto a2= holder<std::string>("123");
  auto a3= holder<std::string>(std::move(a2));
  auto a4= holder<std::string>(a3);
  // auto a5= holder<std::string>(555);

  a1 = a4;
  a1 = std::move(a2);
}