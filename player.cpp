/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 12:56:22
 *
 * Project: ChallengeGame
 * File: player.cpp
 *
 * Author: Sergey Lola
 */

#include "player.h"

// ---------------------------------------------------------------------------
Player::Player() : GameObject(GameObject::Player) {}

// ---------------------------------------------------------------------------
const Player *Player::toPlayer() const { return this; }

// ---------------------------------------------------------------------------
Player *Player::toPlayer() { return this; }

// ---------------------------------------------------------------------------
SHORT Player::driftDirection(SHORT border) const
{
  const SHORT centerX = (m_size.X == 1) ? m_pos.X : m_pos.X + (m_size.X >> 1) - 1;
  return ((centerX > border) ? 1 : ((centerX < border ) ? -1 : 0));
}

