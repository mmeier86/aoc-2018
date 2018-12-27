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

int parse_claim(char* in, claim_t* claim){
  int res = sscanf(in, "#%*d @ %u,%u: %ux%u",
                   &claim->startx,
                   &claim->starty,
                   &claim->lengthx,
                   &claim->lengthy
                   );
  return res == 4 ? 0 : -1;
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
  return NULL;
}
