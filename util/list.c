#include "list.h"



/*************************
* Generic List Macros 
*************************/

/*
* list_new - Creates an empty list
* with initial size of 0
* @list: List to initialize
* @elementSize - Size of data stored in the list
* @freeFn - function to free list later
*/
void list_new(List* list, int elementSize, freeFunction freeFn) {
  assert(elementSize > 0);
  assert(freeFn != NULL);
  list->length = 0;
  list->elementSize = elementSize;
  list->head = list->tail = NULL;
  list->freeFn = freeFn;
}

/*
* list_destroy - Destroys the List list
* and frees mmory
*/
void list_destroy(List* list) {
  ListNode* cur;
  while (list->head != NULL) {
    cur = list->head;
    list->head = cur->next;

    if (list->freeFn) {
      list->freeFn(cur->data);
    }

    free(cur->data);
    free(cur);
  }
}

/*
* list_prepend - adds an element elem to the start
* of the List list
*/
void list_prepend(List* list, element_t elem) {
  ListNode* node = malloc(sizeof(ListNode));
  node->data = malloc(list->elementSize);
  memcpy(node->data, elem, list->elementSize);

  node->next = list->head;
  list->head = node;

  if (!list->tail) {
    list->tail = list->head;
  }

  list->length++;
}

/*
* list_append - Adds the element elem to the end
* of the List list
*/
void list_append(List* list, element_t elem) {
  ListNode* node = malloc(sizeof(ListNode));
  node->data = malloc(list->elementSize);
  node->next = NULL;

  memcpy(node->data, elem, list->elementSize);

  if (list->length == 0) {
    list->head = list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }

  list->length++;
}


/*
* list_for_each - Iterates through the List list
*/
void list_for_each(List* list, listIterator iterator) {
  assert(iterator != NULL);

  ListNode* node = list->head;
  bool result = TRUE;
  while (node != NULL && result) {
    result = iterator(node->data);
    node = node->next;
  }
}

/*
* list_head - Stores data in lists head in elem
*/
void list_head(List* list, element_t elem) {
  /* TODO - what about removal? */
  assert(list->head != NULL);

  ListNode* node = list->head;
  memcpy(elem, node->data, list->elementSize);

}

/*
* list_tail - Returns a pointer to the tail of
* the List list
*/
void list_tail(List* list, element_t elem) {
  assert(list->tail != NULL);
  ListNode* node = list->tail;
  memcpy(elem, node->data, list->elementSize);
}

/*
* list_dequeue - Copies head data into elem and remves
* from the list, setting the new head.
* Returns 1 if successfully dequeued
* Else returns 0
*/
int list_dequeue(List* list, element_t elem) {
  if (list->head == NULL) {
    return 0;
  } else {
    ListNode* node = list->head;
    memcpy(elem, node->data, list->elementSize);
    list->head = node->next;
    list->length--;

    free(node->data);
    free(node);

    return 1;
  }
}

/*
* list_size - Returns the size of the List list
*/
int list_size(List* list) {
  return list->length;
}

