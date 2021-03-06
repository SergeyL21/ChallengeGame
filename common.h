/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 11:43:39
 *
 * Project: ChallengeGame
 * File: common.h
 *
 * Author: Sergey Lola
 */

#pragma once

#include <windows.h>

// ---------------------------------------------------------------------------
// вспомогательный макрос для обозначения передачи аргумента по ссылке
#undef OUT
#define OUT

// ---------------------------------------------------------------------------
// перечисление цвета символа
enum class Color : WORD
{
  Black       = 0x0000,
  DarkBlue    = 0x0001,
  DarkRed     = 0x0004,
  Gray        = 0x0007,
  White       = 0x000F
};

// ---------------------------------------------------------------------------
// перечисление возможных активных клавиш
enum class KeyCode
{
  LeftArrow   = 0x25,
  RightArrow  = 0x27,
  A           = 0x41,
  D           = 0x44,
  Esc         = 0x1B,
  Space       = 0x20,
  _ENUM_SIZE_ = 6
};

// ---------------------------------------------------------------------------
// перечисление состояний клавиш
enum class KeyState
{
  Pressed,    // одиночное нажатие
  Released,   // не нажата
  Held        // зажата постоянно
};
