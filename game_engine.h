/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 12:26:45
 *
 * Project: ChallengeGame
 * File: game_engine.h
 *
 * Author: Sergey Lola
 */

#pragma once

#include <vector>

#include "common.h"

class GameObject;

class GameEngine {
public:
  void initConsole(const COORD &size);
  void start();
  void endGame();

private:
  void showConsoleCursor(bool flag);
  void drawPixel(const COORD &pos, Color color = Color::White);
  void drawSymbol(const COORD &pos, char symbol, Color color = Color::White);
  void drawRect(const COORD &pos, const COORD &size, Color color = Color::White);
  void drawText(const COORD &pos, const char *text, Color color = Color::White);
  bool updatePlayerCollision();

private:
  unsigned score{0u};
  GameObject *m_player;
  std::vector<GameObject *> m_objects;
};
