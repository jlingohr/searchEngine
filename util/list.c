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

static void list_delete_node(List* list, ListNode* node)
{
  if (node == list->head) {
    list->head = node->next;
  }
  else {
    node->prev->next = node->next;
  }
  if (node == list->tail) {
    list->tail = node->prev;
  }
  else {
    node->next->prev = node->prev;
  }
  node->next = node->prev = NULL;
  list_dec_ref(list, node);
  list_dec_ref(list, node);
}

/*
* list_destroy - Destroys the List list
* and frees mmory
*/
void list_destroy(List* list) {
  /*ListNode* cur;
  while (list->head != NULL) {
    cur = list->head;
    list->head = list->head->next;

    if (list->freeFn) {
      list->freeFn(cur->data);
      cur->data = NULL;
    }
    cur->data = NULL;
    cur->next = NULL;
    free(cur);
    cur = NULL;
  }*/

  for (ListNode* node = list->head, *n; node != NULL; node = n) {
    n = node->next;
    list_delete_node(list, node);
  }
  list->length = 0;
  list->elementSize = 0;
  free(list);
  list = NULL;
}

/*
* list_prepend - adds an element elem to the start
* of the List list
*/
void list_prepend(List* list, element_t elem) {
  ListNode* node = calloc(1, sizeof(ListNode));
  node->data = elem;
  node->ref_count = 2;
  node->prev = node->next = NULL;

  if (list->head == NULL) { // Empty list
    list->tail = node;
  } else {
    //node->next = list->head;
    //list->head = node;
    list->head->next = node;
    node->next = list->head;
  }
  list->head = node;
  list->length++;
}

/*
* list_append - Adds the element elem to the end
* of the List list
*/
void list_append(List* list, element_t elem) {
  ListNode* node = calloc(1, sizeof(ListNode));
  node->data = elem;
  node->ref_count = 2;
  node->next = node->prev = NULL;

  if (list->head == NULL) { // Empty list
    list->head = node;
  } else {
    list->tail->next = node;
    node->prev = list->tail;
  }
  list->tail = node;
  list->length++;
}


/*
* list_for_each - Iterates through the List list
*
void list_for_each(List* list, listIterator iterator) {
  assert(iterator != NULL);

  ListNode* node = list->head;
  bool result = TRUE;
  while (node != NULL && result) {
    result = iterator(node->data);
    node = node->next;
  }
}*/

/*
* list_head - Stores data in lists head in elem

void list_head(List* list, element_t elem) {
  assert(list->head != NULL);

  ListNode* node = list->head;
  memcpy(elem, node->data, list->elementSize);

}
*/
/*
* list_tail - Returns a pointer to the tail of
* the List list

void list_tail(List* list, element_t elem) {
  assert(list->tail != NULL);
  ListNode* node = list->tail;
  memcpy(elem, node->data, list->elementSize);
}
*/
/*
* list_dequeue - Copies head data into elem and remves
* from the list, setting the new head.
* Returns 1 if successfully dequeued
* Else returns 0
*/
element_t list_dequeue(List* list) {
  element_t result = NULL;

  if (list->head == NULL) {
    return result;
  } else {
    ListNode* tmp = list->head;
    list->head = list->head->next;
    result = tmp->data;
    tmp->next = NULL;
    tmp->data = NULL;
    free(tmp);
    list->length--;
    return result;
  }
}

/*
* list_get - Retrive object with match key and points elem to it,
* Returns 1 on success, 0 otherwise
*/
int list_get(List* list, element_t key, element_t* elem)
{
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
  *elem = NULL;
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


void list_inc_ref(ListNode* node)
{
  node->ref_count += 1;
}

void list_dec_ref(List* list, ListNode* node)
{
  node->ref_count = 1;
  if (node->ref_count == 0) {
    if (list->freeFn)
      list->freeFn(node->data);
    free(node);
  }
}

/*****************
* List Algorithms
*****************/

/*
* MergeSort - Sort a linked-list using merge sort
*/
List* MergeSort(List* list, int len, int (*f)(element_t, element_t)) {
  // TODO - Careful wih dangling pointers
  if (len <= 1)
    return list;

  List* left = calloc(1, sizeof(List));
  List* right = calloc(1, sizeof(List));
  list_new(left, list->elementSize, list->compare, list->freeFn);
  list_new(right, list->elementSize, list->compare, list->freeFn);

  int mid = len / 2;
  //ListNode* cur;
  element_t tmp;
  while (list->length) {
    tmp = list_dequeue(list);
    if (mid > 0) {
      list_append(left, tmp);
    } else {
      list_append(right, tmp);
    }
    mid--;
  }
  List* sort_left = MergeSort(left, left->length, f);
  List* sort_right = MergeSort(right, right->length, f);

  if (sort_left != left) list_destroy(left);
  if (sort_right != right) list_destroy(right);

  // MEMORY LEAKS and DANGLING POINTERS
  return Merge(sort_left, sort_right, f);
}

/*
* Merge - Merges A and B returning a new list
*/
List* Merge(List* A, List* B, int (*f)(element_t, element_t)) {
  // TODO - Watch dangling pointers and mem leaks! 
  List* list = calloc(1, sizeof(List));
  list_new(list, A->elementSize, A->compare, A->freeFn);

  element_t tmp = NULL;
  while (list_length(A) > 0 || list_length(B) > 0) {
    if (list_length(A) > 0 && list_length(B) > 0) {
      if (f(list_head(A), list_head(B)) <= 0) {
        tmp = list_dequeue(A);
      }
      else {
        tmp = list_dequeue(B);
      }
      list_append(list, tmp);
    }
    else if (list_length(A) > 0) {
      tmp = list_dequeue(A);
      list_append(list, tmp);
    }
    else if (list_length(B) > 0) {
      tmp = list_dequeue(B);
      list_append(list, tmp);
    }
  }
  return list;
}


