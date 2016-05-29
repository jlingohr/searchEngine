#include "list.h"



/*************************
* Generic List Macros 
*************************/

/*
* list_new - Creates an empty list
* with initial size of 0
* @list: List to initialize
* @elementSize - Size of data stored in the list
* @cmp: Function used to compare elements for search
* @freeFn - function to free list later
*/
void list_new(List* list, int elementSize, list_compare cmp, freeFunction freeFn) {
  assert(elementSize > 0);
  assert(cmp != NULL);
  list->length = 0;
  list->elementSize = elementSize;
  list->head = list->tail = NULL;
  list->compare = cmp;
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

  if (list->head == NULL) { // Empty list
    list->head = list->tail = node;
    node->next = NULL;
  } else {
    node->next = list->head;
    list->head = node;
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

  if (list->head == NULL) { // Empty list
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
    list_head(list, elem);
    ListNode* node = list->head;
    list->head = node->next;
    

    free(node->data);
    free(node);

    list->length--;
    return 1;
  }
}

/*
* list_get - Retrive object with match key and points elem to it,
* Returns 1 on success, 0 otherwise
*/
int list_get(List* list, element_t key, element_t* elem)
{
  // TODO - should it remove element?
  ListNode* node = NULL;
  ListNode* cur = list->head;
  while (cur) {
    if (list->compare(key, cur->data)) {
      *elem = cur->data;
      return 1;
    }
    node = cur;
    cur = node->next;
  }
  return 0;
}


/*
* list_size - Returns the size of the List list
*/
int list_size(List* list) {
  return list->length;
}

/**
 * Fold in_list using function f placing result in out_element_p.
 * For f(out,in):
 *    out is pointer to the accumulator element (i.e., the value of out_element_p)
 *        if *out is NULL then f should allocate a new element and set *out to point to it
 *    in  is an element from in_list
 */
void list_foldl(void (*f) (element_t*, element_t, element_t), element_t* out_element_p, List* list)
{
  ListNode* node = list->head;
  while (node) {
    f(out_element_p, *out_element_p, node->data);
    node = node->next;
  }
}

/*****************
* List Algorithms
*****************/

/*
* MergeSort - Sort a linked-list using merge sort
*/
void MergeSort(List* list, int len, int (*f)(element_t, element_t)) {
  // TODO
  if (len <= 1)
    return;

  List* left = initList();
  List* right = initList();
  int mid = len / 2;

  ListNode* cur = list->head;

  for (int i = 0; i < len; i++) {
    if (mid > 0)
      listAdd(left, cur);
    else
      listAdd(right, cur);
    mid--;
  }
  MergeSort(left, left->len, f);
  MergeSort(right, right->len, f);

  /* MEMORY LEAKS and DANGLING POINTERS */
  Merge(left, right, f);
}

/*
* Merge - Merges A and B returning a new list
*/
List* Merge(List* A, List* B, int (*f)(element_t, element_t)) {
  // TODO - Watch dangling pointers and mem leaks! 
  List* list = initList();
  ListNode* tmp;

  if (A == NULL)
    return B;
  if (B == NULL)
    return A;

  while (A->len > 0 || B->len > 0) {
    if (A->len > 0 && B->len > 0) {
      if (f(A, B) <= 0) {
        tmp = listRemove(A);
      }
      else {
        tmp = listRemove(B);
      }
      listAdd(list, tmp);
    }
    else if (A->len > 0) {
      tmp = listRemove(A);
      listAdd(list, tmp);
    }
    else if (B->len > 0) {
      tmp = listRemove(B);
      listAdd(list, tmp);
    }
  }
  return list;
}
