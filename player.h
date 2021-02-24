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

// ---------------------------------------------------------------------------
// класс описания игрового объекта типа игрок
class Player : public GameObject {
public:
  Player();
  virtual ~Player() = default;

  // переопределение методов базового класса
  virtual const Player *toPlayer() const override;
  virtual Player *toPlayer() override;

  // метод получения текущей скорости смещения игрока
  inline float driftSpeed() const { return m_driftSpeed; }
  // метод изменения скорости смещения игрока
  inline void setDriftSpeed(float value) { m_driftSpeed = value; }
  // метод определения направления смещения игрока по заданной границе
  // (условная середина окна консоли)
  SHORT driftDirection(SHORT border) const;

protected:
  float m_driftSpeed {0};     // текущая скорость смещения игрока влево/вправо
};
