/**
 * @file test_day_02.c
 * @brief Tests for AoC 2018 Day 2
 */

#include "inventory_mgmt.h"
#include "aoc_err.h"

#include <unity.h>

#include <stddef.h>
#include <stdlib.h>

void test_part1_tok_null_returns_null(void){
  char* res = box_checksum(NULL);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is NULL.", err);
  free(err);
}

void test_part1_empty_tok_returns_null(void){
  char in[] = "\n";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is empty.", err);
  free(err);
  free_tok(tok);
}

void test_part1_triples_not_counted_as_doubles(void){
  char in[] = "abcbb\nabgtrt";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_EQUAL_STRING("1",res);
  free_tok(tok);
  free(res);
}

void test_part1_multiple_doubles_in_id_counted_only_once(void){
  char in[] = "abcbudrr\nabgtrtlot";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_EQUAL_STRING("1",res);
  free_tok(tok);
  free(res);
}

void test_part1_multiple_triples_in_id_counted_only_once(void){
  char in[] = "abcbrubdrr\nabgtrtlo";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_EQUAL_STRING("1",res);
  free_tok(tok);
  free(res);
}

void test_part1_id_with_doubles_and_triples_counted_twice(void){
  char in[] = "abcbubdrr";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_EQUAL_STRING("1",res);
  free_tok(tok);
  free(res);
}

void test_part1_invalid_char_leads_to_abort_and_error_msg(void){
  char in[] = "aftgr\ndr8tuz";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Invalid char \"8\" in \"dr8tuz\".", err);
  free(err);
  free_tok(tok);
}

void test_part1_more_than_three_occurences_are_not_counted(void){
  char in[] = "abcb\nabcbdb\nabcbdbeb";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_EQUAL_STRING("1",res);
  free_tok(tok);
  free(res);
}

void test_part1_example(void){
  char in[] = "abcdef\nbababc\nabbcde\nabcccd\naabcdd\nabcdee\nababab";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = box_checksum(tok);
  TEST_ASSERT_EQUAL_STRING("12",res);
  free_tok(tok);
  free(res);
}

void test_part2_tok_null_returns_null(void){
  char* res = similar_id(NULL);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is NULL.", err);
  free(err);
}

void test_part2_empty_tok_returns_null(void){
  char in[] = "\n";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = similar_id(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is empty.", err);
  free(err);
  free_tok(tok);
}

void test_part2_no_match_produces_error(void){
  char in[] = "abcd\nfghj";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = similar_id(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("No Match Found.", err);
  free(err);
  free_tok(tok);
}

void test_part2_matching_string_is_returned(void){
  char in[] = "abde\nabce";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = similar_id(tok);
  TEST_ASSERT_EQUAL_STRING("abe",res);
  free_tok(tok);
  free(res);
}

void test_part2_diff_in_first_char_is_returned(void){
  char in[] = "abde\nbbde";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = similar_id(tok);
  TEST_ASSERT_EQUAL_STRING("bde",res);
  free_tok(tok);
  free(res);
}

void test_part2_example(void){
  char in[] = "abcde\nfghij\nklmno\npqrst\nfguij\naxcye\nwvxyz";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = similar_id(tok);
  TEST_ASSERT_EQUAL_STRING("fgij",res);
  free_tok(tok);
  free(res);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_part1_tok_null_returns_null);
  RUN_TEST(test_part1_empty_tok_returns_null);
  RUN_TEST(test_part1_triples_not_counted_as_doubles);
  RUN_TEST(test_part1_multiple_doubles_in_id_counted_only_once);
  RUN_TEST(test_part1_multiple_triples_in_id_counted_only_once);
  RUN_TEST(test_part1_id_with_doubles_and_triples_counted_twice);
  RUN_TEST(test_part1_invalid_char_leads_to_abort_and_error_msg);
  RUN_TEST(test_part1_more_than_three_occurences_are_not_counted);
  RUN_TEST(test_part1_example);
  RUN_TEST(test_part2_tok_null_returns_null);
  RUN_TEST(test_part2_empty_tok_returns_null);
  RUN_TEST(test_part2_no_match_produces_error);
  RUN_TEST(test_part2_matching_string_is_returned);
  RUN_TEST(test_part2_example);
  return UNITY_END();
}
