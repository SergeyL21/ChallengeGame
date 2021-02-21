/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 12:14:45
 *
 * Project: ChallengeGame
 * File: player.h
 *
 * Author: Sergey Lola
 */

#pragma once

#include "game_object.h"

class Player : public GameObject {
public:
  Player();
  virtual ~Player() = default;

  virtual const class Player *toPlayer() const override;
  virtual class Player *toPlayer() override;

  int driftSpeed() const;
  void setDriftSpeed(int value);

protected:
  int m_driftSpeed;      // текущая скорость смещения игрока влево/вправо
};
