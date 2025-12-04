#include "SimpleAny.h"

#include <memory>
#include <type_traits>
#include <iostream>
#include <typeindex>
#include <string_view>
#include <source_location>
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


  // === 1
  // Bypassing RTTI Disabling

  // RTTI enabled
  template<class T>
  std::type_index type_id_rtti_enabled() {
    return typeid(T);
  }

  // RTTI disabled
  struct my_type_index {
    explicit my_type_index(std::string_view name) noexcept : m_name{name} {}
    std::string_view name() const noexcept { return m_name; }
    std::string_view m_name;
  };

  inline bool operator==(const my_type_index& lhs, const my_type_index& rhs) {
    return lhs.name() == rhs.name();
  }

  inline bool operator!=(const my_type_index& lhs, const my_type_index& rhs) {
    return !(lhs == rhs);
  }

  template<class T>
  inline my_type_index type_id_rtti_disabled() {
    return my_type_index{ std::source_location::current().function_name() };
  }

  void disablingRTTI() {
    std::cout << std::endl;
    std::cout << "1. Bypassing RTTI Disabling" << std::endl;

    assert(type_id_rtti_enabled<int>() == type_id_rtti_enabled<int>());
    assert(type_id_rtti_enabled<int>() != type_id_rtti_enabled<unsigned int>());

    assert(type_id_rtti_disabled<int>() == type_id_rtti_disabled<int>());
    assert(type_id_rtti_disabled<int>() != type_id_rtti_disabled<unsigned int>());
  }
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
  
  // Bypassing RTTI Disabling
  disablingRTTI();
}
