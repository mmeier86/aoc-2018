/**
 * @file aoc_err.c
 * @brief Implementation of the aoc_err module
 */

#include "aoc_err.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Holds the latest error message from a AoC Day function
 *
 * NULL when no error occured.
 */
static char* aoc_err_msg;

void set_aoc_err_msg(const char* errmsg, int errcode){
  if(errmsg == NULL){
    return;
  }
  size_t msg_len = strlen(errmsg)+1;
  if(errcode != 0){
    msg_len += 1 + strlen(strerror(errcode));
  }
  aoc_err_msg = malloc(msg_len);
  if(errcode != 0){
    snprintf(aoc_err_msg, msg_len, "%s\n%s", errmsg, strerror(errcode));
  }
  else{
    snprintf(aoc_err_msg, msg_len, "%s", errmsg);
  }
}

char* get_latest_aoc_err_msg(){
  char* latest_msg = aoc_err_msg;
  aoc_err_msg = NULL;
  return latest_msg;
}
