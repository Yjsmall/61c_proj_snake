#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) { return x > 42; }

bool is_vowel(char c) {
  char *vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False
  otherwise. Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is
  a,e,i,o,u) and returns false otherwise Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
  const char *TAIL = "wasd";
  for (size_t i = 0; i < strlen(TAIL); i++) {
    bool test_tail = is_tail(TAIL[i]);
    if (!assert_true("test_is_tail", test_tail)) {
      return false;
    }
  }
  return true;
}

bool test_is_head() {
  const char *HEAD = "WASDx";
  for (size_t i = 0; i < strlen(HEAD); i++) {
    bool test_head = is_head(HEAD[i]);
    if (!assert_true("test_is_head", test_head)) {
      return false;
    }
  }
  return true;
}

bool test_is_snake() {
  const char *SNAKE = "wasd^<v>WASDx";
  for (size_t i = 0; i < strlen(SNAKE); i++) {
    bool test_snake = is_snake(SNAKE[i]);
    if (!assert_true("test_is_snake", test_snake)) {
      return false;
    }
  }
  return true;
}

bool char_equal(char first, char second) {
  return first == second ? true : false;
}

bool test_body_to_tail() {
  const char *BODY = "^<v>x";
  const char *TAIL = "wasd?";
  for (size_t i = 0; i < strlen(BODY); i++) {
    char c = body_to_tail(BODY[i]);
    bool res = char_equal(TAIL[i], c);
    if (!assert_true("test_is_snake", res)) {
      return false;
    }
  }
  return true;
}

bool test_head_to_body() {
  // TODO: Implement this function.
  const char *HEAD = "WASDx";
  const char *BODY = "^<v>?";
  for (size_t i = 0; i < strlen(BODY); i++) {
    char c = head_to_body(HEAD[i]);
    bool res = char_equal(BODY[i], c);
    if (!assert_true("test_is_head_to_body", res)) {
      return false;
    }
  }
  return true;
}

bool int_equal(unsigned int first, unsigned int second) {
  return first == second ? true : false;
}

bool test_get_next_row() {
  const char *DOWN = "vsS";
  const char *UP = "^wW";
  const char *others = "???";
  unsigned int nums[] = {0, 1, 2};
  unsigned int base = 1;

  for (int i = 0; i < strlen(DOWN); i++) {
    unsigned int l = get_next_row(base, DOWN[i]);
    unsigned int r = get_next_row(base, UP[i]);
    unsigned int m = get_next_row(base, others[i]);
    if (!assert_true("test_get_next_row", int_equal(l, nums[2]))) {
      return false;
    }
    if (!assert_true("test_get_next_row", int_equal(m, nums[1]))) {
      return false;
    }
    if (!assert_true("test_get_next_row", int_equal(r, nums[0]))) {
      return false;
    }
  }
  return true;
}

bool test_get_next_col() {
  // TODO: Implement this function.
  const char *RIGHT = ">dD";
  const char *LEFT = "<aA";
  const char *others = "???";
  unsigned int nums[] = {0, 1, 2};
  unsigned int base = 1;

  for (int i = 0; i < strlen(RIGHT); i++) {
    unsigned int l = get_next_col(base, RIGHT[i]);
    unsigned int r = get_next_col(base, LEFT[i]);
    unsigned int m = get_next_col(base, others[i]);
    if (!assert_true("test_get_next_col", int_equal(l, nums[2]))) {
      return false;
    }
    if (!assert_true("test_get_next_col", int_equal(m, nums[1]))) {
      return false;
    }
    if (!assert_true("test_get_next_col", int_equal(r, nums[0]))) {
      return false;
    }
  }
  return true;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
