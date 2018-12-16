/**
 * @file chronal_calibration.c
 * @brief Implementation for AoC Day 01, Chronal Calibration
 */

#include "chronal_calibration.h"

#include "aoc_err.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
