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
  return UNITY_END();
}
