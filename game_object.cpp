/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 12:45:08
 *
 * Project: ChallengeGame
 * File: game_object.cpp
 *
 * Author: Sergey Lola
 */

#include "game_object.h"

#include <cmath>

// ---------------------------------------------------------------------------
GameObject::GameObject(GameObject::Type type) : m_type(type) {}

// ---------------------------------------------------------------------------
void GameObject::setPos(const COORD &pos)
{
  m_pos = pos;
  m_rx = pos.X;
  m_ry = pos.Y;
  updateRect();
}

// ---------------------------------------------------------------------------
void GameObject::setSize(const COORD &size)
{
  m_size = size;
  updateRect();
}

// ---------------------------------------------------------------------------
SHORT GameObject::stepPos0X(float speed, float dt)
{
  m_rx += dt * speed;
  return lroundf(m_rx);
}

// ---------------------------------------------------------------------------
SHORT GameObject::stepPos0Y(float speed, float dt)
{
  m_ry += dt * speed;
  return lroundf(m_ry);
}

// ---------------------------------------------------------------------------
COORD GameObject::stepPosXY(float speedX, float speedY, float delta)
{
  return {stepPos0X(speedX, delta), stepPos0Y(speedY, delta)};
}

// ---------------------------------------------------------------------------
void GameObject::updateRect()
{
  m_rect.Left = m_pos.X;
  m_rect.Top = m_pos.Y;
  m_rect.Right = m_pos.X + m_size.X - 1;
  m_rect.Bottom = m_pos.Y + m_size.Y - 1;

  if (m_rect.Right < m_rect.Left) {
    m_rect.Right = m_rect.Left = 0;
  }

  if (m_rect.Bottom < m_rect.Top) {
    m_rect.Bottom = m_rect.Top = 0;
  }
}
