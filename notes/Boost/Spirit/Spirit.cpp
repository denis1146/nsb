#include "Spirit.h"

#include <iostream>
#include <string_view>

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/scope_exit.hpp>

#include "NoteHelpers.h"

namespace 
{

// DD.MM.YYYY
// D.MM.YYYY
struct Data
{
  uint8_t day{};
  uint8_t month{};
  uint16_t year{};
};

std::ostream& operator<<(std::ostream& os, const Data& d)
{
  if (d.day < 10) os << '0';
  os << static_cast<int>(d.day) << '.';
  if (d.month < 10) os << '0';
  os << static_cast<int>(d.month) << '.'
     << static_cast<int>(d.year);
  return os;
}

Data parseData(std::string_view s)
{
  using boost::spirit::qi::_1;
  using boost::spirit::qi::char_;
  using boost::spirit::qi::ushort_;
  using boost::spirit::qi::uint_parser;
  using boost::phoenix::ref;
  Data ret;
  const auto y = [&ret](uint16_t v) { ret.year = v; };
  uint_parser<uint8_t, 10, 1, 2> day_parser_;
  
  const char* begin = s.begin();
  const bool success = boost::spirit::qi::parse(begin, s.end(),
    day_parser_[ref(ret.day) = _1]   >> '.' >>
    ushort_[ref(ret.month) = _1] >> char_('.') >>
    ushort_[y]
  );
  if (!success || begin != s.end()) 
    throw std::runtime_error("parsing failed");

  return ret;
}


// YYYY.DD.MMThh:mm:ss
// YYYY.DD.MMThh:mm:ssZ
// YYYY.DD.MMThh:mm:ss+hh:mm
// YYYY.DD.MM-hh:mm:ss
// YYYY.DD.MM-hh:mm
// hh:mm:ss+hh:mm
// hh:mm+hh:mm

struct Time
{
  uint8_t hours{};
  uint8_t minutes{};
  uint8_t seconds{};
};

enum class ZoneOffsets {
  NOT_SET,
  Z,
  UTC_PLUS,
  UTC_MINUS,
};

struct DataTime
{
  Data d{};
  Time t{};
  ZoneOffsets zone{ ZoneOffsets::NOT_SET };
  uint16_t zone_offset_in_minutes{};
};

void set_zone_offset(DataTime& dt, char sign, uint8_t hours, uint8_t minutes) {
  dt.zone = (sign == '+') ? ZoneOffsets::UTC_PLUS : ZoneOffsets::UTC_MINUS;
  dt.zone_offset_in_minutes = hours * 60 + minutes;
};

std::ostream& operator<<(std::ostream& os, const Time& t)
{
  if (t.hours < 10) os << '0';
  os << static_cast<int>(t.hours) << ':';
  if (t.minutes < 10) os << '0';
  os << static_cast<int>(t.minutes) << ':';
  if (t.seconds < 10) os << '0';
  os << static_cast<int>(t.seconds);
  return os;
}

std::ostream& operator<<(std::ostream& os, const DataTime& dt)
{
  if (dt.d.year != 0 || dt.d.month != 0 || dt.d.day != 0) os << dt.d << 'T';
  os << dt.t;
  switch (dt.zone) {
    case ZoneOffsets::Z:
      os << 'Z';
      break;
    case ZoneOffsets::UTC_PLUS: 
      os << '+';
      [[fallthrough]];
    case ZoneOffsets::UTC_MINUS: 
      if (dt.zone == ZoneOffsets::UTC_MINUS) os << '-';

      if (dt.zone_offset_in_minutes / 60 < 10) os << '0';
      os << dt.zone_offset_in_minutes / 60 << ':';
      if (dt.zone_offset_in_minutes % 60 < 10) os << '0';
      os << dt.zone_offset_in_minutes % 60;
      break;
    default:
      break;
  }

  return os;
}

DataTime parseDataTime(std::string_view s)
{
  using boost::spirit::qi::_1;
  using boost::spirit::qi::_2;
  using boost::spirit::qi::_3;
  using boost::spirit::qi::char_;
  using boost::spirit::qi::uint_parser;
  using boost::phoenix::ref;
  using boost::phoenix::bind;
  uint_parser<uint8_t, 10, 2, 2> u2_;
  uint_parser<uint16_t, 10, 4, 4> u4_;

  DataTime ret;
  boost::spirit::qi::rule<const char*, void()> timezone_parser =
  -(
    char_('Z')[ref(ret.zone) = ZoneOffsets::Z]
    |
    ((char_('+')|char_('-')) >> u2_ >> ':' >> u2_)[
      bind(&set_zone_offset, ref(ret), _1, _2, _3)
    ]
  );

  boost::spirit::qi::rule<const char*, void()> data_parser =
    u4_[ref(ret.d.year) = _1] >> '.'>>
    u2_[ref(ret.d.month) = _1] >> '.' >>
    u2_[ref(ret.d.day) = _1];
  
  boost::spirit::qi::rule<const char*, void()> time_parser =
    (u2_[ref(ret.t.hours) = _1] >> ':' >>
     u2_[ref(ret.t.minutes) = _1] >>
     -(':' >> u2_[ref(ret.t.seconds) = _1]));


  const char* begin = s.begin();
  const bool success = boost::spirit::qi::parse(begin, s.end(),
    (
      (data_parser >> (char_('T') | char_('-')) >> time_parser)
      | data_parser
      | time_parser
    )
    >> timezone_parser
  );

  if (!success || begin != s.end()) 
    throw std::runtime_error("DataTime parsing failed");
  return ret;
}

}

void Spirit::run()
{
  constexpr char noteTitle[] = "Spirit";
  nsb::showNoteTitle(noteTitle);
  return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  try {
    auto d = parseData("1.01.1970");
    std::cout << "Data: " << d << std::endl;
    d = parseData("02.01.1970");
    std::cout << "Data: " << d << std::endl;
  }
  catch(const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
  }
  std::cout << std::endl;

  try {
    auto showDt = [](const DataTime& dt) { std::cout << "DataTime: " << dt << std::endl; };

    const auto dt = parseDataTime("1970.01.01T01:23:45");
    showDt(dt);
    showDt(parseDataTime("1970.01.01T01:23:45Z"));
    showDt(parseDataTime("1970.01.01T01:23:45+03:00"));
    showDt(parseDataTime("1970.01.01T01:23:45"));
    showDt(parseDataTime("1970.01.01T01:23"));
    showDt(parseDataTime("01:23:45+03:00"));
    showDt(parseDataTime("01:23+03:00"));
    showDt(parseDataTime("12:34"));
    showDt(parseDataTime(""));
  }
  catch(const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}