
/* ========================================================================== */
/* File: list.h
 *
 * Component name: Crawler
 *
 * This file contains the definitions for a doubly-linked list of WebPages.
 *
 */
/* ========================================================================== */
#ifndef LIST_H
#define LIST_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "../../util/common.h"                          // common functionality
 

// ---------------- Constants

// ---------------- Structures/Types

typedef struct ListNode {
    WebPage *page;                           // the data for a given page
    struct ListNode *prev;                   // pointer to previous node
    struct ListNode *next;                   // pointer to next node
} ListNode;

typedef struct List {
    ListNode *head;                          // "beginning" of the list
    ListNode *tail;                          // "end" of the list
} List;

// ---------------- Public Variables

extern List toVisit;

// ---------------- Prototypes/Macros
void listAdd(WebPage *page);
int initList();
WebPage* listRemove();
void inc_ref(WebPage* page);
void dec_ref(WebPage* page);

#endif // LIST_H