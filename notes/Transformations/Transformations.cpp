#include "Transformations.h"

#include <iostream>
#include <sstream>
#include <iostream>
#include <charconv>
#include <locale>
#include <exception>
#include <boost/scope_exit.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/polymorphic_pointer_cast.hpp>

#include "NoteHelpers.h"

namespace
{

template <class Target, class Source>
struct overflow_checker
{
  void operator()(boost::numeric::range_check_result r) const
  {
    if (r != boost::numeric::cInRange)
      throw std::logic_error("bad numeric conversion: out of range (my_cast)");
  }
};

template <class Target, class Source>
Target my_cast(const Source& in)
{
  using conv_traits = boost::numeric::conversion_traits<Target, Source>;
  using converter = boost::numeric::converter<Target, Source,
    conv_traits,
    overflow_checker<Target, Source>>;
  return converter::convert(in);
}

void numbertoNumber()
{
    // 1
    [[maybe_unused]] uint8_t v{};
    try {
      v = boost::numeric_cast<uint8_t>(555);
    }
    catch (const boost::numeric::positive_overflow& e) {
      std::cout << "Error (positive_overflow): " << e.what() << std::endl;
      v = boost::numeric_cast<uint8_t>(42);
    }
    catch(const boost::numeric::negative_overflow& e) {
      std::cout << "Error: " << e.what() << std::endl;
    }
    catch(const boost::numeric::bad_numeric_cast& e) {
      std::cout << "Error: " << e.what() << std::endl;
    }

    // 2 my_cast
    try {
      v = my_cast<uint8_t>(555);
    }
    catch(const std::exception& e) {
      std::cout << "Error (my_cast): " << e.what() << std::endl;
      v = my_cast<uint8_t>(0);
    }
}


template<typename T>
struct myNegativeNumber
{
  explicit myNegativeNumber(T v = 0): m_v{v} {}

  T val() const
  {
    return m_v;
  }

private:
  T m_v{};
};

using myNegativeNumber16 = myNegativeNumber<uint16_t>;
using myNegativeNumber32 = myNegativeNumber<uint32_t>;

inline std::ostream& operator<<(std::ostream& os, const myNegativeNumber16& num)
{
  os << '-' << num.val();
  return os;
}

inline std::istream& operator>>(std::istream& is, myNegativeNumber16& num)
{
  char sign;
  is >> sign;
  if (sign != '-') throw std::logic_error("This is not a negative number");

  uint16_t v{};
  is >> v;
  num = myNegativeNumber16{ v };
  return is;
}

template<class CharT>
std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const myNegativeNumber32& num)
{
  os << static_cast<CharT>('-') << num.val();
  return os;
}

template<class CharT>
std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is, myNegativeNumber32& num)
{
  CharT sign;
  is >> sign;
  if (sign != static_cast<CharT>('-')) throw std::logic_error("This is not a negative number");

  uint32_t v{};
  is >> v;
  num = myNegativeNumber32{ v };
  return is;
}

void toAndFromString()
{
  const auto num = boost::lexical_cast<myNegativeNumber16>("-42");
  std::cout << "myNegativeNumber16 (lexical_cast): " << num << std::endl;
  [[maybe_unused]] const auto st = boost::lexical_cast<std::string>(num);
  assert(st == "-42");
  [[maybe_unused]] const auto i = boost::lexical_cast<int>(num);
  assert(i == -42);

  const auto num2 = boost::lexical_cast<myNegativeNumber32>(L"-555");
  std::array<wchar_t, 10> buf{};
  buf = boost::lexical_cast<decltype(buf)>(num2);
  std::wcout << L"myNegativeNumber32 (lexical_cast L\"\"): " << buf.data() << std::endl;
}


struct B {
  virtual ~B() = default;
};

struct D1 : B {
  void show1(){ std::cout << "D1::show1()" << std::endl; };
};

struct D2 : B {
  void show2(){ std::cout << "D2::show2()" << std::endl; };
};

void polymorphic()
{
  std::shared_ptr<B> b = std::make_unique<D1>();
  if (auto d2 = boost::dynamic_pointer_cast<D2>(b)) d2->show2();

  try {
    auto d2 = boost::polymorphic_pointer_cast<D2>(b);
    // static_cast for NDEBUG=1 or ASSERT for NDEBUG=0
    // auto d2 = boost::polymorphic_pointer_downcast<D2>(b);
    d2->show2();
  }
  catch(const std::exception& e) {
    std::cerr << "Error (polymorphic_pointer_cast): " << e.what() << '\n';
  }
}

} // namespace

void Transformations::run() 
{
  constexpr char noteTitle[] = "Transformations";
  nsb::showNoteTitle(noteTitle);
  return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  // String to a number
  {
    // 1
    int v{};
    std::istringstream iss("1");
    iss >> v;

    // 2
    char* end;
    v = static_cast<int>(std::strtol("2", &end, 10));

    // 3
    // std::string_view st{"q"};
    // std::string_view st{"99999999999999999"};
    std::string_view st{"3"};
    auto [ptr, ec] = std::from_chars(st.data(), st.data() + st.size(), v);
    if (ec == std::errc::invalid_argument)
        std::cout << "This is not a number" << std::endl;
    else if (ec == std::errc::result_out_of_range)
        std::cout << "This number is larger than an int" << std::endl;
    
    // 4
    v = boost::lexical_cast<int>("4");
    try {
      v = boost::lexical_cast<uint8_t>("256");
    }
    catch(const boost::bad_lexical_cast& e) {
      std::cout << "Error (bad_lexical_cast): " << e.what() << std::endl;
      v = 42;
    }
    std::locale::global(std::locale( "en_US.UTF8"));
    v = boost::lexical_cast<int>("1,000");
    std::locale::global(std::locale( "ru_RU.UTF8"));
    v = boost::lexical_cast<int>("1 001");
    std::locale::global(std::locale::classic());
  }

  // Number to string
  {
    // 1
    std::string s;
    std::stringstream oss;
    oss << 1;
    oss >> s;

    // 2
    char buf[100];
    std::sprintf(buf, "%i", 2);
    s = buf;

    // 3
    s = std::to_string(3);

    // 4
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), 4);
    s = ec == std::errc{} ? std::string{ buf, ptr } : "Error";

    // 5
    s = boost::lexical_cast<std::string>(5);
  }

  // Number to number
  {
    numbertoNumber();
  }

  // To string, from string
  {
    toAndFromString();
  }

  // Polymorphic
  {
    polymorphic();
  }
}