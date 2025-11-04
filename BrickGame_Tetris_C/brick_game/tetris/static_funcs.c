#include "tetrislib.h"

ptrvars_T* getGlobalPtr() {
  static ptrvars_T stream;
  return &stream;
}

GameIndices_T* getGlobalIndex() {
  static GameIndices_T stream;
  return &stream;
}

GameInfo_t* getUI() {
  static GameInfo_t stream;
  return &stream;
}

GameInfo_t updateCurrentState() {
  ptrvars_T* p_temp = getGlobalPtr();
  GameIndices_T* st_temp = getGlobalIndex();
  GameInfo_t ui_info;
  installVariables();
  Switch_T status = collisionChecking(0, 1);
  if (status == TurnedON) {
    for (int i = 0; i < FIGURESIZE; i++) {
      for (int j = 0; j < FIGURESIZE; j++) {
        if (p_temp->current_figure[i][j]) {
          int x = st_temp->blockpos_X + j;
          int y = st_temp->blockpos_Y + i;
          if (y >= 0 && y < TETFIELD_H && x >= 0 && x < TETFIELD_W) {
            p_temp->tetrisfield[y][x] = 1;
          }
        }
      }
    }
    int deleted = destroyRows(p_temp->tetrisfield);
    copypasteFigure(p_temp->current_figure, p_temp->next_figure);
    p_temp->next_figure = transferNewModule();
    st_temp->blockpos_X = (TETFIELD_W - FIGURESIZE) / 2;
    st_temp->blockpos_Y = 0;
    updateScore(&deleted);
  }
  updateLevel();
  int** display_field = mallocElements(Gamefield, NULL);
  for (int i = 0; i < TETFIELD_H; i++) {
    for (int j = 0; j < TETFIELD_W; j++) {
      display_field[i][j] = p_temp->tetrisfield[i][j];
    }
  }
  overflowField();
  updateSpeed();
  ui_info.speed = st_temp->speed;
  ui_info.next = p_temp->next_figure;
  ui_info.level = st_temp->level;
  ui_info.score = st_temp->currentscore;
  ui_info.pause = st_temp->pause_trigger;
  ui_info.high_score = st_temp->highscore;
  spawnFigureOnField(display_field, p_temp->current_figure);
  ui_info.field = display_field;
  if (st_temp->s21UI_status == Terminate) {
    for (int i = 0; i < TETFIELD_H; i++) {
      free(display_field[i]);
    }
    free(display_field);
  }
  return ui_info;
}

void overflowField() {
  ptrvars_T* p_temp = getGlobalPtr();
  GameIndices_T* st_temp = getGlobalIndex();
  for (int i = 0; i < FIGURESIZE; i++) {
    for (int j = 0; j < FIGURESIZE; j++) {
      if (p_temp->current_figure[i][j]) {
        int x = st_temp->blockpos_X + j;
        int y = st_temp->blockpos_Y + i;
        if (y >= 0 && y < TETFIELD_H && p_temp->tetrisfield[y][x] == 1) {
          st_temp->power_status = TurnedOFF;
          terminateTetris();
        }
      }
    }
  }
}

void installVariables() {
  ptrvars_T* p_temp = getGlobalPtr();
  GameIndices_T* st_temp = getGlobalIndex();
  if (p_temp->tetrisfield == NULL) {
    p_temp->tetrisfield = mallocElements(Gamefield, p_temp->tetrisfield);
  }
  if (p_temp->current_figure == NULL) {
    mallocElements(CurrentFigure, p_temp->current_figure);
    p_temp->current_figure = transferNewModule();
    st_temp->blockpos_X = (TETFIELD_W - FIGURESIZE) / 2;
    st_temp->blockpos_Y = -3;
  }
  if (p_temp->next_figure == NULL) {
    mallocElements(NextFigure, p_temp->next_figure);
    p_temp->next_figure = transferNewModule();
  }
}

int** synchronizeFieldfilling() {
  ptrvars_T* p_temp = getGlobalPtr();
  GameIndices_T* st_temp = getGlobalIndex();
  if (st_temp->attaching_status == TurnedON) {
    for (int i = 0; i < FIGURESIZE; i++) {
      for (int j = 0; j < FIGURESIZE; j++) {
        if (p_temp->current_figure[i][j]) {
          int x = st_temp->blockpos_X + j;
          int y = st_temp->blockpos_Y + i;
          if (y >= TETFIELD_H - 1 || (x >= 0 && x < TETFIELD_W && y >= 0 &&
                                      p_temp->current_figure[i][j] == 1)) {
            p_temp->tetrisfield[y][x] = 1;
          }
        }
      }
    }
  }
  return p_temp->tetrisfield;
}

void userInput(UserAction_t action, bool hold) {
  ptrvars_T* stream = getGlobalPtr();
  GameIndices_T* check = getGlobalIndex();
  check->s21UI_status = action;
  if (action == Pause) {
    check->pause_trigger =
        (check->pause_trigger == TurnedON) ? TurnedOFF : TurnedON;
  } else if (action == Start) {
    check->pause_trigger = TurnedOFF;
    check->s21UI_status = Start;
  } else if (check->s21UI_status == Start) {
    check->power_status = TurnedON;
    copypasteFigure(stream->current_figure, stream->next_figure);
  } else if (check->s21UI_status == Terminate) {
    check->power_status = TurnedOFF;
    terminateTetris();
  } else if (check->moving_status == TurnedON &&
             check->pause_trigger == TurnedOFF && hold == false) {
    check->overpowered_input = false;
    if (action == Down) {
      if (collisionChecking(0, 1) == TurnedOFF) {
        figureMovements(Down);
      }
    } else if (action == Left) {
      if (collisionChecking(-1, 0) == TurnedOFF) {
        figureMovements(Left);
      }
    } else if (action == Right) {
      if (collisionChecking(1, 0) == TurnedOFF) {
        figureMovements(Right);
      }
    } else if (action == Up) {
      if ((collisionChecking(-1, 0) == TurnedOFF) &&
          (collisionChecking(1, 0) == TurnedOFF)) {
        stream->current_figure = figureRotating();
      }
    }
  } else if (check->moving_status == TurnedON &&
             check->pause_trigger == TurnedOFF && hold == true) {
    check->overpowered_input = 70;
    if (action == Down) {
      if (collisionChecking(0, 1) == TurnedOFF) {
        figureMovements(Down);
      }
    } else if (action == Left) {
      if (collisionChecking(-1, 0) == TurnedOFF) {
        figureMovements(Left);
      }
    } else if (action == Right) {
      if (collisionChecking(1, 0) == TurnedOFF) {
        figureMovements(Right);
      }
    } else if (action == Up) {
      if ((collisionChecking(-1, 0) == TurnedOFF) &&
          (collisionChecking(1, 0) == TurnedOFF)) {
        stream->current_figure = figureRotating();
      }
    }
  }
}

void terminateTetris() {
  ptrvars_T* elems = getGlobalPtr();

  if (elems->tetrisfield != NULL) {
    for (int j = 0; j < TETFIELD_H; j++) {
      free(elems->tetrisfield[j]);
    }
    free(elems->tetrisfield);
    elems->tetrisfield = NULL;
    if (elems->current_figure != NULL) {
      for (int i = 0; i < FIGURESIZE; i++) {
        free(elems->current_figure[i]);
      }
      free(elems->current_figure);
      elems->current_figure = NULL;
    }

    if (elems->next_figure != NULL) {
      for (int i = 0; i < FIGURESIZE; i++) {
        free(elems->next_figure[i]);
      }
      free(elems->next_figure);
    }
    elems->next_figure = NULL;
  }
}

int** transferNewModule() {
  GameIndices_T* stream = getGlobalIndex();
  stream->spawnfigure_status = TurnedON;
  int** result = (int**)malloc(FIGURESIZE * sizeof(int*));
  for (int i = 0; i < FIGURESIZE; i++) {
    result[i] = (int*)calloc(FIGURESIZE, sizeof(int));
  }
  FigureID figure;
  figure = rand() % 7;
  if (figure == redlong) {
    result[1][0] = 1;
    result[1][1] = 1;
    result[1][2] = 1;
    result[1][3] = 1;
  } else if (figure == purpleZ) {
    result[1][0] = 1;
    result[1][1] = 1;
    result[2][1] = 1;
    result[2][2] = 1;
  } else if (figure == greenS) {
    result[1][0] = 1;
    result[1][1] = 1;
    result[2][1] = 1;
    result[2][2] = 1;
  } else if (figure == pinksquare) {
    result[1][1] = 1;
    result[1][2] = 1;
    result[2][1] = 1;
    result[2][2] = 1;
  } else if (figure == bluetank) {
    result[0][1] = 1;
    result[1][0] = 1;
    result[1][1] = 1;
    result[1][2] = 1;
  } else if (figure == orangeleftfoot) {
    result[0][0] = 1;
    result[1][0] = 1;
    result[1][1] = 1;
    result[1][2] = 1;
  } else if (figure == yellowrightfoot) {
    result[1][0] = 1;
    result[1][1] = 1;
    result[1][2] = 1;
    result[0][2] = 1;
  }
  stream->spawnfigure_status = TurnedOFF;
  return result;
}

void figureMovements(UserAction_t check) {
  GameIndices_T* stream = getGlobalIndex();
  if (stream->moving_status == TurnedON) {
    if (check == Down) {
      if (stream->overpowered_input == true) {
        stream->blockpos_Y++;
        if (collisionChecking(0, 1) == TurnedOFF) {
          stream->blockpos_Y++;
        }
      } else if (stream->overpowered_input == false) {
        stream->blockpos_Y++;
      }
    } else if (check == Right) {
      if (stream->overpowered_input == true) {
        stream->blockpos_X++;
        if (collisionChecking(1, 0) == TurnedOFF) {
          stream->blockpos_X++;
        }
      } else if (stream->overpowered_input == false) {
        stream->blockpos_X++;
      }
    } else if (check == Left) {
      if (stream->overpowered_input == true) {
        stream->blockpos_X--;
        if (collisionChecking(-1, 0) == TurnedOFF) {
          stream->blockpos_X--;
        }
      } else if (stream->overpowered_input == false) {
        stream->blockpos_X--;
      }
    }
  }
}

int** figureRotating() {
  ptrvars_T* stream = getGlobalPtr();
  GameIndices_T* tray = getGlobalIndex();
  tray->rotating_status = TurnedON;
  int** current_figure = stream->current_figure;
  int** figure_before = (int**)malloc(FIGURESIZE * sizeof(int*));
  for (int i = 0; i < FIGURESIZE; i++) {
    figure_before[i] = (int*)malloc(FIGURESIZE * sizeof(int));
  }
  for (int i = 0; i < FIGURESIZE; i++) {
    for (int j = 0; j < FIGURESIZE; j++) {
      figure_before[i][j] = current_figure[i][j];
    }
  }
  int** figure_now = (int**)malloc(FIGURESIZE * sizeof(int*));
  for (int i = 0; i < FIGURESIZE; i++) {
    figure_now[i] = (int*)malloc(FIGURESIZE * sizeof(int));
  }

  for (int i = 0; i < FIGURESIZE; i++) {
    for (int j = 0; j < FIGURESIZE; j++) {
      figure_now[j][FIGURESIZE - 1 - i] = figure_before[i][j];
    }
  }

  for (int i = 0; i < FIGURESIZE; i++) {
    for (int j = 0; j < FIGURESIZE; j++) {
      current_figure[i][j] = 0;
    }
  }

  for (int i = 0; i < FIGURESIZE; i++) {
    free(figure_before[i]);
  }
  free(figure_before);
  return figure_now;
}

void patternAssembling() {
  GameIndices_T* stream = getGlobalIndex();
  srand(time(NULL));
  stream->blockpos_Y = 0;
  stream->blockpos_X = (TETFIELD_W - FIGURESIZE) / 2;
  stream->currentscore = 0;
  stream->hold_button = TurnedOFF;
  stream->speed = 0;
  stream->level = 1;
  stream->moving_status = TurnedON;
  stream->pause_trigger = TurnedON;
  stream->rotating_status = TurnedOFF;
  stream->attaching_status = TurnedOFF;
  stream->overpowered_input = 0;
  FILE* file = fopen("max_score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &stream->highscore) != 1) {
      stream->highscore = 0;
    }
    fclose(file);
  }
}

Switch_T collisionChecking(int dest_X, int dest_Y) {
  ptrvars_T* stream = getGlobalPtr();
  GameIndices_T* check = getGlobalIndex();
  bool collision = false;

  for (int i = 0; i < FIGURESIZE; i++) {
    for (int j = 0; j < FIGURESIZE; j++) {
      if (stream->current_figure[i][j] == 1) {
        int locateX = check->blockpos_X + j + dest_X;
        int locateY = check->blockpos_Y + i + dest_Y;

        if (locateX < 0 || locateX == TETFIELD_W) {
          collision = true;
          check->blockrotate_status = TurnedOFF;
        } else if (locateY >= TETFIELD_H) {
          collision = true;
          check->blockrotate_status = TurnedON;
        } else if ((locateY >= 0 &&
                    stream->tetrisfield[locateY][locateX] == 1)) {
          collision = true;
          check->blockrotate_status = TurnedON;
        }
      }
    }
  }
  return collision ? TurnedON : TurnedOFF;
}

bool checkFullRows(int** field_rows, int row) {
  bool check_type = true;
  for (int check = 0; check < TETFIELD_W; check++) {
    if (field_rows[row][check] == 0) {
      check_type = false;
    }
  }
  return check_type;
}

void removeRow(int** field_rows, int row) {
  for (int r = row; r > 0; r--) {
    for (int col = 0; col < TETFIELD_W; col++) {
      field_rows[r][col] = field_rows[r - 1][col];
    }
  }
  memset(field_rows[0], 0, sizeof(int) * TETFIELD_W);
}

int destroyRows(int** field_rows) {
  int count = 0;
  for (int row = TETFIELD_H - 1; row >= 0; row--) {
    if (checkFullRows(field_rows, row) == true) {
      removeRow(field_rows, row);
      count++;
    }
  }
  saveMaxScore();
  return count;
}

void updateScore(int* count_dr) {
  GameIndices_T* stream = getGlobalIndex();
  if (*count_dr == 1) {
    stream->currentscore += 100;
  } else if (*count_dr == 2) {
    stream->currentscore += 300;
  } else if (*count_dr == 3) {
    stream->currentscore += 700;
  } else if (*count_dr == 4) {
    stream->currentscore += 1500;
  }
  *count_dr = 0;
}

void updateLevel() {
  GameIndices_T* stream = getGlobalIndex();
  int checkscore = stream->currentscore;
  stream->level = (checkscore / 600) + 1;
  if (stream->level >= 10) {
    stream->level = 10;
  }
}

void saveMaxScore() {
  GameIndices_T* stream = getGlobalIndex();
  FILE* file;
  int high_score = 0;

  file = fopen("max_score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &high_score) != 1) {
      high_score = 0;
    }
    fclose(file);
  }

  if (stream->currentscore > high_score) {
    stream->highscore = stream->currentscore;
    file = fopen("max_score.txt", "w");
    fprintf(file, "%d", stream->currentscore);
    fclose(file);
  }
}

void updateSpeed() {
  GameIndices_T* stream = getGlobalIndex();
  stream->speed = 100 - (stream->level * 10);
  if (stream->speed < 10) stream->speed = 10;
}