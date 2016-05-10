#ifndef _LISTWPAGE_H
#define _LISTWPAGE_H

#include "../../util/list.h"




/* WebPage list wrappers */
WebPage* listRemovePage(List* list);
void listAddPage(List* list, WebPage* page);
void inc_ref(WebPage* page);
void dec_ref(WebPage* page);

#endif