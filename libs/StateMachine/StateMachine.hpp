#pragma once

#include <tuple>
#include <type_traits>
#include <variant>

// State Machine implementation inspired by:
// https://youtu.be/S5hZdXRPBRE?si=Js9awBq802ulhEGK

/** @brief Structure for transitioning to a target state.
 *
 *  @tparam State The target state type to transition to.
 *  @return A TransitionTo object holding the TargetState type.
 */
template <class State>
struct TransitionTo {
  using TargetState = State;
};

/** @brief Main structure of the finite state machine.
 *
 *  @tparam S A tuple of state types (std::tuple<States...>).
 *  @tparam Handlers Variadic list of handler types (state transition functors).
 */
template <class S, class... Handlers>
struct StateMachine;

/** @brief Specialization of the state machine for a tuple of state types.
 *
 *  This class implements a finite state machine where each state
 *  is represented as a distinct type, and transitions are handled via
 *  overloaded call operators in the handler types.
 *
 *  @tparam States... Variadic list of concrete state types.
 *  @tparam Handlers... Variadic list of handler types that define state transitions.
 *
 *  Inherits from all Handlers types to gain access to their operator() overloads.
 */
template <class... States, class... Handlers>
struct StateMachine<std::tuple<States...>, Handlers...> : Handlers... {
  using Handlers::operator()...;

  /** @brief Constructor of the state machine.
   *
   *  @param h Variadic list of handler objects (rvalue references) to be moved
   *  into internal storage. Each handler must be callable with an event argument
   *  and return either TransitionTo<NewState> or void to indicate a state change.
   */
  StateMachine(Handlers&&... h);

  /** @brief Processes an event and performs state transitions.
   *
   *  Dispatches the event to the appropriate overloaded operator() based on the
   *  current active state. If a handler returns TransitionTo<NewState>, the state
   *  machine transitions to that new state.
   *
   *  @param event The event object to process.
   */
  template <class Event>
  void onEvent(Event&& event);

  /** @brief Returns the current state of the machine.
   *
   *  @return Const reference to variant containing pointer to the current state.
   */
  [[nodiscard]] const std::variant<States*...>& state() const&;

private:
  std::tuple<States...> m_states;
  std::variant<States*...> m_state;
};

/** @brief Factory function for creating a state machine.
 *
 *  Deduces the state tuple from the state types and constructs a StateMachine
 *  instance with the provided handlers.
 *
 *  @tparam S Tuple of state types (usually std::tuple<States...>).
 *  @tparam Handlers... Variadic list of handler types.
 *  @param h Variadic list of handler objects to forward into the state machine.
 *  @return A fully constructed StateMachine object.
 */
template <class S, class... Handlers>
auto makeStateMachine(Handlers&&... h);

#include "StateMachine_impl.hpp"
