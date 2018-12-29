/**
 * @file cloth_cutting.c
 * @brief Implementation for AoC Day 03, Cloth Cutting
 */

#include "cloth_cutting.h"

#include "aoc_err.h"
#include "dllist.h"

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

bool intersect(claim_t* a, claim_t* b){
  if (a->startx > b->startx+b->lengthx-1 || b->startx > a->startx+a->lengthx-1)
    return false;

  if (a->starty > b->starty+b->lengthy-1 || b->starty > a->starty+a->lengthy-1)
    return false;

  return true;
}

struct bidir{
  node_t* candidate;
  claim_t* claim;
};

typedef struct bidir bidir_t;

/**
 * Alright. Lets admit up front that this is not how this was supposed
 * to look. My initial idea was: Okay, lets just reuse Part 1, after
 * generating the claims array, check for each claim whether their
 * entire area has a value of 1 and report the ID of the claim
 * for which this was true.
 *
 * But I thought I'd be smart and remove the need to generate the
 * claims array. Instead, lets just have a check for each pair of
 * claims whether they intersect.
 * But I wanted to be extra smart: If, in the inner loop, we find
 * a conflicting claim, we just remove it from the list of claims.
 * Because it can't possibly be the one non-overlapping claim.
 * Of course, that doesn't work. As I found out when I entered
 * the solution my program offered into the AoC website. Why?
 * Because of course with the above approach, I might remove
 * a claim from testing entirely which conflicts with a claim
 * coming AFTER it. That conflict wouldn't be detected.
 *
 * But I was enamored with my "Candidates list" idea, so I
 * produced the implementation you see here.
 *
 * In principle, it uses a list of candidates for the outer
 * loop of the n*n intersection check. But it uses a second,
 * never changing full list of claims in the inner loop.
 * Now, when the current candidate intersects with a claim
 * from the inner claims loop, the candidate corresponding
 * to that claim gets removed. But the claim stays in the
 * list, so it will be checked again by the following
 * candidates. Thus, it at least isn't n*n anymore, and
 * it also doesn't use the big "cloth" array either.
 *
 */
char* find_valid_claim(tok_t* tok){
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
  dllist_t* candidates = init_list();
  bidir_t* bidirlist = malloc(num_claims*sizeof(bidir_t));
  bidir_t* curr_bidir = bidirlist;
  node_t* curr_candidate;
  claim_t* curr_claim = claims;
  for(int i=0; i<num_claims; i++){
    curr_candidate = append(candidates, curr_claim);
    curr_bidir->claim = curr_claim;
    curr_bidir->candidate = curr_candidate;
    curr_bidir++;
    curr_claim++;
  }
  curr_candidate = head(candidates);
  bool intersects;
  while(curr_candidate != NULL){
    curr_bidir = bidirlist;
    intersects = false;
    for(int i=0; i<num_claims; i++){
      if(((claim_t*)data(curr_candidate))->id != curr_bidir->claim->id){
        if(intersect(((claim_t*)data(curr_candidate)), curr_bidir->claim)){
          if(curr_bidir->candidate != NULL){
            remove_node(candidates, curr_bidir->candidate);
            curr_bidir->candidate = NULL;
          }
          intersects = true;
          break;
        }
      }
      curr_bidir++;
    }
    if(intersects){
      bidirlist[((claim_t*)data(curr_candidate))->id-1].candidate = NULL;
      curr_candidate = remove_node(candidates, curr_candidate);
    }
    else{
      curr_candidate = next(curr_candidate);
    }
  }
  free(bidirlist);
  claim_t* valid_claim = data(head(candidates));
  free_dllist(candidates);
  unsigned n = valid_claim->id;
  unsigned counter = 0;
  while(n != 0){
    n /= 10;
    counter++;
  }
  char* res = malloc(counter+2);
  snprintf(res,counter+2,"%u",valid_claim->id);
  free(claims);
  return res;
}
