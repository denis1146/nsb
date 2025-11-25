#include "CountingInString.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <span>
#include <ranges>
#include <array>
#include <locale>
#include <string_view>
#include <boost/scope_exit.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "NoteHelpers.h"

namespace
{

  const std::locale LOC = std::locale::classic();
  
  template<typename SequenceT>
  inline SequenceT to_upper_copy(const SequenceT& input) {
    return boost::algorithm::to_upper_copy(input, LOC);
  }

}

void CountingInString::run()
{
  constexpr char noteTitle[] = "CountingInString";
  nsb::showNoteTitle(noteTitle);
  return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  constexpr char txt[] = 
    "0 1 2 3 4 5 6 7 8 9 a b c d e f \n"
    "Test tEst teSt tesT \n"
    "test test_2 test_3\n"
    "STRING string."
    "string STRING!"
    " ";

  using split_iterator_t = boost::split_iterator<const char*>;
  split_iterator_t splitIt = boost::make_split_iterator(txt,
    boost::algorithm::token_finder(boost::is_any_of("\n.!")));

  for (size_t i = 1; !splitIt.eof(); ++splitIt, ++i) {
    boost::iterator_range<const char*> range = *splitIt;
    // std::span range{ splitIt->begin(), splitIt->end() }; // iterator_range
    // std::ranges::subrange<const char*> range = *splitIt; // iterator_range
    std::string_view st{ range.begin(), range.end() };
    std::cout << "String " << i << ":\t" << std::quoted(st) << std::endl;

    std::cout << "Whitespaces:\t" <<
      std::count(range.begin(), range.end(), ' ') <<
      std::endl;
    std::cout << std::endl;
  }
  std::cout << std::endl;

  size_t n{};
  using std::operator""s;
  using std::operator""sv;
  constexpr auto words{"1. 1 2. 1 2 3? 4"sv};
  // for (const auto word : std::ranges::split_view(words, ". "sv)) {
  for (const auto word : words | std::views::split(". "sv)) {
      std::cout << std::quoted(std::string_view(word)) << ' ';
      ++n;
  }
  std::cout << std::endl << "Number of lines: " << n << std::endl << std::endl;

  n = 0;
  for (const auto word : words | std::views::split(". "sv)) {
    for (const auto w : word | std::views::split("? "sv)) {
      std::cout << std::quoted(std::string_view(w)) << ' ';
      ++n;
    }
  }
  std::cout << std::endl << "Number of lines: " << n << std::endl << std::endl;

  std::cout << boost::algorithm::to_upper_copy("abc\n"s);
  std::cout << to_upper_copy("def\n"s);
}
