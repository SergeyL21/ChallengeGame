/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 11:35:24
 *
 * Project: ChallengeGame
 * File: main.cpp
 *
 * Author: Sergey Lola
 */

#include "game_engine.h"

using namespace std;

// ---------------------------------------------------------------------------
int main()
{
  GameEngine engine;
  engine.initConsole(60, 40);
  engine.start();
  return 0;
}
