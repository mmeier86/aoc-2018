/**
 * @file test_day_02.c
 * @brief Tests for AoC 2018 Day 2
 */

#include "inventory_mgmt.h"
#include <unity.h>

void test_part1_example(void){
  char in[] = "abcdef\nbababc\nabbcde\nabcccd\naabcdd\nabcdee\nababab";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_EQUAL_STRING("12",res);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_part1_example);
  return UNITY_END();
}
