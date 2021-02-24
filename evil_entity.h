/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 12:18:33
 *
 * Project: ChallengeGame
 * File: evil_entity.h
 *
 * Author: Sergey Lola
 */

#pragma once

#include "game_object.h"

// ---------------------------------------------------------------------------
// класс описания игрового объекта типа противник
class EvilEntity : public GameObject {
public:
  EvilEntity();
  virtual ~EvilEntity() = default;

  // переопределение методов базового класса
  virtual const EvilEntity *toEvilEntity() const override;
  virtual EvilEntity *toEvilEntity() override;

private:

};
