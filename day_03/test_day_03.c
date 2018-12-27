/**
 * @file test_day_03.c
 * @brief Tests for AoC 2018 Day 3
 */

#include "cloth_cutting.h"
#include "aoc_err.h"

#include <unity.h>

#include <stddef.h>
#include <stdlib.h>

void test_part1_tok_null_returns_null(void){
  char* res = cloth_slicing(NULL);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is NULL.", err);
  free(err);
}

void test_part1_empty_tok_returns_null(void){
  char in[] = "\n";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = cloth_slicing(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is empty.", err);
  free(err);
  free_tok(tok);
}

void test_parse_claims_null_sets_error(void){
  claim_t* res = parse_all_claims(NULL);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is NULL.", err);
  free(err);
}

void test_parse_claims_malformed_input_sets_error(void){
  char in[] = "123 @ 3,2: 5r4";
  tok_t* tok = get_tokenizer(in, "\n");
  claim_t* res = parse_all_claims(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Error while parsing \"123 @ 3,2: 5r4\".",err);
  free(err);
}

void test_parse_claims_parse_single(void){
  char in[] = "#123 @ 3,2: 5x4";
  tok_t* tok = get_tokenizer(in, "\n");
  claim_t* res = parse_all_claims(tok);
  TEST_ASSERT_EQUAL_UINT(3u, res[0].startx);
  TEST_ASSERT_EQUAL_UINT(2u, res[0].starty);
  TEST_ASSERT_EQUAL_UINT(5u, res[0].lengthx);
  TEST_ASSERT_EQUAL_UINT(4u, res[0].lengthy);
  free(res);
  free_tok(tok);
}

void test_parse_claims_aoc_example(void){
  char in[] = "#1 @ 1,3: 4x4\n#2 @ 3,1: 4x4\n#3 @ 5,5: 2x2";
  tok_t* tok = get_tokenizer(in, "\n");
  claim_t* res = parse_all_claims(tok);
  TEST_ASSERT_EQUAL_UINT(1u, res[0].startx);
  TEST_ASSERT_EQUAL_UINT(3u, res[0].starty);
  TEST_ASSERT_EQUAL_UINT(4u, res[0].lengthx);
  TEST_ASSERT_EQUAL_UINT(4u, res[0].lengthy);
  TEST_ASSERT_EQUAL_UINT(3u, res[1].startx);
  TEST_ASSERT_EQUAL_UINT(1u, res[1].starty);
  TEST_ASSERT_EQUAL_UINT(4u, res[1].lengthx);
  TEST_ASSERT_EQUAL_UINT(4u, res[1].lengthy);
  TEST_ASSERT_EQUAL_UINT(5u, res[2].startx);
  TEST_ASSERT_EQUAL_UINT(5u, res[2].starty);
  TEST_ASSERT_EQUAL_UINT(2u, res[2].lengthx);
  TEST_ASSERT_EQUAL_UINT(2u, res[2].lengthy);
  free(res);
  free_tok(tok);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_part1_tok_null_returns_null);
  RUN_TEST(test_part1_empty_tok_returns_null);
  RUN_TEST(test_parse_claims_null_sets_error);
  RUN_TEST(test_parse_claims_malformed_input_sets_error);
  RUN_TEST(test_parse_claims_parse_single);
  RUN_TEST(test_parse_claims_aoc_example);
  return UNITY_END();
}
