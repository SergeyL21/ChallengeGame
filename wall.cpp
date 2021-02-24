/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 12:53:39
 *
 * Project: ChallengeGame
 * File: wall.cpp
 *
 * Author: Sergey Lola
 */

#include "wall.h"

// ---------------------------------------------------------------------------
Wall::Wall() : GameObject(GameObject::Wall) { }

// ---------------------------------------------------------------------------
const Wall *Wall::toWall() const { return this; }

// ---------------------------------------------------------------------------
Wall *Wall::toWall() { return this; }
