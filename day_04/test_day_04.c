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

entry_t* multi_guard_expected_schedule(){
  entry_t* exp = malloc(17*sizeof(entry_t));
  exp[0] = (entry_t){(struct tm){0,5,0,1,10,-382,0,0,0,0,0},10,ASLEEP};
  exp[1] = (entry_t){(struct tm){0,30,0,1,10,-382,0,0,0,0,0},10,ASLEEP};
  exp[2] = (entry_t){(struct tm){0,22,0,3,10,-382,0,0,0,0,0},167,ASLEEP};
  exp[3] = (entry_t){(struct tm){0,0,0,1,10,-382,0,0,0,0,0},10,START};
  exp[4] = (entry_t){(struct tm){0,47,0,2,10,-382,0,0,0,0,0},2,ASLEEP};
  exp[5] = (entry_t){(struct tm){0,25,0,1,10,-382,0,0,0,0,0},10,AWAKE};
  exp[6] = (entry_t){(struct tm){0,39,0,3,10,-382,0,0,0,0,0},167,ASLEEP};
  exp[7] = (entry_t){(struct tm){0,18,0,2,10,-382,0,0,0,0,0},2,AWAKE};
  exp[8] = (entry_t){(struct tm){0,2,0,3,10,-382,0,0,0,0,0},167,START};
  exp[9] = (entry_t){(struct tm){0,55,0,1,10,-382,0,0,0,0,0},10,AWAKE};
  exp[10] = (entry_t){(struct tm){0,33,0,3,10,-382,0,0,0,0,0},167,AWAKE};
  exp[11] = (entry_t){(struct tm){0,7,0,2,10,-382,0,0,0,0,0},2,ASLEEP};
  exp[12] = (entry_t){(struct tm){0,31,0,3,10,-382,0,0,0,0,0},167,ASLEEP};
  exp[13] = (entry_t){(struct tm){0,56,23,1,10,-382,0,0,0,0,0},2,START};
  exp[14] = (entry_t){(struct tm){0,41,0,3,10,-382,0,0,0,0,0},167,AWAKE};
  exp[15] = (entry_t){(struct tm){0,25,0,3,10,-382,0,0,0,0,0},167,AWAKE};
  exp[16] = (entry_t){(struct tm){0,51,0,2,10,-382,0,0,0,0,0},2,AWAKE};
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
  TEST_ASSERT_EQUAL_UINT(17,res->entrycount);
  TEST_ASSERT_EQUAL_UINT(3,res->guardcount);
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
  for(int i=0; i<17; i++){
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
  check_entry(exp+9, res->schedule[4]);
  check_entry(exp+13, res->schedule[5]);
  check_entry(exp+11, res->schedule[6]);
  check_entry(exp+7, res->schedule[7]);
  check_entry(exp+4, res->schedule[8]);
  check_entry(exp+16, res->schedule[9]);
  check_entry(exp+8, res->schedule[10]);
  check_entry(exp+2, res->schedule[11]);
  check_entry(exp+15, res->schedule[12]);
  check_entry(exp+12, res->schedule[13]);
  check_entry(exp+10, res->schedule[14]);
  check_entry(exp+6, res->schedule[15]);
  check_entry(exp+14, res->schedule[16]);
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
  free_sched(res);
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
  RUN_TEST(test_parse_sched_single_guard_correct_entries);
  RUN_TEST(test_parse_sched_single_guard_sorted_by_time);
  RUN_TEST(test_parse_sched_multi_guard_counts_correct);
  RUN_TEST(test_parse_sched_multi_guard_correct_entries);
  RUN_TEST(test_parse_sched_multi_guard_sorted_by_time);
  RUN_TEST(test_parse_schedule_failed_parse_error_message);
  RUN_TEST(test_part1_tok_null_returns_null);
  RUN_TEST(test_part1_empty_tok_returns_null);
  return UNITY_END();
}
