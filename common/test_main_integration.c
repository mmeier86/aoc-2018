/**
 * @file test_main_integration.c
 * @brief Integration tests for main.c
 */

#include "main.h"

#include <unity.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char* testdir = NULL;

FILE* stdout_ut = NULL;
FILE* stderr_ut = NULL;

struct memstream_t{
  char* streambuff;
  size_t streamsize;
} stderr_data, stdout_data;

char* third_line_func(tok_t* tok){
  char* curr;
  char* res = malloc(64);
  for(int i = 0; i<3; i++){
    curr = n_tok(tok);
  }
  strncpy(res, curr, 64);
  return res;
}

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

void setUp(void){
  // Output streams for main.c message checking
  stdout_ut = open_memstream(&stdout_data.streambuff, &stdout_data.streamsize);
  fflush(stdout_ut);
  stderr_ut = open_memstream(&stderr_data.streambuff, &stderr_data.streamsize);
  fflush(stderr_ut);
}

void tearDown(void){
  // Flush the streams to make sure the pointer is up to date
  fflush(stdout_ut);
  fclose(stdout_ut);
  free(stdout_data.streambuff);
  fflush(stderr_ut);
  fclose(stderr_ut);
  free(stderr_data.streambuff);
}

void test_output_third_input_line(void){
  char* fpath = get_file_path(testdir, "four_lines.txt");
  int argc = 2;
  char* argv[] = {"main", fpath};
  int res = aoc_main(argc, argv, third_line_func);
  fflush(stdout_ut);
  TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS,res);
  TEST_ASSERT_EQUAL_STRING("Hello, World!\n",stdout_data.streambuff);
  free(fpath);
}

int main(int argc, char** argv){
  if( argc != 2 ){
    fprintf( stderr, "usage: %s TESTFILE_DIR", argv[0] );
    exit( EXIT_FAILURE );
  }
  testdir = argv[1];
  UNITY_BEGIN();
  RUN_TEST(test_output_third_input_line);
  return UNITY_END();
}
