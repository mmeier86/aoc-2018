/**
 * @file cloth_cutting.c
 * @brief Implementation for AoC Day 03, Cloth Cutting
 */

#include "cloth_cutting.h"

#include "aoc_err.h"

#include <stddef.h>

int parse_claim(char* in, claim_t* claim){
  (void)(claim);
  (void)(in);
  return -1;
}

claim_t* parse_all_claims(tok_t* tok){
  (void)(tok);
  return NULL;
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
