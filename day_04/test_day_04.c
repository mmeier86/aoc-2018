/**
 * @file test_day_04.c
 * @brief Tests for AoC 2018 Day 4
 */

#include "repose_record.h"
#include "aoc_err.h"

#include <unity.h>

#include <stddef.h>
#include <stdlib.h>

void check_tm(struct tm* exp, struct tm* actual){
  TEST_ASSERT_EQUAL_INT(exp->tm_sec, actual->tm_sec);
  TEST_ASSERT_EQUAL_INT(exp->tm_min, actual->tm_min);
  TEST_ASSERT_EQUAL_INT(exp->tm_hour, actual->tm_hour);
  TEST_ASSERT_EQUAL_INT(exp->tm_mday, actual->tm_mday);
  TEST_ASSERT_EQUAL_INT(exp->tm_mon, actual->tm_mon);
  TEST_ASSERT_EQUAL_INT(exp->tm_year, actual->tm_year);
}

void check_entry(entry_t* exp, entry_t* actual){
  check_tm(&exp->timestamp, &actual->timestamp);
  TEST_ASSERT_EQUAL_UINT(exp->guardid, actual->guardid);
  TEST_ASSERT_EQUAL_INT(exp->action, actual->action);
}

void test_parse_entry_null_string_returns_error(void){
  entry_t en;
  int res = parse_entry(NULL, &en);
  TEST_ASSERT_EQUAL_INT(-1,res);
}

void test_parse_entry_null_en_returns_error(void){
  char in[] = "[1518-06-03 00:17] falls asleep";
  int res = parse_entry(in, NULL);
  TEST_ASSERT_EQUAL_INT(-1,res);
}

void test_parse_entry_malformed_time_sets_error(void){
  char in[] = "[15aaa18-06-03 00:17] falls asleep";
  entry_t en;
  int res = parse_entry(in, &en);
  TEST_ASSERT_EQUAL_INT(-1,res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Failed to parse time in "
                           "\"[15aaa18-06-03 00:17] falls asleep\".", err);
  free(err);
}

void test_parse_entry_regex_error_sets_error_message(void){
  char in[] = "[1518-06-03 00:17] fallaaaaawers asleep";
  entry_t en;
  int res = parse_entry(in, &en);
  TEST_ASSERT_EQUAL_INT(-1,res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Failed to match action in "
                           "\"[1518-06-03 00:17] fallaaaaawers asleep\".", err);
  free(err);
  free_sched(NULL);
}

void test_parse_entry_parses_shift_start_event_correctly(void){
  char in[] = "[1518-08-31 23:56] Guard #3011 begins shift";
  struct tm exptime = {0,56,23,31,7,-382,0,0,0,0,0};
  entry_t exp = {exptime,3011,START};
  entry_t actual = {0};
  int res = parse_entry(in, &actual);
  TEST_ASSERT_EQUAL_INT(0,res);
  check_entry(&exp, &actual);
  free_sched(NULL);
}

void test_parse_entry_parses_falling_asleep_event_correctly(void){
  char in[] = "[1518-06-30 00:01] falls asleep";
  struct tm exptime = {0,1,0,30,5,-382,0,0,0,0,0};
  entry_t exp = {exptime,0,ASLEEP};
  entry_t actual = {0};
  int res = parse_entry(in, &actual);
  TEST_ASSERT_EQUAL_INT(0,res);
  check_entry(&exp, &actual);
  free_sched(NULL);
}

void test_parse_entry_parses_waking_up_event_correctly(void){
  char in[] = "[1518-10-08 00:23] wakes up";
  struct tm exptime = {0,23,0,8,9,-382,0,0,0,0,0};
  entry_t exp = {exptime,0,AWAKE};
  entry_t actual = {0};
  int res = parse_entry(in, &actual);
  TEST_ASSERT_EQUAL_INT(0,res);
  check_entry(&exp, &actual);
  free_sched(NULL);
}

void test_parse_sched_tok_null_returns_null(void){
  sched_t* res = parse_schedule(NULL);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is NULL.", err);
  free(err);
}

void test_parse_sched_single_guard_counts_correct(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(5,res->entrycount);
  TEST_ASSERT_EQUAL_UINT(1,res->guardcount);
  free_sched(res);
  free_tok(tok);
}

entry_t* single_guard_expected_schedule(){
  entry_t* exp = malloc(5*sizeof(entry_t));
  exp[0] = (entry_t){(struct tm){0,5,0,1,10,-382,0,0,0,0,0},10,ASLEEP};
  exp[1] = (entry_t){(struct tm){0,30,0,1,10,-382,0,0,0,0,0},10,ASLEEP};
  exp[2] = (entry_t){(struct tm){0,0,0,1,10,-382,0,0,0,0,0},10,START};
  exp[3] = (entry_t){(struct tm){0,25,0,1,10,-382,0,0,0,0,0},10,AWAKE};
  exp[4] = (entry_t){(struct tm){0,55,0,1,10,-382,0,0,0,0,0},10,AWAKE};
  return exp;
}

void test_parse_sched_single_guard_correct_entries(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  entry_t* exp = single_guard_expected_schedule();
  for(int i=0; i<5; i++){
    check_entry(exp+i, &(res->schedstore[i]));
  }
  free_sched(res);
  free_tok(tok);
  free(exp);
}

void test_parse_sched_single_guard_sorted_by_time(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  entry_t* exp = single_guard_expected_schedule();
  check_entry(exp+2, res->schedule[0]);
  check_entry(exp, res->schedule[1]);
  check_entry(exp+3, res->schedule[2]);
  check_entry(exp+1, res->schedule[3]);
  check_entry(exp+4, res->schedule[4]);
  free_sched(res);
  free_tok(tok);
  free(exp);
}

void test_parse_sched_single_guard_id_correct(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(1u,res->guardcount);
  TEST_ASSERT_EQUAL_UINT(10u,res->guardids[0]);
  free_sched(res);
  free_tok(tok);
}

// Times:
// Guard #2:
//   1518-11-01/02: 00:07 - 00:18, 00:47 - 00:51
// Guard #10:
//   1518-11-01: 00:05 - 00:25 , 00:30 - 00:55
//   1518-11-04: 00:07 - 00:41
// Guard #167:
//   1518-11-03: 00:22 - 00:25 , 00:31 - 00:33 , 00:39 - 00:41
entry_t* multi_guard_expected_schedule(){
  entry_t* exp = malloc(20*sizeof(entry_t));
  exp[0] = (entry_t){(struct tm){0,5,0,1,10,-382,0,0,0,0,0},10,ASLEEP};
  exp[1] = (entry_t){(struct tm){0,30,0,1,10,-382,0,0,0,0,0},10,ASLEEP};
  exp[2] = (entry_t){(struct tm){0,22,0,3,10,-382,0,0,0,0,0},167,ASLEEP};
  exp[3] = (entry_t){(struct tm){0,0,0,1,10,-382,0,0,0,0,0},10,START};
  exp[4] = (entry_t){(struct tm){0,47,0,2,10,-382,0,0,0,0,0},2,ASLEEP};
  exp[5] = (entry_t){(struct tm){0,25,0,1,10,-382,0,0,0,0,0},10,AWAKE};
  exp[6] = (entry_t){(struct tm){0,39,0,3,10,-382,0,0,0,0,0},167,ASLEEP};
  exp[7] = (entry_t){(struct tm){0,18,0,2,10,-382,0,0,0,0,0},2,AWAKE};
  exp[8] = (entry_t){(struct tm){0,2,0,3,10,-382,0,0,0,0,0},167,START};
  exp[9] = (entry_t){(struct tm){0,2,0,4,10,-382,0,0,0,0,0},10,START};
  exp[10] = (entry_t){(struct tm){0,7,0,4,10,-382,0,0,0,0,0},10,ASLEEP};
  exp[11] = (entry_t){(struct tm){0,41,0,4,10,-382,0,0,0,0,0},10,AWAKE};
  exp[12] = (entry_t){(struct tm){0,55,0,1,10,-382,0,0,0,0,0},10,AWAKE};
  exp[13] = (entry_t){(struct tm){0,33,0,3,10,-382,0,0,0,0,0},167,AWAKE};
  exp[14] = (entry_t){(struct tm){0,7,0,2,10,-382,0,0,0,0,0},2,ASLEEP};
  exp[15] = (entry_t){(struct tm){0,31,0,3,10,-382,0,0,0,0,0},167,ASLEEP};
  exp[16] = (entry_t){(struct tm){0,56,23,1,10,-382,0,0,0,0,0},2,START};
  exp[17] = (entry_t){(struct tm){0,41,0,3,10,-382,0,0,0,0,0},167,AWAKE};
  exp[18] = (entry_t){(struct tm){0,25,0,3,10,-382,0,0,0,0,0},167,AWAKE};
  exp[19] = (entry_t){(struct tm){0,51,0,2,10,-382,0,0,0,0,0},2,AWAKE};
  return exp;
}

void test_parse_sched_multi_guard_counts_correct(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(20,res->entrycount);
  TEST_ASSERT_EQUAL_UINT(3,res->guardcount);
  free_sched(res);
  free_tok(tok);
}

void test_parse_sched_multi_guard_id_correct(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(3u,res->guardcount);
  TEST_ASSERT_EQUAL_UINT(2u,res->guardids[0]);
  TEST_ASSERT_EQUAL_UINT(10u,res->guardids[1]);
  TEST_ASSERT_EQUAL_UINT(167u,res->guardids[2]);
  free_sched(res);
  free_tok(tok);
}

void test_parse_sched_multi_guard_correct_entries(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  entry_t* exp = multi_guard_expected_schedule();
  for(int i=0; i<20; i++){
    check_entry(exp+i, &(res->schedstore[i]));
  }
  free_sched(res);
  free_tok(tok);
  free(exp);
}

void test_parse_sched_multi_guard_sorted_by_time(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  entry_t* exp = multi_guard_expected_schedule();
  check_entry(exp+3, res->schedule[0]);
  check_entry(exp, res->schedule[1]);
  check_entry(exp+5, res->schedule[2]);
  check_entry(exp+1, res->schedule[3]);
  check_entry(exp+12, res->schedule[4]);
  check_entry(exp+16, res->schedule[5]);
  check_entry(exp+14, res->schedule[6]);
  check_entry(exp+7, res->schedule[7]);
  check_entry(exp+4, res->schedule[8]);
  check_entry(exp+19, res->schedule[9]);
  check_entry(exp+8, res->schedule[10]);
  check_entry(exp+2, res->schedule[11]);
  check_entry(exp+18, res->schedule[12]);
  check_entry(exp+15, res->schedule[13]);
  check_entry(exp+13, res->schedule[14]);
  check_entry(exp+6, res->schedule[15]);
  check_entry(exp+17, res->schedule[16]);
  check_entry(exp+9, res->schedule[17]);
  check_entry(exp+10, res->schedule[18]);
  check_entry(exp+11, res->schedule[19]);
  free_sched(res);
  free_tok(tok);
  free(exp);
}

void test_parse_schedule_failed_parse_error_message(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-06-03 00:17] fallaaaaawers asleep\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  sched_t* res = parse_schedule(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Failed to match action in "
                           "\"[1518-06-03 00:17] fallaaaaawers asleep\".", err);
  free(err);
  free_tok(tok);
}

void test_analyze_sched_tok_null_returns_null(void){
  analyzed_sched_t* res = analyze_schedule(NULL);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is NULL.", err);
  free(err);
}

void test_analyze_schedule_failed_parse_error_message(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-06-03 00:17] fallaaaaawers asleep\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Failed to match action in "
                           "\"[1518-06-03 00:17] fallaaaaawers asleep\".", err);
  free(err);
  free_tok(tok);
}

void test_analyze_schedule_single_guard_single_shift_guardcount(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(1u,res->n_guards);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_single_guard_single_shift_correct_id(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(10u,res->a_guards[0].id);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_single_guard_single_shift_sched_not_null(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_NOT_NULL(res->schedule);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_single_guard_single_shift_sum_minutes_asleep(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(45u,res->a_guards[0].total_minutes_asleep);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_single_guard_single_shift_minutes_asleep(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  unsigned exp[60] = {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                      0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                      0,0,0,0};
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT_ARRAY(exp,res->a_guards[0].minutes_asleep,60);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_single_guard_multi_shift_sum_minutes_asleep(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:10] Guard #10 begins shift\n"
    "[1518-11-03 00:13] falls asleep\n"
    "[1518-11-03 00:29] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-03 00:26] wakes up\n"
    "[1518-11-03 00:40] wakes up\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(69u,res->a_guards[0].total_minutes_asleep);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_single_guard_multi_shift_minutes_asleep(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:10] Guard #10 begins shift\n"
    "[1518-11-03 00:13] falls asleep\n"
    "[1518-11-03 00:29] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-03 00:26] wakes up\n"
    "[1518-11-03 00:40] wakes up\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-01 00:55] wakes up\n";
  unsigned exp[60] = {0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
                      0,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                      0,0,0,0};
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT_ARRAY(exp,res->a_guards[0].minutes_asleep,60);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_multi_guard_guardcount(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(3u,res->n_guards);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_multi_guard_correct_id(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(2u,res->a_guards[0].id);
  TEST_ASSERT_EQUAL_UINT(10u,res->a_guards[1].id);
  TEST_ASSERT_EQUAL_UINT(167u,res->a_guards[2].id);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_multi_guard_sum_minutes_asleep(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT(15u,res->a_guards[0].total_minutes_asleep);
  TEST_ASSERT_EQUAL_UINT(79u,res->a_guards[1].total_minutes_asleep);
  TEST_ASSERT_EQUAL_UINT(7u,res->a_guards[2].total_minutes_asleep);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_analyze_schedule_multi_guard_minutes_asleep(void){
  char in[] = "[1518-11-01 00:05] falls asleep\n"
    "[1518-11-01 00:30] falls asleep\n"
    "[1518-11-03 00:22] falls asleep\n"
    "[1518-11-01 00:00] Guard #10 begins shift\n"
    "[1518-11-02 00:47] falls asleep\n"
    "[1518-11-01 00:25] wakes up\n"
    "[1518-11-03 00:39] falls asleep\n"
    "[1518-11-02 00:18] wakes up\n"
    "[1518-11-03 00:02] Guard #167 begins shift\n"
    "[1518-11-04 00:02] Guard #10 begins shift\n"
    "[1518-11-04 00:07] falls asleep\n"
    "[1518-11-04 00:41] wakes up\n"
    "[1518-11-01 00:55] wakes up\n"
    "[1518-11-03 00:33] wakes up\n"
    "[1518-11-02 00:07] falls asleep\n"
    "[1518-11-03 00:31] falls asleep\n"
    "[1518-11-01 23:56] Guard #2 begins shift\n"
    "[1518-11-03 00:41] wakes up\n"
    "[1518-11-03 00:25] wakes up\n"
    "[1518-11-02 00:51] wakes up\n";
  unsigned exp[3][60] = {{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,
                          0,0,0,0,0,0},
                         {0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,
                          1,1,1,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
                          1,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,
                          0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0}
  };
  tok_t* tok = get_tokenizer(in, "\n");
  analyzed_sched_t* res = analyze_schedule(tok);
  TEST_ASSERT_NOT_NULL(res);
  TEST_ASSERT_EQUAL_UINT_ARRAY(exp[0],res->a_guards[0].minutes_asleep,60);
  TEST_ASSERT_EQUAL_UINT_ARRAY(exp[1],res->a_guards[1].minutes_asleep,60);
  TEST_ASSERT_EQUAL_UINT_ARRAY(exp[2],res->a_guards[2].minutes_asleep,60);
  free_analyzed_sched(res);
  free_tok(tok);
}

void test_part1_tok_null_returns_null(void){
  char* res = most_asleep_guard(NULL);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is NULL.", err);
  free(err);
}

void test_part1_empty_tok_returns_null(void){
  char in[] = "\n";
  tok_t* tok = get_tokenizer(in, "\n");
  char* res = most_asleep_guard(tok);
  TEST_ASSERT_NULL(res);
  char* err = get_latest_aoc_err_msg();
  TEST_ASSERT_EQUAL_STRING("Tokenizer is empty.", err);
  free(err);
  free_tok(tok);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_parse_entry_null_string_returns_error);
  RUN_TEST(test_parse_entry_null_en_returns_error);
  RUN_TEST(test_parse_entry_malformed_time_sets_error);
  RUN_TEST(test_parse_entry_regex_error_sets_error_message);
  RUN_TEST(test_parse_entry_parses_shift_start_event_correctly);
  RUN_TEST(test_parse_entry_parses_falling_asleep_event_correctly);
  RUN_TEST(test_parse_entry_parses_waking_up_event_correctly);
  RUN_TEST(test_parse_sched_tok_null_returns_null);
  RUN_TEST(test_parse_sched_single_guard_counts_correct);
  RUN_TEST(test_parse_sched_single_guard_id_correct);
  RUN_TEST(test_parse_sched_single_guard_correct_entries);
  RUN_TEST(test_parse_sched_single_guard_sorted_by_time);
  RUN_TEST(test_parse_sched_multi_guard_counts_correct);
  RUN_TEST(test_parse_sched_multi_guard_id_correct);
  RUN_TEST(test_parse_sched_multi_guard_correct_entries);
  RUN_TEST(test_parse_sched_multi_guard_sorted_by_time);
  RUN_TEST(test_parse_schedule_failed_parse_error_message);
  RUN_TEST(test_analyze_sched_tok_null_returns_null);
  RUN_TEST(test_analyze_schedule_failed_parse_error_message);
  RUN_TEST(test_analyze_schedule_single_guard_single_shift_guardcount);
  RUN_TEST(test_analyze_schedule_single_guard_single_shift_correct_id);
  RUN_TEST(test_analyze_schedule_single_guard_single_shift_sched_not_null);
  RUN_TEST(test_analyze_schedule_single_guard_single_shift_sum_minutes_asleep);
  RUN_TEST(test_analyze_schedule_single_guard_single_shift_minutes_asleep);
  RUN_TEST(test_analyze_schedule_single_guard_multi_shift_sum_minutes_asleep);
  RUN_TEST(test_analyze_schedule_single_guard_multi_shift_minutes_asleep);
  RUN_TEST(test_analyze_schedule_multi_guard_guardcount);
  RUN_TEST(test_analyze_schedule_multi_guard_correct_id);
  RUN_TEST(test_analyze_schedule_multi_guard_sum_minutes_asleep);
  RUN_TEST(test_analyze_schedule_multi_guard_minutes_asleep);
  RUN_TEST(test_part1_tok_null_returns_null);
  RUN_TEST(test_part1_empty_tok_returns_null);
  return UNITY_END();
}
