/**
 * @file chronal_calibration.c
 * @brief Implementation for AoC Day 01, Chronal Calibration
 */

#include "chronal_calibration.h"

#include "aoc_err.h"

#include <errno.h>
#include <search.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* compute_freq(tok_t* tok){
  long res = 0;
  char* curr;
  long parsed;
  while((curr = n_tok(tok)) != NULL){
    errno = 0;
    parsed = strtol(curr, NULL, 10);
    if(errno != 0){
      set_aoc_err_msg("Error parsing frequency numbers", errno);
      return NULL;
    }
    res += parsed;
  }
  int count = 0;
  long n = res;
  while(n != 0){
    n /= 10l;
    count++;
  }
  char* output = malloc(count+2);
  snprintf(output, count+2, "%ld",res);
  return output;
}

int treecomp(const void* first, const void* second){
  if(*((long*)first) < *((long*)second)){
    return -1;
  }
  else if(*((long*)first) > *((long*)second)){
    return 1;
  }
  else{
    return 0;
  }
}

char* get_first_repetition(tok_t* tok){
  long* sum = malloc(sizeof(long));
  *sum = 0;
  long* prevsum = malloc(sizeof(long));
  *prevsum = 0;
  char* curr;
  long parsed;
  void* treeroot = NULL;
  bool repeated = false;
  long* searchres;
  // Insert initial zero
  tsearch(sum, &treeroot, treecomp);
  while(!repeated){
    while((curr = n_tok(tok)) != NULL){
      errno = 0;
      parsed = strtol(curr, NULL, 10);
      if(errno != 0){
        set_aoc_err_msg("Error parsing frequency numbers", errno);
        return NULL;
      }
      prevsum = sum;
      sum = malloc(sizeof(long));
      *sum = *prevsum + parsed;
      searchres = tsearch(sum, &treeroot, treecomp);
      if(*((long**)searchres) != sum){
        repeated = true;
        break;
      }
    }
    reset_tok(tok);
  }
  int count = 0;
  long n = *sum;
  while(n != 0){
    n /= 10l;
    count++;
  }
  char* output = malloc(count+2);
  snprintf(output, count+2, "%ld",*sum);
  tdestroy(treeroot,free);
  return output;

}
