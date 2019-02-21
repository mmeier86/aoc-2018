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

int parse_entry(const char* s, entry_t* en);

sched_t* parse_schedule(tok_t* tok);

void free_sched(sched_t* schedule);

char* most_asleep_guard(tok_t* tok);
