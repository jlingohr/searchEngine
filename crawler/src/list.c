#include "list.h"

/*
* initList - Initializes singly-linked list
*/
int initList() {
  toVisit.head = NULL;
  toVisit.tail = NULL;
  return 1;
}

/*
* listAdd - Add webpage to end of list
*/
void listAdd(WebPage *page) {
  /* TODO - Add reference counting */
  ListNode* tmp;

  tmp = malloc(sizeof(ListNode));
  tmp->page = page;

  if (toVisit.head == NULL) {
    toVisit.head = toVisit.tail = tmp;
    tmp->prev = tmp->next = NULL;
  }
  else {
    tmp->prev = toVisit.tail;
    toVisit.tail = tmp;
    tmp->next = NULL;
  }
}

/*
* listRemove - Remove webpage from front of list
* Return pointer to the first webpage in the list
*/
WebPage* listRemove() {
  /* TODO - Add reference counting */
  WebPage* page;
  ListNode* tmp;

  if (toVisit.head == NULL)
    return NULL;

  tmp = toVisit.head;
  toVisit.head = tmp->next;
  page = tmp->page;
  tmp = NULL;
  return page;
}
