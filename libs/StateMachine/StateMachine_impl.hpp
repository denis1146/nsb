template <class... States, class... Handlers>
StateMachine<std::tuple<States...>, Handlers...>::StateMachine(Handlers&&... h)
: Handlers(h)...
, m_states{}
, m_state(&std::get<0>(m_states))
{
}

template <class... States, class... Handlers>
template <class Event>
void StateMachine<std::tuple<States...>, Handlers...>::onEvent(Event&& event)
{
  std::visit(
      [this, &event](auto statePtr) {
        if constexpr (std::is_invocable_v<StateMachine, decltype(*statePtr), Event&&>) {
          using ResultType = std::invoke_result_t<StateMachine, decltype(*statePtr), Event&&>;
          if constexpr (std::is_same_v<ResultType, void>) {  // No transition
            (*this)(*statePtr, std::forward<Event>(event));
          } else {  // Transitioned to ...
            auto transitionTo = (*this)(*statePtr, std::forward<Event>(event));
            m_state = &std::get<typename ResultType::TargetState>(m_states);
          }
        } else {  // No rules invoked
        }
      },
      m_state);
}

template <class... States, class... Handlers>
const std::variant<States*...>& StateMachine<std::tuple<States...>, Handlers...>::state() const&
{
  return m_state;
}

template <class S, class... Handlers>
auto makeStateMachine(Handlers&&... h)
{
  return StateMachine<S, std::decay_t<Handlers>...>{ std::forward<Handlers>(h)... };
}
