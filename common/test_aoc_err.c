/**
 * @file test_aoc_err.c
 * @brief UTs for the aoc_err module
 */

#include <unity.h>
#include "aoc_err.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_errmsg_null_leaves_aoc_err_msg_unchanged(void){
  set_aoc_err_msg("Foo", 0);
  set_aoc_err_msg(NULL,0);
  set_aoc_err_msg(NULL,ENOENT);
  char* res = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Foo",res);
  free(res);
}

void test_errmsg_without_errcode_is_shown(void){
  set_aoc_err_msg("My Error Message.", 0);
  char* res = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("My Error Message.", res);
  free(res);
}

void test_errmsg_with_errcode_is_shown(void){
  char exp[1024];
  sprintf(exp, "My Error Message.\n%s", strerror(ENOENT));
  set_aoc_err_msg("My Error Message.", ENOENT);
  char* res = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING(exp,res);
  free(res);
}

void test_get_latest_resets_error_msg(void){
  set_aoc_err_msg("My Error Message.", 0);
  char* intermediate = get_latest_aoc_err_msg();
  free(intermediate);
  char* res = get_latest_aoc_err_msg();
  TEST_ASSERT_NULL(res);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_errmsg_with_errcode_is_shown);
  RUN_TEST(test_errmsg_without_errcode_is_shown);
  RUN_TEST(test_errmsg_null_leaves_aoc_err_msg_unchanged);
  RUN_TEST(test_get_latest_resets_error_msg);
  return UNITY_END();
}
