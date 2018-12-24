/**
 * @file test_tokenizer.c
 * @brief UTs for tokenizer.h
 */

#include <unity.h>
#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_create_empty_str_returns_null(void){
  tok_t* res = get_tokenizer("", "\n");
  TEST_ASSERT_NULL(res);
}

void test_create_empty_delim_returns_null(void){
  tok_t* res = get_tokenizer("Hello", "");
  TEST_ASSERT_NULL(res);
}

void test_tok_null_returns_null(void){
  char* res = n_tok(NULL);
  TEST_ASSERT_NULL(res);
}

void test_tok_single_char_returns_char(void){
  char in[] = "s";
  tok_t* tokenizer = get_tokenizer(in, "\n");
  TEST_ASSERT_NOT_NULL(tokenizer);
  char* res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("s",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  free_tok(tokenizer);
}

void test_tok_single_char_append_delim_returns_char(void){
  char in[] = "s\n";
  tok_t* tokenizer = get_tokenizer(in, "\n");
  TEST_ASSERT_NOT_NULL(tokenizer);
  char* res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("s",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  free_tok(tokenizer);
}

void test_tok_single_char_prepend_delim_returns_char(void){
  char in[] = "\ns";
  tok_t* tokenizer = get_tokenizer(in, "\n");
  TEST_ASSERT_NOT_NULL(tokenizer);
  char* res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("s",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  free_tok(tokenizer);
}

void test_tok_multi(void){
  char in[] = "Hello,\nWorld.";
  tok_t* tokenizer = get_tokenizer(in, "\n");
  TEST_ASSERT_NOT_NULL(tokenizer);
  char* res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("Hello,",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("World.",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  free_tok(tokenizer);
}

void test_tok_multi_trailing_newlines(void){
  char in[] = "Hello,\nWorld.\n\n";
  tok_t* tokenizer = get_tokenizer(in, "\n");
  TEST_ASSERT_NOT_NULL(tokenizer);
  char* res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("Hello,",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("World.",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  free_tok(tokenizer);
}

void test_tok_reset_before_first_ntok_call(void){
  char in[] = "Hello,\nWorld.\nfoo\nbar\n";
  tok_t* tokenizer = get_tokenizer(in, "\n");
  TEST_ASSERT_NOT_NULL(tokenizer);
  reset_tok(tokenizer);
  char* res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("Hello,",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("World.",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("foo",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("bar",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  free_tok(tokenizer);
}

void test_tok_reset_after_last_ntok_call(void){
  char in[] = "Hello,\nWorld.\nfoo\nbar\n";
  tok_t* tokenizer = get_tokenizer(in, "\n");
  TEST_ASSERT_NOT_NULL(tokenizer);
  char* res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("Hello,",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("World.",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("foo",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("bar",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  reset_tok(tokenizer);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("Hello,",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("World.",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("foo",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_EQUAL_STRING("bar",res);
  res = n_tok(tokenizer);
  TEST_ASSERT_NULL(res);
  free_tok(tokenizer);
}

void test_tok_count_at_start_is_correct(void){
  char in[] = "Hello, World\n";
  tok_t* tok = get_tokenizer(in, "\n");
  int res = tok_count(tok);
  TEST_ASSERT_EQUAL_INT(1,res);
  free(tok);
}

void test_tok_count_end_is_zero(void){
  char in[] = "Hello, World\n";
  tok_t* tok = get_tokenizer(in, "\n");
  while(n_tok(tok) != NULL){}
  int res = tok_count(tok);
  TEST_ASSERT_EQUAL_INT(0,res);
  free(tok);
}

void test_count_multiple_tokens(void){
  char in[] = "Hello, World\nfoo\nbar\nbaz";
  tok_t* tok = get_tokenizer(in, "\n");
  int res = tok_count(tok);
  TEST_ASSERT_EQUAL_INT(4,res);
  free(tok);
}

void test_count_repeated_delims_not_counted(void){
  char in[] = "Hello, World\nfoo\n\n\nbar\n\nbaz";
  tok_t* tok = get_tokenizer(in, "\n");
  int res = tok_count(tok);
  TEST_ASSERT_EQUAL_INT(4,res);
  free(tok);
}

void test_count_leading_and_trailing_delims_not_counted(void){
  char in[] = "\n\nHello, World\nfoo\nbar\nbaz\n\n";
  tok_t* tok = get_tokenizer(in, "\n");
  int res = tok_count(tok);
  TEST_ASSERT_EQUAL_INT(4,res);
  free(tok);
}

void test_count_running_count_correct(void){
  char in[] = "Hello, World\nfoo\nbar\nbaz";
  tok_t* tok = get_tokenizer(in, "\n");
  TEST_ASSERT_EQUAL_INT(4,tok_count(tok));
  n_tok(tok);
  TEST_ASSERT_EQUAL_INT(3,tok_count(tok));
  n_tok(tok);
  TEST_ASSERT_EQUAL_INT(2,tok_count(tok));
  n_tok(tok);
  TEST_ASSERT_EQUAL_INT(1,tok_count(tok));
  n_tok(tok);
  TEST_ASSERT_EQUAL_INT(0,tok_count(tok));
  free(tok);
}

void test_tok_reset_resets_tok_count(void){
  char in[] = "Hello, World\nfoo\nbar\nbaz";
  tok_t* tok = get_tokenizer(in, "\n");
  TEST_ASSERT_EQUAL_INT(4,tok_count(tok));
  n_tok(tok);
  n_tok(tok);
  reset_tok(tok);
  TEST_ASSERT_EQUAL_INT(4,tok_count(tok));
  free(tok);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_create_empty_str_returns_null);
  RUN_TEST(test_create_empty_delim_returns_null);
  RUN_TEST(test_tok_null_returns_null);
  RUN_TEST(test_tok_single_char_returns_char);
  RUN_TEST(test_tok_single_char_append_delim_returns_char);
  RUN_TEST(test_tok_single_char_prepend_delim_returns_char);
  RUN_TEST(test_tok_multi);
  RUN_TEST(test_tok_multi_trailing_newlines);
  RUN_TEST(test_tok_reset_before_first_ntok_call);
  RUN_TEST(test_tok_reset_after_last_ntok_call);
  RUN_TEST(test_tok_count_at_start_is_correct);
  RUN_TEST(test_tok_count_end_is_zero);
  RUN_TEST(test_count_multiple_tokens);
  RUN_TEST(test_count_repeated_delims_not_counted);
  RUN_TEST(test_count_leading_and_trailing_delims_not_counted);
  RUN_TEST(test_count_running_count_correct);
  RUN_TEST(test_tok_reset_resets_tok_count);
  return UNITY_END();
}
