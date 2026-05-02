#include "StateMachine/StateMachine.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace {

// Events
struct Press {};
struct Release {};
struct Click {};
struct Hover {};

// States
struct StatePressed {};
struct StateReleased {
  bool isHovered = false;
};

}  // namespace

TEST(StateMachineTest, StateMachine)
{
  std::size_t clickCounter = 0;
  std::function<void()> click;
  auto sm = makeStateMachine<std::tuple<StateReleased, StatePressed>>(
      [](StatePressed&, Release) -> TransitionTo<StateReleased> { return {}; },
      [](StatePressed&, Hover) { /* button is pressed */ },

      [](StateReleased&, Press) -> TransitionTo<StatePressed> { return {}; },
      [](StateReleased& s, Hover) { s.isHovered = true; },
      [&click](StateReleased&, Click) { click(); });

  click = [&sm, &clickCounter]() {
    sm.onEvent(Press{});
    sm.onEvent(Release{});
    ++clickCounter;
  };

  ASSERT_NE(std::get_if<StateReleased*>(&sm.state()), nullptr);

  sm.onEvent(Press{});
  ASSERT_NE(std::get_if<StatePressed*>(&sm.state()), nullptr);
  sm.onEvent(Press{});
  ASSERT_NE(std::get_if<StatePressed*>(&sm.state()), nullptr);
  sm.onEvent(Hover{});
  ASSERT_NE(std::get_if<StatePressed*>(&sm.state()), nullptr);

  sm.onEvent(Click{});
  ASSERT_EQ(clickCounter, 0);
  sm.onEvent(Click{});
  ASSERT_EQ(clickCounter, 0);

  sm.onEvent(Release{});
  ASSERT_NE(std::get_if<StateReleased*>(&sm.state()), nullptr);
  sm.onEvent(Release{});
  ASSERT_NE(std::get_if<StateReleased*>(&sm.state()), nullptr);
  ASSERT_FALSE((*std::get_if<StateReleased*>(&sm.state()))->isHovered);
  sm.onEvent(Hover{});
  ASSERT_NE(std::get_if<StateReleased*>(&sm.state()), nullptr);
  ASSERT_TRUE((*std::get_if<StateReleased*>(&sm.state()))->isHovered);

  sm.onEvent(Click{});
  ASSERT_EQ(clickCounter, 1);
  sm.onEvent(Click{});
  ASSERT_EQ(clickCounter, 2);
  sm.onEvent(Click{});
  ASSERT_EQ(clickCounter, 3);
}
