/**
 * @file dllist.c
 * @brief Implementation of Double Linked List for AoC
 */

#include "dllist.h"

#include <stddef.h>
#include <stdlib.h>

struct node{
  void* data;
  node_t* next;
  node_t* prev;
};

struct dllist{
  node_t* head;
  node_t* tail;
};

dllist_t* init_list(){
  dllist_t* newlist = malloc(sizeof(dllist_t));
  newlist->tail = NULL;
  newlist->head = NULL;
  return newlist;
}

void free_dllist(dllist_t* l){
  node_t* curr = head(l);
  node_t* prev = NULL;
  while(curr != NULL){
    prev = curr;
    curr = next(curr);
    free(prev);
  }
  free(l);
}

void* data(const node_t* n){
  if(n == NULL){
    return NULL;
  }
  return n->data;
}

node_t* append(dllist_t* l, void* data){
  if(l == NULL){
    return NULL;
  }
  node_t* newnode = malloc(sizeof(node_t));
  newnode->data = data;
  newnode->next = NULL;
  if(l->head == NULL){
    l->head = newnode;
    newnode->prev = NULL;
  }
  else{
    newnode->prev = l->tail;
    l->tail->next = newnode;
  }
  l->tail = newnode;
  return newnode;
}

node_t* remove_node(dllist_t* l, node_t* n){
  if(n == NULL || l == NULL){
    return NULL;
  }
  node_t* predecessor = n->prev;
  node_t* successor = n->next;
  if(predecessor != NULL){
    predecessor->next = successor;
  }
  if(successor != NULL){
    successor->prev = predecessor;
  }
  if(tail(l) == n){
    l->tail = predecessor;
  }
  if(head(l) == n){
    l->head = successor;
  }
  free(n);
  return successor;
}

node_t* next(node_t* n){
  if(n == NULL){
    return NULL;
  }
  return n->next;
}

node_t* prev(node_t* n){
  if(n == NULL){
    return NULL;
  }
  return n->prev;
}

node_t* head(dllist_t* l){
  return l->head;
}

node_t* tail(dllist_t* l){
  return l->tail;
}
