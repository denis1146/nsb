#include "RangesTest.h"

#include <ranges>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

void RangesTest::run()
{
  auto show = [] { std::cout << std::string(19, '=') + " RangesTest " + std::string(19, '=') << std::endl; };
  show();

  std::vector<int> v{ 1,2,3,4,5,6,7,8,9 };
  auto numbers = v | std::views::filter([](int n) { return n % 2 == 0; })
              | std::views::transform([](int n) { return n * n; });
  // v = {10, 11};
  std::cout << "ranges: ";
  std::ranges::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;


  auto view = std::views::iota(1, 10) |
              std::views::drop(5) |
              std::views::take(3);
  std::cout << "view: ";
  std::ranges::copy(view.begin(), view.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;


  std::cout << "revert: ";
  for (auto&& n : std::views::iota(1, 10) | std::views::reverse) {
    std::cout << n << " ";
  }
  std::cout << std::endl;


  std::cout << "view2: ";
  std::vector view2{ 5,4,3,2,1 };
  // v = std::move(v) | std::ranges::action::sort | std::ranges::action::reverse;
  std::ranges::sort(view2);
  std::ranges::copy(view2.begin(), view2.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  show();
}