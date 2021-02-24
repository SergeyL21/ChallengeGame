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
#include <string>
#include <memory>

#include "player.h"
#include "input_controller.h"

// ---------------------------------------------------------------------------
// класс игрового движка
class GameEngine
{
public:
  GameEngine() = default;
  ~GameEngine() = default;

  // метод выделения ресурсов для консоли и установки ее размеров
  bool initConsole(SHORT width, SHORT height);
  // метод запуска игрового движка
  void start();
  // метод получения последней ошибке в виде текста
  std::string lastErrorText() const;
  // метод индикации о наличии ошибки
  bool hasError() const;

private:
  // метод включения отображения позиции курсора
  void showConsoleCursor(bool flag);
  // метод отрисовки одного символа по заданным координатам
  void drawSymbol(const COORD &pos, char symbol, Color color = Color::White);
  // метод быстрой отрисовки одного символа по заданным координатам
  void fastDrawSymbol(const COORD &pos, char symbol, Color color = Color::White);
  // метод заполнения символами заданной области
  void fillSymbol(const SMALL_RECT &rect, char symbol, Color color = Color::White);
  // метод заполнения символами заданной области (при помощи двух точек)
  void fillSymbol(const COORD &pos1, const COORD &pos2, char symbol, Color color = Color::White);
  // метод отрисовки строки по заданным координатам
  void drawString(const COORD &pos, const std::string &str, Color color = Color::White);
  // метод отрисовки игрового объекта
  void drawGameObject(const GameObject *obj);
  // метод перемещения игрового объекта на новую позицию
  void moveGameObject(GameObject *obj, const COORD &new_pos);
  // метод обновления информации о последней ошибке
  void setLastErrorText(const char *text);
  // метод проверки валидности введенных координат
  bool validatePos(const COORD &pos);
  // метод генерации нового противника по координатам получаемым из ГСЧ
  void spawnEvilEntity(int seed = 0);

  // метод обновления отрисовки
  void updateView();
  // метод обновления состояний противников (здесь также происходит их удаление)
  void updateEvilEntities(float delta);
  // метод проверки столкновений игрока с игровыми объектами
  bool updatePlayerCollision();

  // включение окна стартового меню игры
  void startMenu();
  // включение окна завершающего игру
  void endGame();

private:
  // флаг о результатах инициализации консоли
  bool m_initializationFlag{false};
  // имеем ли мы ошибки выполнения программы
  bool m_hasError{false};
  // текст последней ошибки
  std::string m_lastErrorText;
  // флаг окончания игры
  bool m_gameOver{false};
  // буфер экрана с символами
  std::unique_ptr<CHAR_INFO[]> m_bufScreen;
  // текущий счет игры
  unsigned m_score{0u};
  // текущий размер окна консоли
  SMALL_RECT m_rectWindow {0, 0, 0, 0};
  // текущая область появления противников
  SMALL_RECT m_spawnArea {0, 0, 0, 0};
  // текущий область базы по которой перемещается игрок
  SMALL_RECT m_baseArea {0, 0, 0, 0};
  // текущая ширина окна консоли
  SHORT m_fieldWidth {0};
  // текущая высота окна консоли
  SHORT m_fieldHeight {0};
  // указатель на консоль вывода данных
  HANDLE m_hConsoleOut { NULL };

  // объект получения данных о нажатии клавиш игроком
  InputController m_inputController;
  // объект игрока
  std::unique_ptr<Player> m_player;
  // объекты на карте
  std::vector<std::unique_ptr<GameObject>> m_objects;
};
