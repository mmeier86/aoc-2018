/**
 * @file common_mocks.c
 * @brief Implementation of mocks for common library modules
 */

#include "common_mocks.h"

#include <stddef.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mock_mm_file_read_t* mock_mm_file_read = NULL;
struct mock_get_tokenizer_t* mock_get_tokenizer = NULL;
struct mock_n_tok_t* mock_n_tok = NULL;
struct mock_free_tok_t* mock_free_tok = NULL;
struct mock_get_latest_aoc_err_msg_t* mock_get_latest_aoc_err_msg = NULL;

char* mm_file_read(const char* fpath){
  if(mock_mm_file_read == NULL){
    fprintf(stderr,"mm_file_read called but no mock set. Aborting.");
    abort();
  }
  if(fpath==NULL){
    mock_mm_file_read->param_1 = NULL;
  }
  else{
    mock_mm_file_read->param_1 = malloc(strlen(fpath)+1);
    strcpy(mock_mm_file_read->param_1, fpath);
  }
  mock_mm_file_read->callcount++;
  errno = mock_mm_file_read->set_errno_to;
  return mock_mm_file_read->retval;
}

tok_t* get_tokenizer(char* s, char* delim){
  if(mock_get_tokenizer == NULL){
    fprintf(stderr,"get_tokenizer called but no mock set. Aborting.");
    abort();
  }
  if(s==NULL){
    mock_get_tokenizer->param_1 = s;
  }
  else{
    mock_get_tokenizer->param_1 = malloc(strlen(s)+1);
    strcpy(mock_get_tokenizer->param_1, s);
  }
  if(delim==NULL){
    mock_get_tokenizer->param_2 = delim;
  }
  else{
    mock_get_tokenizer->param_2 = malloc(strlen(delim)+1);
    strcpy(mock_get_tokenizer->param_2, delim);
  }
  mock_get_tokenizer->callcount++;
  return mock_get_tokenizer->retval;
}

char* n_tok(tok_t* tok){
  if(mock_n_tok == NULL){
    fprintf(stderr,"n_tok called but no mock set. Aborting.");
    abort();
  }
  if(tok==NULL){
    mock_n_tok->param_1 = tok;
  }
  else{
    mock_n_tok->param_1 = malloc(sizeof(tok_t));
    memcpy(mock_n_tok->param_1, tok, sizeof(tok_t));
  }
  mock_n_tok->callcount++;
  return mock_n_tok->retval;
}

void free_tok(tok_t* tok){
  if(mock_free_tok == NULL){
    fprintf(stderr,"free_tok called but no mock set. Aborting.");
    abort();
  }
  mock_free_tok->param_1 = tok;
  mock_free_tok->callcount++;
}

char* get_latest_aoc_err_msg(){
  if(mock_get_latest_aoc_err_msg == NULL){
    fprintf(stderr,"get_latest_aoc_err_msg called but no mock set. Aborting.");
    abort();
  }
  mock_free_tok->callcount++;
  return mock_get_latest_aoc_err_msg->retval;
}
