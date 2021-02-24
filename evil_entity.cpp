/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 18:42:04
 *
 * Project: ChallengeGame
 * File: evil_entity.cpp
 *
 * Author: Sergey Lola
 */

#include "evil_entity.h"

// ---------------------------------------------------------------------------
EvilEntity::EvilEntity() : GameObject(GameObject::EvilEntity) {}

// ---------------------------------------------------------------------------
const EvilEntity *EvilEntity::toEvilEntity() const { return this; }

// ---------------------------------------------------------------------------
EvilEntity *EvilEntity::toEvilEntity() { return this; }

