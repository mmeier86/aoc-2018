/**
 * @file main.c
 * @brief Implementation for AoC 2018 main function
 */

#include "main.h"

#include "aoc_err.h"
#include "aoc_streams.h"
#include "mm_files.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* usage = "Usage: %s 1|2 INPUT_FILE\n";
static const char* parterr = "Part %s requested but no part %s function provided.\n";

int exec_dayfunc(const char* fpath, char* (*dayfunc)(tok_t*)){
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
    fprintf(STDERR_STREAM, "Error in AoC function call");
    char* latest_error = get_latest_aoc_err_msg();
    if(latest_error == NULL){
      fprintf(STDERR_STREAM, ".\n");
    }
    else{
      fprintf(STDERR_STREAM,":\n%s\n",latest_error);
      free(latest_error);
    }
    return EXIT_FAILURE;
  }
  fprintf(STDOUT_STREAM, "%s\n", res);
  free(res);
  return EXIT_SUCCESS;
}

int aoc_main(int argc, char** argv, char* (*p1func)(tok_t*),
             char* (*p2func)(tok_t*)){
  (void)(p2func);
  if(argc < 3){
    fprintf(STDOUT_STREAM, "Too few arguments.\n");
    fprintf(STDOUT_STREAM, usage, argv[0]);
    return EXIT_FAILURE;
  }
  else if(argc > 3){
    fprintf(STDOUT_STREAM, "Too many arguments.\n");
    fprintf(STDOUT_STREAM, usage, argv[0]);
    return EXIT_FAILURE;
  }
  const char* part = argv[1];
  const char* fpath = argv[2];
  char* (*dayfunc)(tok_t*);
  if(strcmp("1", part) == 0){
    dayfunc = p1func;
  }
  else if(strcmp("2", part) == 0){
    dayfunc = p2func;
  }
  else{
    fprintf(STDERR_STREAM, "\"%s\" is an invalid part number. Choose 1 or 2.\n",
            part);
    return EXIT_FAILURE;
  }
  if(dayfunc == NULL){
    fprintf(STDERR_STREAM, parterr, part, part);
    return EXIT_FAILURE;
  }
  return exec_dayfunc(fpath, dayfunc);
}
