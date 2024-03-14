#include "linkedList.h"
#include "hcompress.h"
#include <stdio.h>
#include <stdlib.h> // malloc

LinkedList* llCreate() {
  return NULL;
}
int llIsEmpty(LinkedList* ll) {
  return (ll == NULL);
}
void llDisplay(LinkedList* ll) {
  LinkedList* temp = ll;
  printf("[");

  while (temp != NULL) {
    printf("Value: %d. Frequency: %d\n", (temp->value->c), temp->value->fr);
    temp = temp->next;
  }
  printf("]\n");
}
tnode* removeFirst(LinkedList** ll){
  //point at the head of the list
  LinkedList* temp = *ll;
  if (temp == NULL){
    return NULL;
  }
  //get the tnode
  tnode* out = temp->value;
  //move the head to the next linkedlist node
  *ll = (*ll)->next;
  //free the linked list node that contained the tnode
  free(temp);
  //return the tnode 
  return out;
}
void add_in_order(LinkedList** ll, tnode* newValue)
{
  LinkedList* newNode = (LinkedList*) malloc(sizeof(LinkedList));
  //create the node
  newNode->value = newValue;
  newNode->next = NULL;

  LinkedList* temp = *ll;
  //case 1
  //it is empty
  if (temp == NULL){
    *ll = newNode;
  }
  //case 2:
  //new node frequency is less than the head
  else if(newNode->value->fr < temp->value->fr){
    *ll = newNode;
    newNode->next = temp;
  }
  //case3:
  //if newNode value is greater than the head but less than the next
  //or it's null
  else {
    while ((temp->next != NULL) &&
            newNode->value->fr  >temp->next->value->fr) {
              temp = temp->next;
    }
    if(temp->next == NULL){
      temp->next = newNode;
    }
    else{
      newNode->next = temp->next;
      temp->next = newNode;
    }
  }
}

void llFree(LinkedList* ll) {
  LinkedList* p = ll;
  while (p != NULL) {
    LinkedList* oldP = p;
    p = p->next;
    free(oldP);
  }
}
