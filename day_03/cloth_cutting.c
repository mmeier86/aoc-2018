/**
 * @file cloth_cutting.c
 * @brief Implementation for AoC Day 03, Cloth Cutting
 */

#include "cloth_cutting.h"

#include "aoc_err.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_claim(char* in, claim_t* claim){
  int res = sscanf(in, "#%u @ %u,%u: %ux%u",
                   &claim->id,
                   &claim->startx,
                   &claim->starty,
                   &claim->lengthx,
                   &claim->lengthy
                   );
  return res == 5 ? 0 : -1;
}

claim_t* parse_all_claims(tok_t* tok){
  if(tok == NULL){
    set_aoc_err_msg("Tokenizer is NULL.", 0);
    return NULL;
  }
  claim_t* claims = malloc(sizeof(claim_t)*tok_count(tok));
  char* curr_in = NULL;
  claim_t* curr_claim = claims;
  bool error = false;
  while((curr_in = n_tok(tok)) != NULL){
    if(parse_claim(curr_in, curr_claim++) != 0){
      error = true;
      break;
    }
  }
  if(error){
    char errmsg[64];
    snprintf(errmsg, 64, "Error while parsing \"%s\".", curr_in);
    set_aoc_err_msg(errmsg, 0);
    free(claims);
    return NULL;
  }
  return claims;
}

char* cloth_slicing(tok_t* tok){
  if(tok == NULL){
    set_aoc_err_msg("Tokenizer is NULL.", 0);
    return NULL;
  }
  if(tok_count(tok) == 0){
    set_aoc_err_msg("Tokenizer is empty.", 0);
    return NULL;
  }
  int num_claims = tok_count(tok);
  claim_t* claims = parse_all_claims(tok);
  if(claims == NULL){
    return NULL;
  }
  unsigned* cloth = malloc(1000*1000*sizeof(unsigned));
  memset(cloth,0,1000*1000*sizeof(unsigned));
  claim_t* curr = claims;
  for(int i = 0; i<num_claims; i++){
    for(unsigned x = curr->startx*1000; x<curr->startx*1000+curr->lengthx*1000;
        x+=1000){
      for(unsigned y = curr->starty; y<curr->starty+curr->lengthy; y++){
        cloth[x+y]++;
      }
    }
    curr++;
  }
  free(claims);
  unsigned overlaps = 0;
  for(unsigned i = 0; i<1000*1000; i++){
    if(cloth[i]>1){
      overlaps++;
    }
  }
  unsigned temp = overlaps;
  unsigned count = 0;
  while(temp != 0){
    temp /= 10u;
    count++;
  }
  char* res = malloc(count+2);
  snprintf(res,count+2, "%u",overlaps);
  free(cloth);
  return res;
}
