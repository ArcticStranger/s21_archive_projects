#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../gui/cli/s21_interface.h"

#define FIGURESIZE 4
#define TETFIELD_H 20
#define TETFIELD_W 10
#define FPS 60
#define SEC_TO_MILLISEC 1000
#define SEC_TO_NANOSEC 1000000
#define SPACE_BUTTON 32

typedef enum { TurnedON, TurnedOFF } Switch_T;

typedef struct {
  int** current_figure;
  int** next_figure;
  int** tetrisfield;
} ptrvars_T;

typedef enum {
  COLLISION_NONE,
  COLLISION_BORDER,
  COLLISION_BOTTOM,
  COLLISION_FIGURE
} CollisionType;

typedef struct {
  Switch_T hold_button;
  Switch_T pause_trigger;
  Switch_T attaching_status;
  Switch_T rotating_status;
  Switch_T spawnfigure_status;
  Switch_T moving_status;
  Switch_T power_status;
  Switch_T blockrotate_status;
  UserAction_t s21UI_status;
  CollisionType collision_format;
  bool overpowered_input;
  int currentscore;
  int highscore;
  int speed;
  int level;
  int blockpos_X;
  int blockpos_Y;
} GameIndices_T;

typedef enum { Gamefield, CurrentFigure, NextFigure } Checkstuff_T;

typedef enum {
  redlong,
  orangeleftfoot,
  yellowrightfoot,
  pinksquare,
  greenS,
  bluetank,
  purpleZ
} FigureID;

int checkMemoryLeaks();
void updateSpeed();
void installVariables();
void overflowField();
void saveMaxScore();
void updateLevel();
void updateScore(int* count_dr);
int destroyRows(int** field_rows);
void removeRow(int** field_rows, int row);
bool checkFullRows(int** field_rows, int row);
void clearFullRows(int** field);
int** synchronizeFieldfilling();
void freeElements(int** source, int count);
Switch_T collisionChecking(int dest_X, int dest_Y);
ptrvars_T* getGlobalPtr();
GameIndices_T* getGlobalIndex();
GameInfo_t* getUI();
GameInfo_t updateCurrentState();
void userInput(UserAction_t action, bool hold);
void terminateTetris();
void figureMovements(UserAction_t check);
void patternAssembling();
int** figureRotating();
int** transferNewModule();
int** mallocElements(Checkstuff_T ptr, int** checkptr);
void copypasteFigure(int** dest, int** source);
void spawnFigureOnField(int** field, int** figure);

#endif