/**
 * @file inventory_mgmt.c
 * @brief Implementation for AoC Day 02, Inventory Management
 */

#include "inventory_mgmt.h"

#include "aoc_err.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* box_checksum(tok_t* tok){
  if(tok == NULL){
    set_aoc_err_msg("Tokenizer is NULL.", 0);
    return NULL;
  }
  if(tok_count(tok) == 0){
    set_aoc_err_msg("Tokenizer is empty.", 0);
    return NULL;
  }
  unsigned doubles = 0;
  unsigned triples = 0;
  unsigned counts[26];
  char* id;
  bool saw_triple;
  bool saw_double;
  while((id = n_tok(tok)) != NULL){
    memset(counts,'\0',sizeof(unsigned)*26);
    saw_triple = false;
    saw_double = false;
    for(char* c = id; *c != '\0'; c++){
      if(((int) *c) < 97 || ((int) *c) > 122){
        char buff[64];
        snprintf(buff, 64, "Invalid char \"%c\" in \"%s\".", *c, id);
        set_aoc_err_msg(buff, 0 );
        return NULL;
      }
      counts[((int)*c)-97]++;
    }
    for(int i = 0; i<26 && !(saw_double && saw_triple); i++){
      if(counts[i] == 2u && !saw_double){
        saw_double = true;
        doubles++;
      }
      else if(counts[i] == 3u && !saw_triple){
        saw_triple = true;
        triples++;
      }
    }
  }
  unsigned checksum = doubles * triples;
  unsigned n = checksum;
  unsigned count = 0;
  while(n != 0){
    n /= 10u;
    count++;
  }
  char* out = malloc(count+2);
  snprintf(out, count+2, "%u", checksum);
  return out;
}

bool s_comp(char* s1, char* s2, size_t* diff_index){
  if(strlen(s1) != strlen(s2)){
    return false;
  }
  int numdiff = 0;
  for(size_t i = 0; i<strlen(s1) && numdiff<2; i++){
    if(s1[i] != s2[i]){
      numdiff++;
      *diff_index = i;
    }
  }
  return numdiff<2;
}

char* similar_id(tok_t* tok){
  if(tok == NULL){
    set_aoc_err_msg("Tokenizer is NULL.", 0);
    return NULL;
  }
  if(tok_count(tok) == 0){
    set_aoc_err_msg("Tokenizer is empty.", 0);
    return NULL;
  }
  int num_toks = tok_count(tok);
  char** s_tab = malloc(sizeof(char*)*num_toks);
  char* curr;
  char** counter = s_tab;
  while((curr = n_tok(tok)) != NULL){
    *(counter++) = curr;
  }
  char* res_id = NULL;
  size_t diff_index;
  for(int outer = 0; outer<num_toks && res_id == NULL; outer++){
    for(int inner = outer+1; inner<num_toks; inner++){
      if(s_comp(s_tab[outer], s_tab[inner], &diff_index)){
        res_id = s_tab[outer];
        break;
      }
    }
  }
  free(s_tab);
  char* out = NULL;
  if(res_id != NULL){
    out = malloc(strlen(res_id));
    memcpy(out, res_id, diff_index);
    memcpy(out+diff_index, res_id+diff_index+1,strlen(res_id)-diff_index);
  }
  else{
    set_aoc_err_msg("No Match Found.", 0);
  }
  return out;
}
