/**
 * @file cloth_cutting.c
 * @brief Implementation for AoC Day 03, Cloth Cutting
 */

#include "repose_record.h"

#include "aoc_err.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <regex.h>

static regex_t* shiftstart = NULL;
static regex_t* asleep = NULL;
static regex_t* wakeup = NULL;

int parse_entry(const char* s, entry_t* en){
  if(s == NULL || en == NULL){
    return -1;
  }
  *en = (entry_t) {0};
  char* remainder = strptime(s, "[%Y-%m-%d %H:%M]", &en->timestamp);
  if(remainder == NULL){
    char errbuff[128];
    snprintf(errbuff, 128, "Failed to parse time in \"%s\".", s);
    set_aoc_err_msg(errbuff, 0);
    return -1;
  }
  if(shiftstart == NULL && asleep == NULL && wakeup == NULL){
    // Only compile regeps once
    shiftstart = malloc(sizeof(regex_t));
    asleep = malloc(sizeof(regex_t));
    wakeup = malloc(sizeof(regex_t));
    regcomp(shiftstart, "Guard #[0-9]+ begins shift", REG_EXTENDED | REG_NOSUB);
    regcomp(asleep, "falls asleep", REG_EXTENDED | REG_NOSUB);
    regcomp(wakeup, "wakes up", REG_EXTENDED | REG_NOSUB);
  }
  if(regexec(shiftstart, remainder, 0, NULL, 0) == 0){
    en->action = START;
    int res = sscanf(remainder, " Guard #%d begins shift", &en->guardid);
    if(res != 1){
      char errbuff[128];
      snprintf(errbuff, 128, "Failed to parse Guard ID in \"%s\".", s);
      set_aoc_err_msg(errbuff, 0);
      return -1;
    }
  }
  else if(regexec(asleep, remainder, 0, NULL, 0) == 0){
    en->action = ASLEEP;
  }
  else if(regexec(wakeup, remainder, 0, NULL, 0) == 0){
    en->action = AWAKE;
  }
  else{
    char errbuff[128];
    snprintf(errbuff, 128, "Failed to match action in \"%s\".", s);
    set_aoc_err_msg(errbuff, 0);
    return -1;
  }
  return 0;
}

void free_sched(sched_t* schedule){
  // First, free the compiled regexps
  regfree(shiftstart);
  free(shiftstart);
  shiftstart = NULL;
  regfree(asleep);
  free(asleep);
  asleep = NULL;
  regfree(wakeup);
  free(wakeup);
  wakeup = NULL;
  if(schedule != NULL){
    free(schedule->schedule);
    free(schedule->schedstore);
    free(schedule);
  }
}

char* most_asleep_guard(tok_t* tok){
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
