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
#include <assert.h>

// ---------------- Constants
#define INTERVAL_PER_FETCH 1                 // seconds between fetches

#define MAX_DEPTH 4                          // don't want to go too far do
#define MAX_URL_LENGTH 1000
#define MAXLINE 1000     
#define STATUS_LOG 0            // switch to one for output    
#define WORD_LENGTH 100
#define MAX_NUMBER_OF_SLOTS 10000  
#define BUF_SIZE 8192
 

// limit crawling to only this domain
#define URL_PREFIX "http://www.cs.dartmouth.edu/~cs50/"

// ---------------- Structures/Types

typedef enum {FALSE, TRUE} bool;

/* For working with generics */
typedef void* element_t;


/* Structure of a webpage */
typedef struct WebPage {
    char url[MAX_URL_LENGTH];                               // url of the page
    char *html;                              // html code of the page
    size_t html_len;                         // length of html code
    int depth;                               // depth of crawl
} WebPage;

/*
* Associated location for each word containing
*   1- page identifer
*   2- Number of occurences
*/
typedef struct DocumentNode {
  int document_id;              /* Document identifer */
  int page_word_frequency;      /* Number of occurrences of the word */
} DocumentNode;

/*
* Each word in a page is represented as a node
* in a list
*/

typedef struct WordNode {
  char word[WORD_LENGTH];       /* The word */
  struct List* page;           /* Pointer to first element of the page list */
} WordNode;

typedef void (*freeFunction)(element_t);




// ---------------- Public Variables

// ---------------- Prototypes/Macros


#endif // COMMON_H
