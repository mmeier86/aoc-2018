/**
 * @file main.c
 * @brief Implementation for AoC 2018 main function
 */

#include "main.h"

#include "mm_files.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef UNITTESTS_ENABLED
extern FILE* stdout_ut;
extern FILE* stderr_ut;
#define STDOUT_STREAM stdout_ut
#define STDERR_STREAM stderr_ut
#else
#define STDOUT_STREAM stdout
#define STDERR_STREAM stderr
#endif

static const char* usage = "Usage: %s INPUT_FILE\n";

int aoc_main(int argc, char** argv, char* (*dayfunc)(tok_t*)){
  if(argc < 2){
    fprintf(STDOUT_STREAM, "Too few arguments.\n");
    fprintf(STDOUT_STREAM, usage, argv[0]);
    return EXIT_FAILURE;
  }
  else if(argc > 2){
    fprintf(STDOUT_STREAM, "Too many arguments.\n");
    fprintf(STDOUT_STREAM, usage, argv[0]);
    return EXIT_FAILURE;
  }
  const char* fpath = argv[1];
  char* content = mm_file_read(fpath);
  int error = errno;
  if(content == NULL){
    fprintf(STDERR_STREAM, "Error loading input from %s: %s\n",
            fpath, strerror(error));
    return EXIT_FAILURE;
  }
  tok_t* tok = get_tokenizer(content, "\n");
  if(tok == NULL){
    free(content);
    fprintf(STDERR_STREAM, "Error initializing input tokenizer.\n");
    return EXIT_FAILURE;
  }
  char* res = dayfunc(tok);
  free_tok(tok);
  free(content);
  if(res == NULL){
    fprintf(STDERR_STREAM, "Error in AoC function call.\n");
    return EXIT_FAILURE;
  }
  fprintf(STDOUT_STREAM, "%s\n", res);
  free(res);
  return EXIT_SUCCESS;
}
