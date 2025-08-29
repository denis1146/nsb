
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <forward_list>
#include <list>
#include <algorithm>

// #include <print>
// #include <stdfloat>

#include "mainConfig.h"

using namespace std;
using namespace std::chrono_literals;

void func()
{
  std::this_thread::sleep_for(500ms);
  cerr << "Test Error" << endl;
  std::this_thread::sleep_for(500ms);
  cerr << "Test Error End" << endl;
}

template <class T>
bool isForwardIt = std::is_same<typename std::iterator_traits<typename T::iterator>::iterator_category,
                                std::forward_iterator_tag>::value;

template <class T>
bool isBidirectionalIt = std::is_same_v<typename std::iterator_traits<typename T::iterator>::iterator_category,
                                        std::bidirectional_iterator_tag>;

int main(int argc, char *argv[])
{
  cout << "Project: " << NSB_PROJECT_NAME << " " << NSB_VERSION << endl;
  cout << "CMAKE_CXX_COMPILER: " << CMAKE_CXX_COMPILER << endl;
  if (argc > 1)
  {
    cout << argv[1] << endl;
  }

  // std::println("C++23");
  // std::float64_t f = 0.1f64;

  double d = 5.5;
  int i = static_cast<int>(d);
  cout << d << i << endl; // Comment

  // TODO: 555

  {
    d += d;
    jthread t{func};
    // thread t0{func};
    this_thread::sleep_for(1000ms);
    cout << "Test" << endl;
    // t0.join();
  }

  {
    using namespace std;
    vector v{1, 1, 1, 2, 3, 10, 11, 12};
    ostream_iterator<int> it{cout, "; "};
    auto first = v.begin();
    cout << "====================" << endl;
    cout << "vector: ";
    while (first != v.end()) {
      *it++ = *first++;
    }
    cout << endl;
    cout << "sst: ";
    istringstream sst("10 11 12 13 14 15");
    vector<int> v1{istream_iterator<int>(sst), istream_iterator<int>{}};
    for (auto el : v1) {
      cout << el << " *** ";
    }
    cout << endl << endl;
    cout << "isForwardIt:       " << isForwardIt<forward_list<int>> << 
      "  " << isForwardIt<list<int>> << endl;
    cout << "isBidirectionalIt: " << isBidirectionalIt<forward_list<int>> << 
      "  " << isBidirectionalIt<list<int>> << endl;
    cout << endl;

    ostream_iterator<int> out{cout, "; "};
    vector<int> v2;
    copy(v.cbegin(), v.cend(), back_inserter(v2));
    v2.erase(remove(v2.begin(), v2.end(), 1), v2.end());
    cout << "v2: ";
    copy(v2.cbegin(), v2.cend(), out);
    cout << endl;

    cout << endl;
    cout << "v2: ";
    copy(v2.cbegin(), v2.cend(), out);
    auto splitted = partition(v2.begin(), v2.end(), [](const auto& el) {return el >= 10;});
    cout << endl;
    cout << "v2: ";
    copy(v2.cbegin(), v2.cend(), out);
    cout << endl;
    cout << "splitted before: ";
    copy(begin(v2), splitted, out);
    cout << endl << "splitted after: ";
    copy(splitted, end(v2), out);




    cout << endl << "====================" << endl;
  }

  return 0;
}
