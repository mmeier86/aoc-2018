/**
 * @file test_day_01.c
 * @brief Tests for AoC 2018 Day 1
 */

#include "chronal_calibration.h"
#include <unity.h>

#include <stdlib.h>

void test_single_entry(void){
  char input[] = "+1\n";
  tok_t* tok = get_tokenizer(input, "\n");
  char* res = compute_freq(tok);
  TEST_ASSERT_EQUAL_STRING("1",res);
  free_tok(tok);
  free(res);
}

void test_multiple_entry(void){
  char input[] = "+1\n+5\n-8";
  tok_t* tok = get_tokenizer(input, "\n");
  char* res = compute_freq(tok);
  TEST_ASSERT_EQUAL_STRING("-2",res);
  free_tok(tok);
  free(res);
}

void test_zero_change(void){
  char input[] = "+1\n+0\n-8";
  tok_t* tok = get_tokenizer(input, "\n");
  char* res = compute_freq(tok);
  TEST_ASSERT_EQUAL_STRING("-7",res);
  free_tok(tok);
  free(res);
}

void test_aoc_example_1(void){
  char input[] = "+1\n+1\n+1";
  tok_t* tok = get_tokenizer(input, "\n");
  char* res = compute_freq(tok);
  TEST_ASSERT_EQUAL_STRING("3",res);
  free_tok(tok);
  free(res);
}

void test_aoc_example_2(void){
  char input[] = "+1\n+1\n-2";
  tok_t* tok = get_tokenizer(input, "\n");
  char* res = compute_freq(tok);
  TEST_ASSERT_EQUAL_STRING("0",res);
  free_tok(tok);
  free(res);
}

void test_aoc_example_3(void){
  char input[] = "-1\n-2\n-3";
  tok_t* tok = get_tokenizer(input, "\n");
  char* res = compute_freq(tok);
  TEST_ASSERT_EQUAL_STRING("-6",res);
  free_tok(tok);
  free(res);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_single_entry);
  RUN_TEST(test_multiple_entry);
  RUN_TEST(test_zero_change);
  RUN_TEST(test_aoc_example_1);
  RUN_TEST(test_aoc_example_2);
  RUN_TEST(test_aoc_example_3);
  UNITY_END();
}
