#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "hcompress.h"

typedef struct node {
  tnode* value;
  struct node* next;
} LinkedList;

LinkedList* llCreate();

int llIsEmpty(LinkedList* ll);

void llDisplay(LinkedList* ll);
tnode* removeFirst(LinkedList** ll);
//void llAdd(LinkedList** ll, tnode* newValue);

void llFree(LinkedList* ll);

void add_in_order(LinkedList** ll, tnode* newValue);

#endif
