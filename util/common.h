/* ========================================================================== */
/* File: common.h
 *
 * Component name: Crawler
 *
 * This file contains the common defines and data structures.
 *
 */
/* ========================================================================== */
#ifndef COMMON_H
#define COMMON_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stddef.h>                          // size_t
#include <stdio.h>
#include <stdlib.h>

// ---------------- Constants
#define INTERVAL_PER_FETCH 1                 // seconds between fetches

#define MAX_DEPTH 4                          // don't want to go too far do
#define MAX_URL_LENGTH 1000
#define MAXLINE 1000     
#define STATUS_LOG 1            // switch to one for output    
#define WORD_LENGTH 1000
#define MAX_NUMBER_OF_SLOTS 10000  
 

// limit crawling to only this domain
#define URL_PREFIX "http://www.cs.dartmouth.edu/~campbell/cs50/"

// ---------------- Structures/Types

/* For working with generics */
typedef void* element_t;

typedef struct WebPage {
    char *url;                               // url of the page
    char *html;                              // html code of the page
    size_t html_len;                         // length of html code
    int depth;                               // depth of crawl
    int ref_count;
} WebPage;


/*
* Associated location for each word containing
*   1- page identifer
*   2- Number of occurences
*/
typedef struct _DocumentNode {
  int document_id;              /* Document identifer */
  int page_word_frequency;      /* Number of occurrences of the word */
} DocumentNode;

/*
* Each word in a page is represented as a node
* in a list
*/

typedef struct _WordNode {
  char word[WORD_LENGTH];       /* The word */
  List* page;           /* Pointer to first element of the page list */
} WordNode;

/*
* Inverted list is a sorted list of locations where the term
* appears in the collection. The location consists of:
*   1- page identifier
*   2- Position of term in the page
*/
typedef struct _InvertedIndex {
  WordNode* start;              /* Start and end pointer of dynamic links */
  WordNode* end;                /* Start of the list */
  WordNode* hash[MAX_NUMBER_OF_SLOTS]; /* Hash slot */
} InvertedIndex;

// ---------------- Public Variables

// ---------------- Prototypes/Macros


#endif // COMMON_H