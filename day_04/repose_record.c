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
#include <search.h>
#include <string.h>

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

int comp_entry_guard_id(const void* first, const void* second){
  if (*(unsigned*) first < *(unsigned*) second)
    return -1;
  if (*(unsigned*) first > *(unsigned*) second)
    return 1;
  return 0;
}

void tree_destroy(void* node){
  // Empty because tdestroy needs destory function to apply to nodes,
  // but our node data is stored in a single array which is destroyed
  // separately.
  (void)(node);
}

int comp_entry_by_time(const void* first, const void* second){
  entry_t* pfirst = *((entry_t**) first);
  entry_t* psecond = *((entry_t**) second);
  if(pfirst->timestamp.tm_year < psecond->timestamp.tm_year){
    return -1;
  }
  else if(pfirst->timestamp.tm_year > psecond->timestamp.tm_year){
    return 1;
  }
  if(pfirst->timestamp.tm_mon < psecond->timestamp.tm_mon){
    return -1;
  }
  else if(pfirst->timestamp.tm_mon > psecond->timestamp.tm_mon){
    return 1;
  }
  if(pfirst->timestamp.tm_mday < psecond->timestamp.tm_mday){
    return -1;
  }
  else if(pfirst->timestamp.tm_mday > psecond->timestamp.tm_mday){
    return 1;
  }
  if(pfirst->timestamp.tm_hour < psecond->timestamp.tm_hour){
    return -1;
  }
  else if(pfirst->timestamp.tm_hour > psecond->timestamp.tm_hour){
    return 1;
  }
  if(pfirst->timestamp.tm_min < psecond->timestamp.tm_min){
    return -1;
  }
  else if(pfirst->timestamp.tm_min > psecond->timestamp.tm_min){
    return 1;
  }
  return 0;
}

void set_guard_ids(sched_t* sched){
  unsigned curr_guard;
  for(size_t i = 0; i<sched->entrycount; i++){
    if(sched->schedule[i]->action == START){
      curr_guard = sched->schedule[i]->guardid;
    }
    else{
      sched->schedule[i]->guardid = curr_guard;
    }
  }
}

sched_t* parse_schedule(tok_t* tok){
  if(tok == NULL){
    set_aoc_err_msg("Tokenizer is NULL.", 0);
    return NULL;
  }
  if(tok_count(tok) == 0){
    set_aoc_err_msg("Tokenizer is empty.", 0);
    return NULL;
  }
  sched_t* sched = malloc(sizeof(sched_t));
  sched->entrycount = tok_count(tok);
  sched->guardcount = 0u;
  sched->schedstore = malloc(sched->entrycount*sizeof(entry_t));
  sched->schedule = malloc(sched->entrycount*sizeof(entry_t*));
  void* guard_ids = NULL;
  char* curr_entry = NULL;
  entry_t* curr_store_entry = sched->schedstore;
  entry_t** curr_sched_entry = sched->schedule;
  while((curr_entry = n_tok(tok)) != NULL){
    if(parse_entry(curr_entry, curr_store_entry) != 0){
      // Parse error, returning
      if(guard_ids!=NULL){
        tdestroy(guard_ids,tree_destroy);
      }
      return NULL;
    }
    if(curr_store_entry->action == START){
      if(tfind(&curr_store_entry->guardid,guard_ids,comp_entry_guard_id) == NULL){
        sched->guardcount++;
        tsearch(&curr_store_entry->guardid,guard_ids,comp_entry_guard_id);
      }
    }
    *curr_sched_entry = curr_store_entry;
    curr_store_entry++;
    curr_sched_entry++;
  }
  qsort(sched->schedule, sched->entrycount, sizeof(entry_t*), comp_entry_by_time);
  set_guard_ids(sched);
  return sched;
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
