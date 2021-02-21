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

class Wall : public GameObject {
public:
  Wall();
  virtual ~Wall() = default;

  char graphSymbol() const;
  void setGraphSymbol(char symbol);

  virtual const class Wall *toWall() const override;
  virtual class Wall *toWall() override;

protected:
  char m_graphSymbol {0x0};      // символ, из которого отрисовывается стена
};
