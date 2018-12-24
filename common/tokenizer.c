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
  int orig_count;
  int num;
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
  tokenizer->orig_count = 0;
  tokenizer->num = 0;

  // Just compute the number of tokens by running through the
  // string. Seems horribly inefficient, but so does just counting
  // the occurences of the delimeter by hand. That would also
  // be a lot more complicated, programming wise, because we'd have
  // to follow all of strtok's rules, like skipping initial occurences.
  int count = 0;
  while(n_tok(tokenizer) != NULL){
    ++count;
  }
  tokenizer->orig_count = count;
  tokenizer->num = count;
  reset_tok(tokenizer);

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
  if(ret != NULL){
    tok->num--;
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
  tok->num = tok->orig_count;
}

int tok_count(tok_t* tok){
  return tok->num;
}
