#include "tetrislib.h"

int** mallocElements(Checkstuff_T ptr, int** checkptr) {
  if (ptr == Gamefield) {
    checkptr = (int**)malloc(TETFIELD_H * sizeof(int*));

    for (int i = 0; i < TETFIELD_H; i++) {
      checkptr[i] = (int*)calloc(TETFIELD_W, sizeof(int));
    }
    return checkptr;
  } else if (ptr == CurrentFigure || ptr == NextFigure) {
    checkptr = (int**)malloc(FIGURESIZE * sizeof(int*));

    for (int i = 0; i < FIGURESIZE; i++) {
      checkptr[i] = (int*)calloc(FIGURESIZE, sizeof(int));
    }
    return checkptr;
  } else
    return 0;
}

void copypasteFigure(int** dest, int** source) {
  for (int i = 0; i < FIGURESIZE; i++) {
    for (int j = 0; j < FIGURESIZE; j++) {
      dest[i][j] = source[i][j];
    }
  }
}

void spawnFigureOnField(int** field, int** figure) {
  GameIndices_T* stream = getGlobalIndex();
  int check_y = stream->blockpos_Y;
  int check_x = stream->blockpos_X;
  for (int i = 0; i < FIGURESIZE; i++) {
    for (int j = 0; j < FIGURESIZE; j++) {
      if (figure[i][j] == 1) {
        int y = i + check_y;
        int x = j + check_x;
        if (y >= 0 && y < TETFIELD_H && x >= 0 && x < TETFIELD_W) {
          field[y][x] = 1;
        }
      }
    }
  }
}

void freeElements(int** source, int count) {
  for (int i = 0; i < count; i++) {
    free(source[i]);
  }
  free(source);
}
