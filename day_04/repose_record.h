/**
 * @file repose_record.h
 * @brief AoC 2018 Day 04, Repose Record
 */

#include "tokenizer.h"

#include <time.h>

typedef enum event{START, ASLEEP, AWAKE} event_t;

typedef struct sched_entry{
  struct tm timestamp;
  unsigned guardid;
  event_t action;
} entry_t;

typedef struct sched{
  unsigned entrycount;
  unsigned guardcount;
  unsigned* guardids;
  entry_t* schedstore;
  entry_t** schedule;
} sched_t;

typedef struct guard{
  unsigned id;
  unsigned total_minutes_asleep;
  unsigned minutes_asleep[60];
} guard_t;

typedef struct analyzed_sched{
  size_t n_guards;
  guard_t* a_guards;
  sched_t* schedule;
} analyzed_sched_t;

int parse_entry(const char* s, entry_t* en);

sched_t* parse_schedule(tok_t* tok);

void free_sched(sched_t* schedule);

void free_analyzed_sched(analyzed_sched_t* sched);

analyzed_sched_t* analyze_schedule(tok_t* tok);

char* most_asleep_guard(tok_t* tok);
