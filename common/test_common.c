/**
 * @file test_common.c
 * @brief UTs for common code in AoC 2018
 */

#include <unity.h>
#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* testdir = NULL;

char* get_file_path(const char* dir, const char* filename){
  size_t dirlen = strlen( dir );
  size_t namelen = strlen( filename );
  size_t pathlen = dirlen + namelen + 1;
  char* res = malloc( sizeof(char) * pathlen +1 );
  memcpy( res, dir, dirlen );
  res[dirlen] = '/';
  memcpy( res + dirlen +1, filename, namelen );
  res[pathlen] = '\0';
  return res;
}

void test_missing_file(void){
  char* fpath = get_file_path(testdir, "missing.txt");
  errno = 0;
  char* res = mm_file_read( fpath );
  int res_err = errno;
  free(fpath);
  TEST_ASSERT_NULL(res);
  TEST_ASSERT_EQUAL_INT(ENOENT,res_err);
}

void test_content_empty_file(void){
  char* fpath = get_file_path(testdir, "empty.txt");
  errno = 0;
  char* res = mm_file_read( fpath );
  int res_err = errno;
  free(fpath);
  TEST_ASSERT_NULL(res);
  TEST_ASSERT_EQUAL_INT(EINVAL,res_err);
}

void test_content_single_char_file(void){
  char* fpath = get_file_path(testdir, "single_char.txt");
  errno = 0;
  char* res = mm_file_read( fpath );
  int res_err = errno;
  free(fpath);
  TEST_ASSERT_EQUAL_INT(0,res_err);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL('\0',res[2]);
  TEST_ASSERT_EQUAL_STRING("y\n",res);
  free(res);
}

void test_content_single_line_file(void){
  char* fpath = get_file_path(testdir, "single_line.txt");
  errno = 0;
  char* res = mm_file_read( fpath );
  int res_err = errno;
  free(fpath);
  TEST_ASSERT_EQUAL_INT(0,res_err);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL('\0',res[14]);
  TEST_ASSERT_EQUAL_STRING("Hello, World!\n",res);
  free(res);
}

void test_content_multi_line_file(void){
  char* fpath = get_file_path(testdir, "multi_line.txt");
  errno = 0;
  char* res = mm_file_read( fpath );
  int res_err = errno;
  free(fpath);
  TEST_ASSERT_EQUAL_INT(0,res_err);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL('\0',res[22]);
  TEST_ASSERT_EQUAL_STRING("Foo\nBar\nHello, World!\n",res);
  free(res);
}

void test_path_null(void){
  char* fpath = NULL;
  errno = 0;
  char* res = mm_file_read( fpath );
  int res_err = errno;
  TEST_ASSERT_EQUAL_INT(EFAULT,res_err);
  TEST_ASSERT_NULL(res);
}

void test_path_empty(void){
  char* fpath = "";
  errno = 0;
  char* res = mm_file_read( fpath );
  int res_err = errno;
  TEST_ASSERT_EQUAL_INT(ENOENT,res_err);
  TEST_ASSERT_NULL(res);
}

int main( int argc, char** argv ){
  if( argc != 2 ){
    fprintf( stderr, "usage: %s TESTFILE_DIR", argv[0] );
    exit( EXIT_FAILURE );
  }
  testdir = argv[1];
  UNITY_BEGIN();
  RUN_TEST(test_missing_file);
  RUN_TEST(test_content_empty_file);
  RUN_TEST(test_content_single_char_file);
  RUN_TEST(test_content_single_line_file);
  RUN_TEST(test_content_multi_line_file);
  RUN_TEST(test_path_null);
  RUN_TEST(test_path_empty);
  return UNITY_END();
}
