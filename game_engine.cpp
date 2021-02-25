/*
 * Created by Qt Creator 4.11.0 @ 21 Feb 2021 18:46:58
 *
 * Project: ChallengeGame
 * File: game_object.cpp
 *
 * Author: Sergey Lola
 */

#include <iostream>
#include <random>
#include <math.h>
#include <ctime>

#include "game_engine.h"
#include "evil_entity.h"
#include "wall.h"
#include "utilities.h"

using namespace std;

// ---------------------------------------------------------------------------
// ширина границ игрового поля
const char FIELD_BORDER_WIDTH = 1;
// максимально возможное число игровых объектов в кадре
const SHORT MAX_SCENE_OBJECTS = 100;
// масштаб времени для вычисления скоростей движения объектов (мс)
const DWORD TIME_SCALE { 1000u };

const std::string SCORE_TEXT {"Score: "};
// расположение надписи "Score"
const COORD SCORE_INFO_POS {0, 0};
// расположения поля с выводом текущих игровых очков игрока
const COORD SCORE_TEXT_POS {7, 0};

// размеры объекта игрока
const COORD PLAYER_BASE_SIZE {4, 2};
const COORD PLAYER_BASE_HALF_SIZE { SHORT(PLAYER_BASE_SIZE.X / 2),
                                    SHORT(PLAYER_BASE_SIZE.Y / 2) };
// горизонтальная скорость игрока (символ/сек)
const float PLAYER_SPEED_0X {20.f};
// горизонтальная скорость смещения игрока (символ/сек)
const float PLAYER_DRIFT_SPEED {7.5f};

// размеры объекта противника
const COORD EVIL_ENTITY_BASE_SIZE {4, 2};
// вертикальная скорость движения противника (символ/сек)
const float EVIL_ENTITY_SPEED_0Y {5.f};
// задержка времени между появлением нового противника (мс)
const float EVIL_ENTITY_SPAWN_DELAY {333.f};
// удвоение появления противников каждые N задержек
const int EVIL_ENTITY_DOUBLE_SPLASH {10};

// символ горизонтальной границы игрового поля
const BYTE BC_HORIZONTAL_LINE = 0xCD;
// символ вертикальной границы игрового поля
const BYTE BC_VERTICAL_LINE = 0xBA;
// символ основания игрового поля
const BYTE BC_BASE = 0xB1;
// символ левой верхней границы игрового поля
const BYTE BC_TOP_LEFT = 0xC9;
// символ правой верхней границы игрового поля
const BYTE BC_TOP_RIGHT = 0xBB;
// символ игровых объектов
const BYTE GH_BLOCK = 0xDB;

// ---------------------------------------------------------------------------
bool GameEngine::initConsole(SHORT width, SHORT height)
{
  m_initializationFlag = false;
  m_objects.reserve(MAX_SCENE_OBJECTS);

  m_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (INVALID_HANDLE_VALUE == m_hConsoleOut)
  {
    setLastErrorText("bad 'm_hConsoleOut' handle");
    return false;
  }

  if (!m_inputController.initInputController())
  {
    setLastErrorText("input controller initialization failed");
    return false;
  }

  m_fieldWidth = width;
  m_fieldHeight = height;
  m_rectWindow = {0, 0, m_fieldWidth, m_fieldHeight};
  if (!SetConsoleWindowInfo(m_hConsoleOut, TRUE, &m_rectWindow))
  {
    setLastErrorText("'SetConsoleWindowInfo' return false");
    return false;
  }

  if (!SetConsoleActiveScreenBuffer(m_hConsoleOut))
  {
    setLastErrorText("'SetConsoleActiveScreenBuffer' return false");
    return false;
  }

  m_bufScreen = unique_ptr<CHAR_INFO[]>{ new CHAR_INFO[width * height], default_delete<CHAR_INFO[]>() };
  memset(m_bufScreen.get(), 0, sizeof(CHAR_INFO) * m_fieldWidth * m_fieldHeight);

  showConsoleCursor(false);

  // отрисовываем текст с текущем счетом игры
  drawString(SCORE_INFO_POS, SCORE_TEXT);
  drawString(SCORE_TEXT_POS, std::string{"00000"});

  // отрисовываем границы игры
  const COORD top_left {0, 1};
  const COORD top_right {SHORT(m_fieldWidth - FIELD_BORDER_WIDTH), 1};
  const COORD bottom_left {0, SHORT(m_fieldHeight - FIELD_BORDER_WIDTH)};
  const COORD bottom_right {SHORT(m_fieldWidth - FIELD_BORDER_WIDTH),
                            SHORT(m_fieldHeight - FIELD_BORDER_WIDTH)};

  // верхняя граница
  drawSymbol(top_left, BC_TOP_LEFT, Color::DarkRed);
  fillSymbol({FIELD_BORDER_WIDTH, 1},
             top_right,
             BC_HORIZONTAL_LINE,
             Color::DarkRed);
  // правая граница
  drawSymbol(top_right, BC_TOP_RIGHT, Color::DarkRed);
  fillSymbol({top_right.X, SHORT(top_right.Y + FIELD_BORDER_WIDTH)},
             bottom_right,
             BC_VERTICAL_LINE,
             Color::DarkRed);
  // нижняя граница
  fillSymbol(bottom_left,
             bottom_right,
             BC_BASE,
             Color::Gray);
  // левая граница
  fillSymbol({top_left.X, SHORT(top_left.Y + FIELD_BORDER_WIDTH)},
             {bottom_left.X, SHORT(bottom_left.Y - FIELD_BORDER_WIDTH)},
             BC_VERTICAL_LINE,
             Color::DarkRed);

  // заполняем данные об объекте игрока
  m_player = make_unique<Player>();
  m_player->setColor(Color::DarkBlue);
  // выставляем игрока на условный центр
  m_player->setPos({SHORT(m_fieldWidth / 2 - PLAYER_BASE_HALF_SIZE.X),
                    SHORT(bottom_left.Y - FIELD_BORDER_WIDTH - PLAYER_BASE_HALF_SIZE.Y)});
  m_player->setSize(PLAYER_BASE_SIZE);
  m_player->setSpeed0X(PLAYER_SPEED_0X);
  m_player->setDriftSpeed(PLAYER_DRIFT_SPEED);
  drawGameObject(m_player.get());

  // заполняем данные об статических препятствиях на пути игрока (левых и правых стенах)
  auto leftWall = new Wall();
  auto rightWall = new Wall();
  leftWall->setColor(Color::DarkRed);
  leftWall->setPos({FIELD_BORDER_WIDTH, m_player->pos().Y});
  leftWall->setSize({1, 1});
  rightWall->setColor(Color::DarkRed);
  rightWall->setPos({SHORT(m_fieldWidth - FIELD_BORDER_WIDTH - 1), m_player->pos().Y});
  rightWall->setSize({1, 1});
  m_objects.push_back(std::unique_ptr<GameObject>(leftWall));
  m_objects.push_back(std::unique_ptr<GameObject>(rightWall));

  // устанавливаем зону основания (по достижению которой происходит удаление врагов)
  m_baseArea = { bottom_left.X,
                 bottom_left.Y,
                 bottom_right.X,
                 bottom_right.Y};

  // устанавливаем зону появления врагов
  m_spawnArea = { FIELD_BORDER_WIDTH,
                  FIELD_BORDER_WIDTH + 1,
                  SHORT(m_fieldWidth - FIELD_BORDER_WIDTH - 1),
                  SHORT(m_fieldHeight - FIELD_BORDER_WIDTH - 1)};

  updateView();
  m_initializationFlag = true;
  return m_initializationFlag;
}

// ---------------------------------------------------------------------------
void GameEngine::start()
{
  if (false == m_initializationFlag)
  {
    return;
  }

  startMenu();

  const SHORT fieldCenterBorder = SHORT(lroundf(float(m_fieldWidth) / 2 - 1));
  COORD nextPlayerPos { m_player->pos() };
  DWORD tickPoint1{GetTickCount()}, tickPoint2{GetTickCount()};
  float deltaTime;
  float spawnDelay {0.f};
  SHORT playerDriftDirection {0};
  bool avoidPlayerDrift;
  int startSeed {int(time(0))};
  do {
    avoidPlayerDrift = false;
    tickPoint1 = tickPoint2;
    tickPoint2 = GetTickCount();
    spawnDelay += tickPoint2 - tickPoint1;
    deltaTime = float(tickPoint2 - tickPoint1) / TIME_SCALE;

    m_inputController.updateInputEvents();
    if (m_inputController.hasKeyLeftMovement())
    {
      nextPlayerPos.X = m_player->stepPos0X(-(m_player->speed0X()), deltaTime);
      moveGameObject(m_player.get(), nextPlayerPos);
      avoidPlayerDrift = true;
    }
    if (m_inputController.hasKeyRightMovement())
    {
      nextPlayerPos.X = m_player->stepPos0X(m_player->speed0X(), deltaTime);
      moveGameObject(m_player.get(), nextPlayerPos);
      avoidPlayerDrift = true;
    }

    // вычисляем размер дрейфа игрока
    if (!avoidPlayerDrift)
    {
      playerDriftDirection = m_player->driftDirection(fieldCenterBorder);
      nextPlayerPos.X = m_player->stepPos0X(
        m_player->driftSpeed() * playerDriftDirection, deltaTime);
      moveGameObject(m_player.get(), nextPlayerPos);
    }

    // генерируем нового противника
    if (spawnDelay > EVIL_ENTITY_SPAWN_DELAY)
    {
      if (!(m_score % EVIL_ENTITY_DOUBLE_SPLASH))
      {
        spawnEvilEntity(startSeed++);
      }
      spawnEvilEntity(startSeed++);
      spawnDelay = 0.f;
      drawString(SCORE_TEXT_POS,
                 utilities::string_format(std::string{"%05d"}, ++m_score));
    }

    updateEvilEntities(deltaTime);

    // если зафиксировали столкновение с игровыми объектами, то игра
    // заканчивается
    if (updatePlayerCollision())
    {
      m_player->setColor(Color::DarkRed);
      drawGameObject(m_player.get());
      m_gameOver = true;
    }
    updateView();
    Sleep(1);
  } while (!m_gameOver);

  endGame();
}

// ---------------------------------------------------------------------------
void GameEngine::startMenu()
{
  const std::string START_GAME_TEXT {"Press 'SPACE' to start the game..."};

  COORD textPosition {
    SHORT(m_fieldWidth / 2 - START_GAME_TEXT.size() / 2),
    SHORT(m_fieldHeight / 2)};

  drawString(textPosition, START_GAME_TEXT);
  updateView();

  do
  {
    m_inputController.updateInputEvents();
    if (m_inputController.hasKeySpace())
    {
      break;
    }
    Sleep(50);
  } while (true);

  fillSymbol(textPosition,
            {SHORT(textPosition.X + START_GAME_TEXT.size()), textPosition.Y},
             0);
  updateView();
}

// ---------------------------------------------------------------------------
void GameEngine::endGame()
{
  const std::string END_GAME_TEXT_STR1 {
    utilities::string_format(string{"Game over! Your score is %d."}, m_score)};
  const std::string END_GAME_TEXT_STR2 {"Press 'ESC' to exit..."};

  COORD textPositionStr1 {
    SHORT(m_fieldWidth / 2 - END_GAME_TEXT_STR1.size() / 2),
    SHORT(m_fieldHeight / 2)};

  COORD textPositionStr2 {
    SHORT(m_fieldWidth / 2 - END_GAME_TEXT_STR2.size() / 2),
    SHORT(m_fieldHeight / 2 + 1)};

  drawString(textPositionStr1, END_GAME_TEXT_STR1);
  drawString(textPositionStr2, END_GAME_TEXT_STR2);
  updateView();
  do
  {
    m_inputController.updateInputEvents();
    if (m_inputController.hasKeyEscape())
    {
      break;
    }
    Sleep(50);
  } while (true);

  m_inputController.closeInputController();
}
// ---------------------------------------------------------------------------
void GameEngine::showConsoleCursor(bool flag)
{
  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(m_hConsoleOut, &cursorInfo);
  cursorInfo.bVisible = flag;
  SetConsoleCursorInfo(m_hConsoleOut, &cursorInfo);
}

// ---------------------------------------------------------------------------
void GameEngine::drawSymbol(const COORD &pos, char symbol, Color color)
{
  if (validatePos(pos))
  {
    fastDrawSymbol(pos, symbol, color);
  }
}

// ---------------------------------------------------------------------------
void GameEngine::fastDrawSymbol(const COORD &pos, char symbol, Color color)
{
  const USHORT index = pos.Y * m_fieldWidth + pos.X;
  m_bufScreen[index].Char.AsciiChar = symbol;
  m_bufScreen[index].Attributes = static_cast<WORD>(color);
}

// ---------------------------------------------------------------------------
void GameEngine::fillSymbol(const SMALL_RECT &rect, char symbol, Color color)
{
  fillSymbol({rect.Left, rect.Top}, {rect.Right, rect.Bottom}, symbol, color);
}

// ---------------------------------------------------------------------------
void GameEngine::fillSymbol(const COORD &pos1, const COORD &pos2, char symbol, Color color)
{
  if (validatePos(pos1) && validatePos(pos2))
  {
    for (SHORT x {pos1.X}; x <= pos2.X; ++x)
    {
      for (SHORT y {pos1.Y}; y <= pos2.Y; ++y)
      {
        fastDrawSymbol({x, y}, symbol, color);
      }
    }
  }
}

// ---------------------------------------------------------------------------
void GameEngine::drawString(const COORD &pos, const string &str, Color color)
{
  const COORD end_pos {SHORT(pos.X + str.size()), pos.Y};
  if (validatePos(end_pos))
  {
    for (SHORT i {0}; i < SHORT(str.size()); ++i)
    {
      fastDrawSymbol({SHORT(pos.X + i), pos.Y}, str.at(i), color);
    }
  }
}

// ---------------------------------------------------------------------------
void GameEngine::drawGameObject(const GameObject *obj)
{
  switch (obj->type())
  {
    case GameObject::Player:
    case GameObject::EvilEntity:
      fillSymbol(obj->rect(), GH_BLOCK, obj->color());
      break;
    case GameObject::Wall:
      break;
  }
}

// ---------------------------------------------------------------------------
void GameEngine::moveGameObject(GameObject *obj, const COORD &new_pos)
{
  // TODO: доделать обработку статических объектов
  // if (false == obj->isStatic())
  // {
  //   return;
  // }

  const COORD old_pos {obj->pos()};
  // если объект остался на том же месте, то выходим
  if ((old_pos.X == new_pos.X) && (old_pos.Y == new_pos.Y))
  {
    return;
  }

  // выполняем очистку старой позиции
  fillSymbol(obj->rect(), 0);
  obj->setPos(new_pos);

  drawGameObject(obj);
}

// ---------------------------------------------------------------------------
void GameEngine::setLastErrorText(const char *text)
{
  m_hasError = true;
  m_lastErrorText = std::string{text};
}

// ---------------------------------------------------------------------------
bool GameEngine::validatePos(const COORD &pos)
{
  return ((pos.X >= 0) &&
          (pos.X < m_fieldWidth) &&
          (pos.Y >= 0) &&
          (pos.Y < m_fieldHeight));
}

// ---------------------------------------------------------------------------
void GameEngine::spawnEvilEntity(int seed)
{
  EvilEntity *entity = new EvilEntity{};
  entity->setSize(EVIL_ENTITY_BASE_SIZE);
  entity->setColor(Color::DarkRed);
  entity->setSpeed0Y(EVIL_ENTITY_SPEED_0Y);

  std::mt19937 generator(seed);
  std::uniform_int_distribution<SHORT> dis(m_spawnArea.Left, m_spawnArea.Right - entity->size().X);
  entity->setPos({SHORT(dis(generator)), m_spawnArea.Top});

  m_objects.push_back(std::unique_ptr<GameObject>(entity));
  drawGameObject(entity);
}

// ---------------------------------------------------------------------------
void GameEngine::updateView()
{
  WriteConsoleOutput(m_hConsoleOut,
                     m_bufScreen.get(),
                     {m_fieldWidth, m_fieldHeight},
                     {0 , 0},
                     &m_rectWindow);
}

// ---------------------------------------------------------------------------
void GameEngine::updateEvilEntities(float delta)
{
  for (auto it = m_objects.begin(); it != m_objects.end();)
  {
    if (auto *entity = (*it)->toEvilEntity())
    {
      // если дошел до нижнего конца игрового поля, то удаляем этот объект
      const SHORT end_line = m_fieldHeight - 1 - FIELD_BORDER_WIDTH;
      if (entity->pos().Y >= end_line)
      {
        // выполняем очистку старой позиции
        fillSymbol(entity->rect(), 0);
        // выполняем обновление основания
        fillSymbol(m_baseArea, BC_BASE, Color::Gray);
        it = m_objects.erase(it);
        continue;
      }

      COORD new_pos;
      new_pos.X = entity->pos().X;
      new_pos.Y = entity->stepPos0Y(entity->speed0Y(), delta);
      moveGameObject(entity, new_pos);
    }
    ++it;
  }
}

// ---------------------------------------------------------------------------
bool GameEngine::updatePlayerCollision()
{
  SMALL_RECT playerRect {m_player->rect()};
  SMALL_RECT objectRect;

  for (auto it = m_objects.cbegin(); it != m_objects.cend(); ++it)
  {
    objectRect = (*it)->rect();
    if ((playerRect.Left <= objectRect.Right) &&
        (playerRect.Right >= objectRect.Left) &&
        (playerRect.Top <= objectRect.Bottom) &&
        (playerRect.Bottom >= objectRect.Top))
    {
      return true;
    }
  }
  return false;
}
