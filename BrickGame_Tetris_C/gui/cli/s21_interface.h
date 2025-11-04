#ifndef s21UI_H
#define s21UI_H

#include "../../brick_game/tetris/tetrislib.h"

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

void initializeGame();
void handleInput(int c, bool hold);
void render(GameInfo_t temp);
long long currentMillis();
// GameInfo_t updateGurrentState();
// void userInput (UserAction_t action, bool hold);
#endif