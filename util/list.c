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