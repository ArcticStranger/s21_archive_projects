
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lncurses
INCLUDE = -I brick_game/tetris

LIB_DIR = brick_game/tetris
GUI_DIR = gui/cli
TEST_DIR = test
BUILD_DIR = build
DIST_DIR = s21_Tetris

LIB_FILES = $(LIB_DIR)/backend_scripts.c $(LIB_DIR)/static_funcs.c
GUI_FILES = $(GUI_DIR)/mainfunc.c
TEST_FILES = $(TEST_DIR)/test.c $(LIB_FILES)
GCOV_FILES = $(TEST_DIR)/test.c $(LIB_FILES)

GCOVFLAGS = -fprofile-arcs -ftest-coverage --coverage

UNAME_S = $(shell uname)
ifeq ($(UNAME_S),Linux)
    OPEN_CMD = xdg-open
    ADD_LDFLAGS = -lm -lsubunit
endif
ifeq ($(UNAME_S),Darwin)
    OPEN_CMD = open
    ADD_LDFLAGS =
endif

# Цели
.PHONY: all install uninstall clean dvi dist test gcov_report clang_format clang_check leaks_game leaks_test

all: install

install:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIB_FILES) $(GUI_FILES) $(LDFLAGS) $(ADD_LDFLAGS) -o $(BUILD_DIR)/Tetris
	echo 0 > $(BUILD_DIR)/max_score.txt

uninstall:
	rm -rf $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TEST_DIR)/test $(TEST_DIR)/*.txt ./gcov ./s21_Tetris.tar.gz ./report coverage.info

dvi:
	$(OPEN_CMD) ../Documentation.html

dist:
	mkdir -p $(DIST_DIR)
	cp -r brick_game $(DIST_DIR)/
	cp -r gui $(DIST_DIR)/
	cp -r test $(DIST_DIR)/
	cp *.h $(DIST_DIR)/
	cp max_score.txt $(DIST_DIR)/
	cp ../Documentation.html $(DIST_DIR)/
	tar -czf s21_Tetris.tar.gz -C /home/username/Documents/C7_BrickGame_v1.0-1/src s21_Tetris
	rm -rf $(DIST_DIR)

test:
	mkdir -p $(TEST_DIR)
	echo 0 > $(TEST_DIR)/max_score.txt
	$(CC) $(CFLAGS) $(INCLUDE) $(TEST_FILES) -lcheck -lm -lsubunit -o $(TEST_DIR)/test
	$(TEST_DIR)/test

gcov_report:
	mkdir -p ./gcov
	rm -f ./gcov/*.gcda ./gcov/*.gcno
	echo 0 > ./gcov/max_score.txt
	
	$(CC) $(CFLAGS) $(INCLUDE) $(GCOVFLAGS) \
	$(TEST_DIR)/test.c \
	$(LIB_DIR)/backend_scripts.c \
	$(LIB_DIR)/static_funcs.c \
	-lcheck $(ADD_LDFLAGS) \
	-o ./gcov/gcov_test
	
	cd ./gcov && ./gcov_test
	lcov -c -d . -o coverage.info --rc lcov_branch_coverage=1
	genhtml coverage.info --output-directory report --branch-coverage
	$(OPEN_CMD) ./report/index.html
	
clang_format:
	cp ../.clang-format $(LIB_DIR)/.clang-format
	clang-format -i $(LIB_DIR)/*.c $(LIB_DIR)/*.h
	clang-format -i $(GUI_DIR)/*.c $(GUI_DIR)/*.h
	clang-format -i $(TEST_DIR)/*.c $(TEST_DIR)/*.h
	rm -f $(LIB_DIR)/.clang-format

clang_check:
	cp ../.clang-format $(LIB_DIR)/.clang-format
	clang-format -n $(LIB_DIR)/*.c $(LIB_DIR)/*.h
	clang-format -n $(GUI_DIR)/*.c $(GUI_DIR)/*.h
	clang-format -n $(TEST_DIR)/*.c $(TEST_DIR)/*.h
	rm -f $(LIB_DIR)/.clang-format

leaks_game: install
	cd $(BUILD_DIR) && leaks -atExit -- ./Tetris

leaks_test: test
	cd $(TEST_DIR) && leaks -atExit -- ./test