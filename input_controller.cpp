/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 00:03:13
 *
 * Project: ChallengeGame
 * File: input_controller.cpp
 *
 * Author: Sergey Lola
 */

#include "input_controller.h"

using namespace std;

// ---------------------------------------------------------------------------
bool InputController::initInputController()
{
  m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
  if (INVALID_HANDLE_VALUE == m_hConsoleIn)
  {
    return false;
  }

  // инициализация карты клавиш
  const array<KeyState, 2> default_states {KeyState::Released, KeyState::Released};
  m_keysState = {
    make_pair(KeyCode::LeftArrow, default_states),
    make_pair(KeyCode::RightArrow, default_states),
    make_pair(KeyCode::A, default_states),
    make_pair(KeyCode::D, default_states),
    make_pair(KeyCode::Space, default_states),
    make_pair(KeyCode::Esc, default_states)
  };

  return true;
}

// ---------------------------------------------------------------------------
void InputController::closeInputController()
{
  if (INVALID_HANDLE_VALUE != m_hConsoleIn)
  {
    FlushConsoleInputBuffer(m_hConsoleIn);
  }
}

// ---------------------------------------------------------------------------
KeyState InputController::keyState(KeyCode key, KeyState *previous) const
{
  const auto &value = m_keysState.at(key);
  if (nullptr != previous) {
    *previous = value.front();
  }
  return value.back();
}

// ---------------------------------------------------------------------------
void InputController::updateInputEvents()
{
  for (auto it = m_keysState.begin(); it != m_keysState.end(); ++it)
  {
    auto &oldState = it->second.front();
    auto &newState = it->second.back();
    auto value = GetAsyncKeyState(static_cast<int>(it->first));
    if (value & 0x8000)
    {
      if (KeyState::Pressed == oldState)
      {
        oldState = newState;
        newState = KeyState::Held;
      }
      else if (KeyState::Released == oldState)
      {
        oldState = newState;
        newState = KeyState::Pressed;
      }
    }
    else
    {
      oldState = newState;
      newState = KeyState::Released;
    }

    switch (it->first)
    {
      case KeyCode::A:
      case KeyCode::LeftArrow:
        m_hasKeyLeftMovement = (keyState(KeyCode::A) != KeyState::Released) ||
                               (keyState(KeyCode::LeftArrow) != KeyState::Released);
        break;
      case KeyCode::D:
      case KeyCode::RightArrow:
        m_hasKeyRightMovement = (keyState(KeyCode::D) != KeyState::Released) ||
                                (keyState(KeyCode::RightArrow) != KeyState::Released);
        break;
      case KeyCode::Space:
        m_hasKeySpace = (newState != KeyState::Released);
        break;
      case KeyCode::Esc:
        m_hasKeyEscape = (newState != KeyState::Released);
        break;
      default:
        break;
    }
  }
}
