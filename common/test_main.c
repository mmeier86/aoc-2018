/**
 * @file test_main.c
 * @brief Tests for main.c
 */

#include "aoc_err.h"
#include "common_mocks.h"
#include "main.h"

#include <unity.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

FILE* stdout_ut = NULL;
FILE* stderr_ut = NULL;

struct memstream_t{
  char* streambuff;
  size_t streamsize;
} stderr_data, stdout_data;

static int func_callcount = 0;
static tok_t* func_param;
static char* func_retval = NULL;

char* empty_func(tok_t* tok){
  func_callcount++;
  func_param = tok;
  return func_retval;
}

void setUp(void){
  func_callcount = 0;
  func_param = NULL;

  // Output streams for main.c message checking
  stdout_ut = open_memstream(&stdout_data.streambuff, &stdout_data.streamsize);
  fflush(stdout_ut);
  stderr_ut = open_memstream(&stderr_data.streambuff, &stderr_data.streamsize);
  fflush(stderr_ut);

  // Set up common function mocks
  mock_mm_file_read = malloc(sizeof(struct mock_mm_file_read_t));
  mock_mm_file_read->callcount = 0;
  mock_mm_file_read->param_1 = NULL;
  mock_mm_file_read->retval = NULL;
  mock_mm_file_read->set_errno_to = 0;

  mock_get_tokenizer = malloc(sizeof(struct mock_get_tokenizer_t));
  mock_get_tokenizer->callcount = 0;
  mock_get_tokenizer->param_1 = NULL;
  mock_get_tokenizer->param_2 = NULL;
  mock_get_tokenizer->retval = NULL;

  mock_n_tok = malloc(sizeof(struct mock_n_tok_t));
  mock_n_tok->callcount = 0;
  mock_n_tok->param_1 = NULL;
  mock_n_tok->retval = NULL;

  mock_free_tok = malloc(sizeof(struct mock_free_tok_t));
  mock_free_tok->callcount = 0;
  mock_free_tok->param_1 = NULL;

  mock_get_latest_aoc_err_msg = malloc(sizeof(struct mock_get_latest_aoc_err_msg_t));
  mock_get_latest_aoc_err_msg->callcount = 0;
  mock_get_latest_aoc_err_msg->retval = NULL;
}

void tearDown(void){
  // Flush the streams to make sure the pointer is up to date
  fflush(stdout_ut);
  fclose(stdout_ut);
  free(stdout_data.streambuff);
  fflush(stderr_ut);
  fclose(stderr_ut);
  free(stderr_data.streambuff);

  // Free function mocks
  free(mock_mm_file_read->param_1);
  free(mock_mm_file_read);

  free(mock_get_tokenizer->param_1);
  free(mock_get_tokenizer->param_2);
  free(mock_get_tokenizer);

  free(mock_n_tok->param_1);
  free(mock_n_tok);

  free(mock_free_tok);
  free(mock_get_latest_aoc_err_msg);
}

void test_too_few_cli_args_prints_usage(void){
  char* argv[] = {"main"};
  int argc = 1;
  const char* exp = "Too few arguments.\nUsage: main INPUT_FILE\n";
  int res = aoc_main(argc, argv, empty_func);
  fflush(stdout_ut);
  TEST_ASSERT_EQUAL_INT(EXIT_FAILURE,res);
  TEST_ASSERT_EQUAL_STRING(exp,stdout_data.streambuff);
}

void test_too_many_cli_args_prints_usage(void){
  char* argv[] = {"main","foo","bar"};
  int argc = 3;
  const char* exp = "Too many arguments.\nUsage: main INPUT_FILE\n";
  int res = aoc_main(argc, argv, empty_func);
  fflush(stdout_ut);
  TEST_ASSERT_EQUAL_INT(EXIT_FAILURE,res);
  TEST_ASSERT_EQUAL_STRING(exp,stdout_data.streambuff);
  free(func_retval);
}

void test_file_read_gets_unchanged_path(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  aoc_main(argc, argv, empty_func);
  TEST_ASSERT_EQUAL_STRING("missing.txt",mock_mm_file_read->param_1);
  TEST_ASSERT_EQUAL_INT(1,mock_mm_file_read->callcount);
}

void test_file_missing_error_printed(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char exp[1024] = "Error loading input: ";
  snprintf(exp, 1024, "Error loading input from missing.txt: %s\n",
           strerror(ENOENT));
  mock_mm_file_read->retval = NULL;
  mock_mm_file_read->set_errno_to = ENOENT;
  aoc_main(argc, argv, empty_func);
  fflush(stderr_ut);
  TEST_ASSERT_EQUAL_STRING(exp,stderr_data.streambuff);
  TEST_ASSERT_EQUAL_INT(1,mock_mm_file_read->callcount);
}

void test_file_loading_error_aborts(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  mock_mm_file_read->retval = NULL;
  mock_mm_file_read->set_errno_to = ENOENT;
  int res = aoc_main(argc, argv, empty_func);
  TEST_ASSERT_EQUAL_INT(EXIT_FAILURE, res);
  TEST_ASSERT_EQUAL_INT(1,mock_mm_file_read->callcount);
  TEST_ASSERT_EQUAL_INT(0,mock_get_tokenizer->callcount);
  TEST_ASSERT_EQUAL_INT(0,mock_free_tok->callcount);
  TEST_ASSERT_EQUAL_INT(0,func_callcount);
}

void test_tokenizer_gets_string_from_file(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  tok_t tok;
  mock_mm_file_read->retval = content;
  mock_get_tokenizer->retval = &tok;
  aoc_main(argc, argv, empty_func);
  TEST_ASSERT_EQUAL_INT(1,mock_get_tokenizer->callcount);
  TEST_ASSERT_EQUAL_STRING("Hello, World!\n",mock_get_tokenizer->param_1);
  TEST_ASSERT_EQUAL_STRING("\n",mock_get_tokenizer->param_2);
}

void test_tokenizer_failure_error_printed(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char exp[] = "Error initializing input tokenizer.\n";
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  mock_get_tokenizer->retval = NULL;
  aoc_main(argc, argv, empty_func);
  fflush(stderr_ut);
  TEST_ASSERT_EQUAL_STRING(exp,stderr_data.streambuff);
  TEST_ASSERT_EQUAL_INT(1,mock_get_tokenizer->callcount);
}

void test_tokenizer_failure_aborts(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  mock_get_tokenizer->retval = NULL;
  int res = aoc_main(argc, argv, empty_func);
  TEST_ASSERT_EQUAL_INT(EXIT_FAILURE, res);
  TEST_ASSERT_EQUAL_INT(1,mock_get_tokenizer->callcount);
  TEST_ASSERT_EQUAL_INT(0,mock_n_tok->callcount);
  TEST_ASSERT_EQUAL_INT(0,mock_free_tok->callcount);
}

void test_tokenizer_handed_to_func(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  tok_t tok;
  tok.id = 15;
  mock_get_tokenizer->retval = &tok;
  aoc_main(argc, argv, empty_func);
  TEST_ASSERT_EQUAL_PTR(&tok,func_param);
  TEST_ASSERT_EQUAL_INT(tok.id, func_param->id);
  TEST_ASSERT_EQUAL_INT(1,func_callcount);
}

void test_tokenizer_freed(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  tok_t tok;
  tok.id = 15;
  mock_get_tokenizer->retval = &tok;
  aoc_main(argc, argv, empty_func);
  TEST_ASSERT_EQUAL_PTR(&tok,mock_free_tok->param_1);
  TEST_ASSERT_EQUAL_INT(tok.id, mock_free_tok->param_1->id);
  TEST_ASSERT_EQUAL_INT(1,func_callcount);
}

void test_func_result_printed(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  tok_t tok;
  tok.id = 15;
  mock_get_tokenizer->retval = &tok;
  func_retval = malloc(64);
  strcpy(func_retval, "Foo, Bar!");
  int res = aoc_main(argc, argv, empty_func);
  fflush(stdout_ut);
  TEST_ASSERT_EQUAL_INT(1,func_callcount);
  TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS,res);
  TEST_ASSERT_EQUAL_STRING("Foo, Bar!\n",stdout_data.streambuff);
}

void test_func_returns_null_error_printed(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  tok_t tok;
  tok.id = 15;
  mock_get_tokenizer->retval = &tok;
  func_retval = NULL;
  mock_get_latest_aoc_err_msg->retval = malloc(64);
  snprintf(mock_get_latest_aoc_err_msg->retval, 64, "Foo");
  char* exp = "Error in AoC function call:\nFoo\n";
  int res = aoc_main(argc, argv, empty_func);
  fflush(stderr_ut);
  TEST_ASSERT_EQUAL_INT(1,func_callcount);
  TEST_ASSERT_EQUAL_INT(EXIT_FAILURE,res);
  TEST_ASSERT_EQUAL_STRING(exp,stderr_data.streambuff);
}

void test_func_returns_null_no_error_msg_aborts(void){
  char* argv[] = {"main", "missing.txt"};
  int argc = 2;
  char* content = malloc(64);
  strcpy(content, "Hello, World!\n");
  mock_mm_file_read->retval = content;
  tok_t tok;
  tok.id = 15;
  mock_get_tokenizer->retval = &tok;
  func_retval = NULL;
  mock_get_latest_aoc_err_msg->retval = NULL;
  char* exp = "Error in AoC function call.\n";
  int res = aoc_main(argc, argv, empty_func);
  fflush(stderr_ut);
  TEST_ASSERT_EQUAL_INT(1,func_callcount);
  TEST_ASSERT_EQUAL_INT(EXIT_FAILURE,res);
  TEST_ASSERT_EQUAL_STRING(exp,stderr_data.streambuff);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_too_few_cli_args_prints_usage);
  RUN_TEST(test_too_many_cli_args_prints_usage);
  RUN_TEST(test_file_read_gets_unchanged_path);
  RUN_TEST(test_file_missing_error_printed);
  RUN_TEST(test_file_loading_error_aborts);
  RUN_TEST(test_tokenizer_gets_string_from_file);
  RUN_TEST(test_tokenizer_failure_error_printed);
  RUN_TEST(test_tokenizer_failure_aborts);
  RUN_TEST(test_tokenizer_handed_to_func);
  RUN_TEST(test_tokenizer_freed);
  RUN_TEST(test_func_result_printed);
  RUN_TEST(test_func_returns_null_no_error_msg_aborts);
  RUN_TEST(test_func_returns_null_error_printed);
  return UNITY_END();
}
