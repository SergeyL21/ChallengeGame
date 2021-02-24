/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 12:06:13
 *
 * Project: ChallengeGame
 * File: wall.h
 *
 * Author: Sergey Lola
 */

#pragma once

#include "game_object.h"

// ---------------------------------------------------------------------------
// класс описания игрового объекта типа стена
class Wall : public GameObject {
public:
  Wall();
  virtual ~Wall() = default;

  // переопределение методов базового класса
  virtual const Wall *toWall() const override;
  virtual Wall *toWall() override;

private:
};
