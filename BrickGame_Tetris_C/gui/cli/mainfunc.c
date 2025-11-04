#include "../../brick_game/tetris/tetrislib.h"

void initializeGame() {
  initscr();
  curs_set(0);
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  napms(150);
  patternAssembling();
  render(updateCurrentState());
}

void handleInput(int c, bool hold) {
  switch (c) {
    case 'u':
      userInput(Start, hold);
      break;
    case 'q':
      userInput(Terminate, hold);
      break;
    case 's':
      userInput(Down, hold);
      break;
    case SPACE_BUTTON:
      userInput(Up, hold);
      break;
    case 'a':
      userInput(Left, hold);
      break;
    case 'd':
      userInput(Right, hold);
      break;
    case 'p':
      userInput(Pause, hold);
      break;
    default:
      break;
  }
}

int main() {
  initializeGame();
  GameIndices_T* stream = getGlobalIndex();
  bool in_process = true;
  long long last_fall = currentMillis();
  const int fall_interval = 500;
  int input_count = 0;
  const int hold_delay = 150;
  // const int repeat_interval = 50;

  while (in_process) {
    int c = getch();
    long long now = currentMillis();
    if (now - last_fall >= fall_interval &&
        stream->pause_trigger == TurnedOFF) {
      userInput(Down, false);
      last_fall = now;
    }
    if (c != ERR) {
      if (now - last_fall >= hold_delay && input_count > 9) {
        handleInput(c, true);
        input_count = 0;
        if (c == 'q') {
          in_process = false;
        }
      } else {
        handleInput(c, false);
        if (c == 'q') {
          in_process = false;
        }
      }
    }
    render(updateCurrentState());
    napms(170 - stream->overpowered_input);
    input_count++;
  }
  endwin();
  return 0;
}

void render(GameInfo_t temp) {
  WINDOW* my_window = newwin(22, 32, 5, 5);
  WINDOW* stats_window = newwin(22, 28, 5, 37);
  box(my_window, 0, 0);
  box(stats_window, 0, 0);
  refresh();
  for (int i = 0; i < TETFIELD_H; i++) {
    for (int j = 0; j < TETFIELD_W; j++) {
      if (temp.field[i][j] == 1) {
        mvwprintw(my_window, i + 1, 3 * j + 1, "[ ]");
      } else {
        mvwprintw(my_window, i + 1, 3 * j + 1, " # ");
      }
    }
  }

  int u1 = 2, k1 = 7;
  int u2 = 4, k2 = 3;
  mvwprintw(stats_window, u1, k1, "Tetris by");
  mvwprintw(stats_window, u2, k2, "MarlysmaYKS[24_01]");
  mvwprintw(stats_window, 8, 2, "CurrentScore: %d", temp.score);
  mvwprintw(stats_window, 10, 2, "HighScore: %d", temp.high_score);
  mvwprintw(stats_window, 12, 2, "Level(max=10): %d", temp.level);

  for (int u = 0; u < FIGURESIZE; u++) {
    for (int k = 0; k < FIGURESIZE; k++) {
      if (temp.next[u][k] == 1) {
        mvwprintw(stats_window, u + 1 + 15, 3 * k + 8, "[ ]");
      } else {
        mvwprintw(stats_window, u + 1 + 15, 3 * k + 8, "   ");
      }
    }
  }
  wrefresh(my_window);
  delwin(my_window);
  wrefresh(stats_window);
  delwin(stats_window);
  for (int i = 0; i < TETFIELD_H; i++) {
    free(temp.field[i]);
  }
  free(temp.field);
}

long long currentMillis() {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000;
}