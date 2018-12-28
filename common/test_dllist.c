/**
 * @file test_dllist.c
 * @brief UTs for Double Linked List
 */

#include "dllist.h"

#include <unity.h>

#include <stddef.h>

void test_init_list_returns_non_null(void){
  dllist_t* l = init_list();
  TEST_ASSERT_NOT_NULL(l);
  free_dllist(l);
}

void test_empty_list_append_new_node_is_tail(void){
  int content = 20;
  dllist_t* l = init_list();
  node_t* new_node = append(l, &content);
  TEST_ASSERT_EQUAL_PTR(new_node, tail(l));
  free_dllist(l);
}

void test_empty_list_append_new_node_is_head(void){
  int content = 20;
  dllist_t* l = init_list();
  node_t* new_node = append(l, &content);
  TEST_ASSERT_EQUAL_PTR(new_node, head(l));
  free_dllist(l);
}

void test_empty_list_tail_is_null(void){
  dllist_t* l = init_list();
  TEST_ASSERT_EQUAL_PTR(NULL, tail(l));
  free_dllist(l);
}

void test_empty_list_head_is_null(void){
  dllist_t* l = init_list();
  TEST_ASSERT_EQUAL_PTR(NULL, head(l));
  free_dllist(l);
}

void test_single_entry_head_equals_tail(void){
  int content = 20;
  dllist_t* l = init_list();
  append(l, &content);
  TEST_ASSERT_EQUAL_PTR(tail(l), head(l));
  free_dllist(l);
}

void test_single_entry_remove_node_returns_null(void){
  int content = 20;
  dllist_t* l = init_list();
  node_t* new_node = append(l, &content);
  node_t* res = remove_node(l, new_node);
  TEST_ASSERT_EQUAL_PTR(NULL, res);
  free_dllist(l);
}

void test_single_entry_remove_node_head_null(void){
  int content = 20;
  dllist_t* l = init_list();
  node_t* new_node = append(l, &content);
  remove_node(l, new_node);
  TEST_ASSERT_EQUAL_PTR(NULL, head(l));
  free_dllist(l);
}

void test_single_entry_remove_node_tail_null(void){
  int content = 20;
  dllist_t* l = init_list();
  node_t* new_node = append(l, &content);
  remove_node(l, new_node);
  TEST_ASSERT_EQUAL_PTR(NULL, tail(l));
  free_dllist(l);
}

void test_single_entry_append_new_node_is_tail(void){
  int content = 20;
  dllist_t* l = init_list();
  append(l, &content);
  node_t* new_node = append(l, &content);
  TEST_ASSERT_EQUAL_PTR(new_node, tail(l));
  free_dllist(l);
}

void test_single_entry_appended_node_is_next_for_head(void){
  int content = 20;
  dllist_t* l = init_list();
  append(l, &content);
  node_t* new_node = append(l, &content);
  TEST_ASSERT_EQUAL_PTR(new_node, next(head(l)));
  free_dllist(l);
}

void test_single_entry_appended_prev_returns_head(void){
  int content = 20;
  dllist_t* l = init_list();
  append(l, &content);
  node_t* new_node = append(l, &content);
  TEST_ASSERT_EQUAL_PTR(head(l), prev(new_node));
  free_dllist(l);
}

void test_single_entry_prev_on_head_is_null(void){
  int content = 20;
  dllist_t* l = init_list();
  append(l, &content);
  TEST_ASSERT_EQUAL_PTR(NULL, prev(head(l)));
  free_dllist(l);
}

void test_single_entry_next_on_tail_is_null(void){
  int content = 20;
  dllist_t* l = init_list();
  append(l, &content);
  TEST_ASSERT_EQUAL_PTR(NULL, next(tail(l)));
  free_dllist(l);
}

void test_multi_remove_tail_predecessor_new_tail(void){
  dllist_t* l = init_list();
  int content1 = 20;
  append(l, &content1);
  int content2 = 30;
  node_t* node2 = append(l, &content2);
  int content3 = 40;
  append(l, &content3);
  remove_node(l, tail(l));
  TEST_ASSERT_EQUAL_PTR(node2, tail(l));
  free_dllist(l);
}

void test_multi_remove_head_successor_new_head(void){
  dllist_t* l = init_list();
  int content1 = 20;
  append(l, &content1);
  int content2 = 30;
  node_t* node2 = append(l, &content2);
  int content3 = 40;
  append(l, &content3);
  remove_node(l, head(l));
  TEST_ASSERT_EQUAL_PTR(node2, head(l));
  free_dllist(l);
}

void test_multi_next_on_head_iterates_whole_list(void){
  dllist_t* l = init_list();
  int content1 = 20;
  append(l, &content1);
  int content2 = 30;
  append(l, &content2);
  int content3 = 40;
  append(l, &content3);
  node_t* curr = head(l);
  TEST_ASSERT_EQUAL_INT(content1, *((int*)data(curr)));
  curr = next(curr);
  TEST_ASSERT_EQUAL_INT(content2, *((int*)data(curr)));
  curr = next(curr);
  TEST_ASSERT_EQUAL_INT(content3, *((int*)data(curr)));
  curr = next(curr);
  TEST_ASSERT_NULL(curr);
  free_dllist(l);
}

void test_multi_prev_on_tail_iterates_whole_list(void){
  dllist_t* l = init_list();
  int content1 = 20;
  append(l, &content1);
  int content2 = 30;
  append(l, &content2);
  int content3 = 40;
  append(l, &content3);
  node_t* curr = tail(l);
  TEST_ASSERT_EQUAL_INT(content3, *((int*)data(curr)));
  curr = prev(curr);
  TEST_ASSERT_EQUAL_INT(content2, *((int*)data(curr)));
  curr = prev(curr);
  TEST_ASSERT_EQUAL_INT(content1, *((int*)data(curr)));
  curr = prev(curr);
  TEST_ASSERT_NULL(curr);
  free_dllist(l);
}

void test_multi_remove_tail_returns_null(void){
  dllist_t* l = init_list();
  int content1 = 20;
  append(l, &content1);
  int content2 = 30;
  append(l, &content2);
  int content3 = 40;
  append(l, &content3);
  TEST_ASSERT_NULL(remove_node(l, tail(l)));
  free_dllist(l);
}

void test_multi_remove_middle_returns_next_node(void){
  dllist_t* l = init_list();
  int content1 = 20;
  append(l, &content1);
  int content2 = 30;
  node_t* node2 = append(l, &content2);
  int content3 = 40;
  node_t* node3 = append(l, &content3);
  TEST_ASSERT_EQUAL_PTR(remove_node(l, node2), node3);
  free_dllist(l);
}

void test_multi_remove_middle_relinks_next_and_prev(void){
  dllist_t* l = init_list();
  int content1 = 20;
  append(l, &content1);
  int content2 = 30;
  node_t* node2 = append(l, &content2);
  int content3 = 40;
  append(l, &content3);
  remove_node(l, node2);
  node_t* curr = head(l);
  TEST_ASSERT_EQUAL_INT(content1, *((int*)data(curr)));
  curr = next(curr);
  TEST_ASSERT_EQUAL_INT(content3, *((int*)data(curr)));
  curr = next(curr);
  TEST_ASSERT_NULL(curr);
  free_dllist(l);
}

void test_data_returns_pointer_to_content(void){
  dllist_t* l = init_list();
  int content = 20;
  node_t* node = append(l, &content);
  TEST_ASSERT_EQUAL_INT(content, *((int*)data(node)));
  free_dllist(l);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_init_list_returns_non_null);
  RUN_TEST(test_empty_list_append_new_node_is_tail);
  RUN_TEST(test_empty_list_append_new_node_is_head);
  RUN_TEST(test_empty_list_tail_is_null);
  RUN_TEST(test_empty_list_head_is_null);
  RUN_TEST(test_data_returns_pointer_to_content);
  RUN_TEST(test_single_entry_head_equals_tail);
  RUN_TEST(test_single_entry_remove_node_head_null);
  RUN_TEST(test_single_entry_remove_node_tail_null);
  RUN_TEST(test_single_entry_remove_node_returns_null);
  RUN_TEST(test_single_entry_append_new_node_is_tail);
  RUN_TEST(test_single_entry_appended_node_is_next_for_head);
  RUN_TEST(test_single_entry_appended_prev_returns_head);
  RUN_TEST(test_single_entry_prev_on_head_is_null);
  RUN_TEST(test_single_entry_next_on_tail_is_null);
  RUN_TEST(test_multi_remove_tail_predecessor_new_tail);
  RUN_TEST(test_multi_remove_head_successor_new_head);
  RUN_TEST(test_multi_next_on_head_iterates_whole_list);
  RUN_TEST(test_multi_prev_on_tail_iterates_whole_list);
  RUN_TEST(test_multi_remove_tail_returns_null);
  RUN_TEST(test_multi_remove_middle_returns_next_node);
  RUN_TEST(test_multi_remove_middle_relinks_next_and_prev);
  return UNITY_END();
}
