/**
 * @file cloth_cutting.h
 * @brief AoC 2018 Day 03, Cloth Cutting
 */

#include "tokenizer.h"

struct claim{
  unsigned id;
  unsigned startx;
  unsigned starty;
  unsigned lengthx;
  unsigned lengthy;
};

typedef struct claim claim_t;

claim_t* parse_all_claims(tok_t* tok);

char* cloth_slicing(tok_t* tok);
