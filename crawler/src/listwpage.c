#include "listwpage.h"

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
