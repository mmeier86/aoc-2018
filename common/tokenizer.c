/**
 * @file tokenizer.c
 * @brief Implementation of tokenizer utility
 */

#include "tokenizer.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct tokintern {
  char* s_input;
  char* s_working;
  char* s_save;
  char* s_delim;
  char* s_orig;
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
  tokenizer->s_orig = s;
  tokenizer->s_working = strdup(s);
  tokenizer->s_input = tokenizer->s_working;
  tokenizer->s_delim = delim;
  return tokenizer;
}

char* n_tok(tok_t* tok){
  if(tok == NULL){
    return NULL;
  }
  char* ret = strtok_r(tok->s_input, tok->s_delim, &tok->s_save);
  if(tok->s_input != NULL){
    // First call with this string - all subsequent calls need
    // to set the first strtok_r argument to the NULL pointer.
    tok->s_input = NULL;
  }
  return ret;
}

void free_tok(tok_t* tok){
  free(tok->s_working);
  free(tok);
}

void reset_tok(tok_t* tok){
  free(tok->s_working);
  tok->s_working = strdup(tok->s_orig);
  tok->s_input = tok->s_working;
}
