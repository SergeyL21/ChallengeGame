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

// ---------------------------------------------------------------------------
// базовый класс описания игрового объекта
class GameObject {
public:
  enum Type
  {
    Player,             // игрок
    EvilEntity,         // противник
    Wall                // стена игрового поля
  };

  explicit GameObject(Type type);
  virtual ~GameObject() = default;

  // преобразование игрового объекта в объект типа "Стена"
  inline virtual const class Wall *toWall() const { return nullptr; }
  inline virtual class Wall *toWall() { return nullptr; }
  // преобразование игрового объекта в объект типа "Игрок"
  inline virtual const class Player *toPlayer() const { return nullptr; }
  inline virtual class Player *toPlayer() { return nullptr; }
  // преобразование игрового объекта в объект типа "Противник"
  inline virtual const class EvilEntity *toEvilEntity() const { return nullptr; }
  inline virtual class EvilEntity *toEvilEntity() { return nullptr; }

  // получение данных о типе игрового объекта
  inline Type type() const { return m_type; }
  // получение данных о позиции игрового объекта
  inline COORD pos() const { return m_pos; }
  // получение данных о прямоугольнике игрового объекта
  inline SMALL_RECT rect() const { return m_rect; }
  // изменение позиции игрового объекта
  void setPos(const COORD &pos);
  // получение данных о размерах игрового объекта
  inline COORD size() const { return m_size; }
  // изменение размеров игрового объекта
  void setSize(const COORD &size);
  // получение данных о цвете игрового объекта
  inline Color color() const { return m_color; }
  // изменение цвета игрового объекта
  inline void setColor(Color color) { m_color = color; }
  // получение данных о горизонтальной скорости игрового объекта
  inline float speed0X() const { return m_speed0X; }
  // изменение горизонтальной скорости игрового объекта
  inline void setSpeed0X(float value) { m_speed0X = value; }
  // получение данных о вертикальной скорости игрового объекта
  inline float speed0Y() const { return m_speed0Y; }
  // изменение вертикальной скорости игрового объекта
  inline void setSpeed0Y(float value) { m_speed0Y = value; }
  // получение данных о статичности игрового объекта
  inline bool isStatic() const { return m_isStatic; }

  // подсчет реальной координаты Х в зависимости от скорости объекта
  SHORT stepPos0X(float speed, float dt);
  // подсчет реальной координаты Y в зависимости от скорости объекта
  SHORT stepPos0Y(float speed, float dt);
  // подсчет реальных координат Х и Y в зависимости от скорости объекта
  COORD stepPosXY(float speedX, float speedY, float dt);

protected:
  COORD m_pos {0, 0};             // текущая позиция объекта (от левого верхнего угла)
  COORD m_size {0, 0};            // текущие размеры объекта
  Color m_color {Color::White};   // текущий цвет объекта

  float m_speed0X {0.f};          // текущая скорость объекта по горизонтали (клетки / секунда)
  float m_speed0Y {0.f};          // текущая скорость объекта по вертикали (клетки / секунда)

private:
  const Type m_type;              // тип объекта
  bool m_isStatic {true};         // является ли объект статическим

  SMALL_RECT m_rect {0, 0, 0, 0}; // текущие координаты прямоугольника объекта
  float m_rx {0.f}, m_ry {0.f};   // реальная позиция объекта с учетом его скорости

private:
  // обновление прямоугольника объекта
  void updateRect();
};
