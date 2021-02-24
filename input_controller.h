/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 23:58:01
 *
 * Project: ChallengeGame
 * File: input_controller.h
 *
 * Author: Sergey Lola
 */

#pragma once

#include "common.h"

#include <map>

// ---------------------------------------------------------------------------
// базовый класс описания игрового объекта
class InputController
{
public:
  InputController() = default;
  ~InputController() = default;

  // метод инициализации ресурсов контроллера
  bool initInputController();
  // метод закрытия ресурсов контроллера
  void closeInputController();

  // метод получения данных о текущих состояниях клавиш
  KeyState keyState(KeyCode key, KeyState *previous = nullptr) const;
  // метод обновления состояний клавиш
  void updateInputEvents();

  // метод определения нажаты ли клавиши отвечающие за левое перемещение игрока
  inline bool hasKeyLeftMovement() const { return m_hasKeyLeftMovement; }
  // метод определения нажаты ли клавиши отвечающие за правое перемещение игрока
  inline bool hasKeyRightMovement() const { return m_hasKeyRightMovement; }
  // метод определения нажата ли клавиша "SPACE"
  inline bool hasKeySpace() const { return m_hasKeySpace; }
  // метод определения нажата ли клавиша "ESC"
  inline bool hasKeyEscape() const { return m_hasKeyEscape; }

private:
  // указатель на вводимые данные (нажатие клавиш) с консоли
  HANDLE m_hConsoleIn {INVALID_HANDLE_VALUE};
  // карта состояний клавиш
  std::map<KeyCode, std::array<KeyState, 2>> m_keysState;

  bool m_hasKeyLeftMovement{false};
  bool m_hasKeyRightMovement{false};
  bool m_hasKeySpace{false};
  bool m_hasKeyEscape{false};
};
