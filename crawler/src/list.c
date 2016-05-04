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
  page->ref_count = 2;

  if (toVisit.head == NULL) { /* List is empty */
    toVisit.head = toVisit.tail = tmp;
    tmp->prev = tmp->next = NULL;
  }
  else { /* List has at least 1 element */
    tmp->prev = toVisit.tail;
    toVisit.tail = toVisit.tail->next = tmp;
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
  dec_ref(page);
  return page;
}

/*
* inc_ref - Manage reference counts to urls
*/
void inc_ref(WebPage* page) {
  page->ref_count += 1;
}

/*
* dec_ref - Manage decrementing references
* If no more references, free the url and page
*/
void dec_ref(WebPage* page) {
  page->ref_count -= 1;
  if (page->ref_count == 0) {
    free(page->url);
    free(page);
  }
}
