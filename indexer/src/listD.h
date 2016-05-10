#ifndef _LISTD_H
#define _LISTD_H

#include "../../util/list.h"

/* DocumentNode wrappers */
void listAddDoc(List* list, DocumentNode* dNode);
DocumentNode* listGetLastDNode(List* list);
void listDeleteDNode(List* list);
void deleteDNode(element_t dNode);


#endif