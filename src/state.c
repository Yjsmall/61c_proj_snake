#include "state.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row,
                         unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  unsigned int row_num = 18;
  unsigned int col_num = 20;
  game_state_t *game = malloc(sizeof(game_state_t));
  game->board = malloc(sizeof(char *) * row_num);
  char *board = "####################";
  char *contents = "#                  #";
  for (int i = 0; i < row_num; i++) {
    game->board[i] = malloc(sizeof(char) * (col_num + 1));
    if (i == 0 || i == row_num - 1) {
      strcpy(game->board[i], board);
      strcpy(game->board[i], board);
    } else {
      strcpy(game->board[i], contents);
    }
  }

  // snake setting
  game->board[2][2] = 'd';
  game->board[2][3] = '>';
  game->board[2][4] = 'D';
  game->board[2][9] = '*';

  game->num_rows = row_num;
  game->num_snakes = 1;
  game->snakes = malloc(sizeof(snake_t) * (game->num_snakes));
  game->snakes->tail_row = 2;
  game->snakes->tail_col = 2;
  game->snakes->head_row = 2;
  game->snakes->head_col = 4;
  game->snakes->live = true;

  return game;
}

/* Task 2 */
void free_state(game_state_t *state) {
  for (size_t i = 0; i < state->num_rows; i++) {
    free(state->board[i]);
  }
  free(state->board);
  state->board = NULL;

  free(state->snakes);
  state->snakes = NULL;

  free(state);
  state = NULL;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for (int i = 0; i < state->num_rows; i++) {
    fprintf(fp, "%s\n", state->board[i]);
  }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row,
                         unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  const char *TAIL = "wasd";
  return strchr(TAIL, c) ? true : false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  const char *HEAD = "WASDx";
  return strchr(HEAD, c) ? true : false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  const char *SNAKE = "wasd^<v>WASDx";
  return strchr(SNAKE, c) ? true : false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  char result;
  switch (c) {
  case '^':
    result = 'w';
    break;
  case '<':
    result = 'a';
    break;
  case 'v':
    result = 's';
    break;
  case '>':
    result = 'd';
    break;
  default:
    result = '?';
    break;
  }
  return result;
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  char result;
  switch (c) {
  case 'W':
    result = '^';
    break;
  case 'A':
    result = '<';
    break;
  case 'S':
    result = 'v';
    break;
  case 'D':
    result = '>';
    break;
  default:
    result = '?';
    break;
  }
  return result;
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  const char *DOWN = "vsS";
  const char *UP = "^wW";
  if (strchr(DOWN, c)) {
    return cur_row + 1;
  }
  if (strchr(UP, c)) {
    return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  const char *RIGHT = ">dD";
  const char *LEFT = "<aA";
  if (strchr(RIGHT, c)) {
    return cur_col + 1;
  }
  if (strchr(LEFT, c)) {
    return cur_col - 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake
  is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  unsigned int head_row = state->snakes[snum].head_row;
  unsigned int head_col = state->snakes[snum].head_col;
  char head = get_board_at(state, head_row, head_col);

  if (!is_head(head)) {
    return '?';
  } else {
    if (head == 'W' || head == 'S') {
      head_row = get_next_row(head_row, head);
    } else {
      head_col = get_next_col(head_col, head);
    }
  }
  return get_board_at(state, head_row, head_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the
  head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  unsigned int head_row = state->snakes[snum].head_row;
  unsigned int head_col = state->snakes[snum].head_col;
  char head = get_board_at(state, head_row, head_col);
  char body = head_to_body(head);
  set_board_at(state, head_row, head_col, body);

  if (head == 'W' || head == 'S') {
    head_row = get_next_row(head_row, head);
    state->snakes[snum].head_row = head_row;
  } else {
    head_col = get_next_col(head_col, head);
    state->snakes[snum].head_col = head_col;
  }
  set_board_at(state, head_row, head_col, head);
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  unsigned int tail_row = state->snakes[snum].tail_row;
  unsigned int tail_col = state->snakes[snum].tail_col;
  char tail = get_board_at(state, tail_row, tail_col);
  set_board_at(state, tail_row, tail_col, ' ');

  if (tail == 'w' || tail == 's') {
    tail_row = get_next_row(tail_row, tail);
    state->snakes[snum].tail_row = tail_row;
  } else {
    tail_col = get_next_col(tail_col, tail);
    state->snakes[snum].tail_col = tail_col;
  }
  tail = get_board_at(state, tail_row, tail_col);
  tail = body_to_tail(tail);
  set_board_at(state, tail_row, tail_col, tail);
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  unsigned int num_snakes = state->num_snakes;
  for (unsigned int i = 0; i < num_snakes; i++) {
    unsigned int head_row = state->snakes[i].head_row;
    unsigned int head_col = state->snakes[i].head_col;

    char nxt = next_square(state, i);

    if (nxt == '*') {
      update_head(state, i);
      add_food(state);
    } else if (nxt == ' ') {
      update_head(state, i);
      update_tail(state, i);
    } else {
      state->snakes[i].live = false;
      set_board_at(state, head_row, head_col, 'x');
    }
  }
}

/* Task 5 */
game_state_t *load_board(FILE *fp) {
  if (fp == NULL) {
    return NULL;
  }

  game_state_t *state = malloc(sizeof(game_state_t));
  unsigned int base = 10;
  unsigned int cnt = 0;
  char buf[1024];

  state->board = malloc(sizeof(char *) * base);

  while (fgets(buf, sizeof(buf), fp) != NULL) {
    state->board[cnt] = malloc(sizeof(char) * 1024);

    strcpy(state->board[cnt], buf);

    cnt++;

    if (cnt == base) {
      base *= 2;
      char **tmp = realloc(state->board, sizeof(char *) * base);
      if (tmp == NULL) {
        for (size_t i = 0; i < base; i++) {
          free(state->board[i]);
        }
        free(state->board);
        return NULL;
      } else {
        state->board = tmp;
      }
    }
  }
  state->num_rows = cnt;
  state->num_snakes = 0;
  state->snakes = NULL;

  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}
