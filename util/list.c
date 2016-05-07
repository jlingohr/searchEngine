#include "list.h"

/*************************
* WebPage List wrappers 
*************************/

/*
* listAddPage - Wrapper function to add WebPage to a list
* @list: List adding to
* @page: element to add to list
*/
void listAddPage(List* list, WebPage* page) {
  page->ref_count = 2;
  listAdd(list, page);
}

/*
* listRemovePage - Remove first WebPage in list
* @list: list removing from
*
* Returns pointer to removed page
*/
WebPage* listRemovePage(List* list) {
  ListNode* node;
  WebPage* page;

  node = listRemove(list);
  if (node == NULL)
    return NULL;
  page = (WebPage*)node->data;
  node = NULL;
  dec_ref(page);
  return page;


  /*
  WebPage* page = (WebPage*) listRemove(list);
  dec_ref(page);
  return page;*/
}

/*
* inc_ref - Manage reference counts to urls
*/
void inc_ref(WebPage* page) {
  /* Refactor */
  page->ref_count += 1;
}

/*
* dec_ref - Manage decrementing references
* If no more references, free the url and page
*/
void dec_ref(WebPage* page) {
  /* Refactor */
  page->ref_count -= 1;
  if (page->ref_count == 0) {
    free(page->url);
    free(page);
  }
}

/*************************
* DocumentNode List wrappers 
*************************/

/*
* listAddDoc - Adds dNode to the end of the list
* @list: List adding to
* @dNode: Node adding
*/
void listAddDoc(List* list, DocumentNode* dNode) {
  listAdd(list, dNode);
}

/*
* listGetLastDNode - Get last DocumentNode in a list
* @list: list operating on
*
* Returns last DocumentNode pointer
*/
DocumentNode* listGetLastDNode(List* list) {
  return (DocumentNode*)listGetLast(list);
}



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
* listForEach - Execute function f for each element of list list
* @f: function to call
* list: List to iterate through
*/
void listForEach(void (*f)(element_t), List* list) {
  ListNode* cur = list->head;
  while (cur) {
    // TODO
  }
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
    //printf("%s", tmp);
  }
  //sprintf(*v, " %d ", a->len);
  f((element_t*)v, (element_t)tmp);
  strcat(*v, "\n");
  free(tmp);
  free(dNode_buf);


}