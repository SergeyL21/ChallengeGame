/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 11:41:03
 *
 * Project: ChallengeGame
 * File: game_object.h
 *
 * Author: Sergey Lola
 */

#pragma once

#include "common.h"

class Player;
class EvilEntity;
class Wall;

class GameObject {
public:
  enum Type
  {
    Player,             // игрок
    EvilEntity,         // противник
    Wall                // стена игрового поля
  };

  explicit GameObject(Type _type);
  virtual ~GameObject() = default;

  virtual const class Wall *toWall() const;
  virtual class Wall *toWall();
  virtual const class Player *toPlayer() const;
  virtual class Player *toPlayer();
  virtual const class EvilEntity *toEvilEntity() const;
  virtual class EvilEntity *toEvilEntity();

  Type type() const;
  COORD pos() const;
  void setPos(const COORD &pos);
  COORD size() const;
  void setSize(const COORD &size);
  Color color() const;
  void setColor(Color color);
  int speed() const;
  void setSpeed(int value);
  bool isStatic() const;

protected:
  COORD m_pos {0u, 0u};         // текущая позиция объекта (от условного центра)
  COORD m_size {0u, 0u};        // текущие размеры объекта
  Color m_color {Color::White}; // текущий цвет объекта
  int m_speed {0u};             // текущая скорость объекта

private:
  const Type m_type;            // тип объекта
  bool m_isStatic {true};       // является ли объект статическим
};
