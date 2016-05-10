#include "listD.h"

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

/*
* listDeleteDNode - Wrapper to delete list of DocumentNode
*/
void listDeleteDNode(List* list) {
  listDelete(deleteDNode, list);
}

/*
* deleteDNode - Helper to free DocumentNodes
*/
void deleteDNode(element_t dNode) {
  free(dNode);
}