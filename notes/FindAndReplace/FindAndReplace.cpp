#include "FindAndReplace.h"

#include <regex>
#include <string>
#include <iostream>
#include <iterator>
#include <boost/scope_exit.hpp>
#include "NoteHelpers.h"

void FindAndReplace::run()
{
  constexpr char noteTitle[] = "FindAndReplace";
  nsb::showNoteTitle(noteTitle);
  return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  std::cout << "Available regex syntaxes:\n"
    << "\t[0] ECMAScript\n"
    << "\t[1] ECMAScript case insensitive\n"
    << "\t[2] POSIX basic\n"
    << "\t[3] POSIX basic case insensitive\n"
    << "\t[4] POSIX extended\n"
    << "\t[5] POSIX extended case insensitive\n"\
    << "\t[6] awk (POSIX)\n"
    << "\t[7] awk (POSIX) case insensitive\n"
    << "\t[8] grep\n"
    << "\t[9] grep case insensitive\n"
    << "\t[a] egrep\n"
    << "\t[b] egrep case insensitive\n";

  std::regex::flag_type flag{};
  switch (std::cin.get()) {
    case '0':
      flag  = std::regex::ECMAScript;
      break;
    case '1':
      flag  = std::regex::ECMAScript | std::regex::icase;
      break;
    case '2':
      flag  = std::regex::basic;
      break;
    case '3':
      flag  = std::regex::basic | std::regex::icase;
      break;
    case '4':
      flag  = std::regex::extended;
      break;
    case '5':
      flag  = std::regex::extended | std::regex::icase;
      break;
    case '6':
      flag  = std::regex::awk;
      break;
    case '7':
      flag  = std::regex::awk | std::regex::icase;
      break;
    case '8':
      flag  = std::regex::grep;
      break;
    case '9':
      flag  = std::regex::grep | std::regex::icase;
      break;
    case 'a':
      flag  = std::regex::egrep;
      break;
    case 'b':
      flag  = std::regex::egrep | std::regex::icase;
      break;
    default:
      std::cout << "Incorrect number of regex syntax."
                   " Exiting... \n";
      return;
  }
  std::cout << std::endl;
  std::cin.clear();
  std::cin.ignore();

  std::string st;
  do
  {
    std::cout << "Input a string to match:\n";
    if (!std::getline(std::cin, st)) {
      std::cout << "Exiting... \n";
      return;
    }
    if (st.empty()) {
      continue;
    }

    auto inputRegexShow = []{ std::cout << "Input regex: \n"; };
    inputRegexShow();
    for (std::string regex; std::getline(std::cin, regex);) {
      if (regex.empty()) {
        std::cerr << "The regular expression string is empty!\n";
        inputRegexShow();
        continue;
      }
      
      try {
        std::regex e{regex, flag};
        if (std::smatch res; std::regex_search(st, res, e)) {
          std::cout << "The entire sequence found: " << res[0] << std::endl;
          std::cout << "Match: ";
          std::copy(res.begin() + 1, res.end(), std::ostream_iterator<std::string>(std::cout, ", "));
          std::cout << std::endl;

          auto inputReplaceStShow = [] { std::cout << "Enter the pattern to replace:\n"; };
          inputReplaceStShow();
          for (std::string replaceSt; std::getline(std::cin, replaceSt);) {
            if (replaceSt.empty()) {
              std::cerr << "The replacement template is empty!\n";
              inputReplaceStShow();
              continue;
            }
            std::cout << "Result:\n" <<
              // std::regex_replace(st, e, replaceSt) << std::endl;
              res.format(replaceSt) << std::endl;
            inputReplaceStShow();
          }
        }
        else {
          std::cout << "No matches found.\n";
        }
      }
      catch(const std::regex_error& e) {
        std::cerr << "Incorrect regex pattern!\n" << 
          "regex_error: " << e.what() << 
          " (code: " << e.code() << ")\n";
      }

      inputRegexShow();
    }
    std::cin.clear();
    std::cin.ignore();
  } while (1);
}
