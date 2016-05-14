#include "list.h"



/*************************
* Generic List Macros 
*************************/

/*
* initList - Initializes singly-linked list
*/
List* initList() {
  List* list = malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
  return list;
}

/*
* listAdd - Add webpage to end of list
*/
void listAdd(List* list, element_t elem) {
  ListNode* tmp;

  tmp = malloc(sizeof(ListNode));
  tmp->data = elem;
  //elem->ref_count = 2;

  if (list->head == NULL) { /* List is empty */
    list->head = list->tail = tmp;
    tmp->next = NULL;
  }
  else { /* Append to end of list */
    list->tail->next = tmp;
    list->tail = tmp;
    tmp->next = NULL;
  }
  list->len++;
}

/*
* listRemove - Remove node from front of list
* Return pointer data of tail node
*/
ListNode* listRemove(List* list) {
  //element_t elem;
  ListNode* tmp;

  if (list->head == NULL)
    return NULL;

  tmp = list->head;
  list->head = tmp->next;
  list->len--;
  return tmp;
}

/*
* listGetLast - Get tail node of a list
* @list: list operating on
*
* Returns pointer to last node
*/
element_t listGetLast(List* list) {
  return list->tail->data;
}


/*
* listDelete - Delete the list
*/
void listDelete(void (*f)(element_t), List* list) {
  ListNode* cur;

  while ((cur = listRemove(list))) {
    f(cur->data);
    free(cur);
  }
}

/*
* listIsEmpty - Returns 1 if list is empty. 0 otherwise
*/
int listIsEmpty(List* list) {
  if (list->head)
    return 0;
  return 1;
}

/* 
* listGet - Returns a pointer to elem if in list
* Otherwise returns NULL
*/
ListNode* listGet(List* list, element_t elem, int (*f)(element_t, element_t)) {
  ListNode* cur = list->head;

  while (cur) {
    if (f(elem, cur->data))
      return cur;
    cur = cur->next;
  }
  return NULL;
}

/* 
* listFoldl - Fold list using function f placing result in out
* @f: function to fold on
* @out: pointer to output variable
* @list: List folding on
*/
void listFoldString(void (*f) (element_t*, element_t), char** v, List* a) {
  /* TODO - REDO sloppy */
  char* tmp = malloc(MAXLINE);
  char* dNode_buf = malloc(MAXLINE);
  ListNode* cur = a->head;
  while (cur) {
    DocumentNode* dNode = (DocumentNode*)cur->data;
    sprintf(dNode_buf, "%d %d ", dNode->document_id, dNode->page_word_frequency);
    cur = cur->next;
    strcat(tmp, dNode_buf);
  }
  f((element_t*)v, (element_t)tmp);
  //strcat(*v, "\n");
  free(tmp);
  free(dNode_buf);


}

/*
* listForEach - Iteratively move through list and call procedure
* f on each listNode
*/
void listForEach(void (*f)(element_t), List* list) {
  ListNode* cur = list->head;

  while (cur) {
    f(cur);
    cur = cur->next;
  }
}


/*
* MergeSort - Sort a linked-list using merge sort
*/
void MergeSort(List* list, int len, int (*f)(element_t, element_t)) {
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
  /* TODO - Watch dangling pointers and mem leaks! */
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