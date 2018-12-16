/**
 * @file tokenizer.c
 * @brief Implementation of tokenizer utility
 */

#include "tokenizer.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct tokintern {
  char* s_working;
  char* s_save;
  char* s_delim;
};

tok_t* get_tokenizer(char* s, char* delim){
  if(s == NULL || delim == NULL){
    return NULL;
  }
  if(!strncmp(s,"",sizeof(char))){
    return NULL;
  }
  if(!strncmp(delim,"",sizeof(char))){
    return NULL;
  }

  tok_t* tokenizer = malloc(sizeof(tok_t));
  if(tokenizer == NULL){
    return NULL;
  }
  tokenizer->s_working = s;
  tokenizer->s_delim = delim;
  return tokenizer;
}

char* n_tok(tok_t* tok){
  if(tok == NULL){
    return NULL;
  }
  char* ret = strtok_r(tok->s_working, tok->s_delim, &tok->s_save);
  if(tok->s_working != NULL){
    // First call with this string - all subsequent calls need
    // to set the first strtok_r argument to the NULL pointer.
    tok->s_working = NULL;
  }
  return ret;
}

void free_tok(tok_t* tok){
  free(tok);
}
