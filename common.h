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

enum class Color : COLORREF
{
  Black       = RGB(0, 0, 0),
  DarkRed     = RGB(142, 0, 0),
  DarkBlue    = RGB(60, 130, 190),
  White       = RGB(255, 255, 255)
};
