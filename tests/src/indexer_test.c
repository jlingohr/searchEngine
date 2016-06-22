/*
* indexer_test.c - Testing for indexer.c
*/

#include "../minunit.h"
#include "../../indexer/src/index.h"
#include "../../util/hashtable.h"
#include "../../util/list.h"
#include "../../util/web.h"
#include <curl/curl.h> 
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "../../util/common.h"

#define MAX_DOC 5

HashTable* Index = NULL;


char* dir = "./data_src/";
char* target_file = "./data_dest";

char* words[] = {"man", "bear", "pig"};

uint32_t hash = 0;
uint32_t hash1(element_t a)
{
  return hash;
}

WordNode* create_random_node()
{
  WordNode* wNode = calloc(1, sizeof(WordNode));
  int r = random();
  wNode->word = strdup(words[r % 2]);

  wNode->page = calloc(1, sizeof(List));
  list_new(wNode->page, sizeof(DocumentNode), dNode_cmp, NULL);

  DocumentNode* dNode = calloc(1, sizeof(DocumentNode));
  dNode->document_id = r % 2;
  dNode->page_word_frequency = r;

  list_append(wNode->page, dNode);

  return wNode;

}


char* test_one_word()
{ 
  Index = calloc(1, sizeof(HashTable));
  hashtable_new(Index, sizeof(WordNode), wNode_cmp, hash1, wNode_free);

  int r = random();
  char* word = words[r % 2];
  int docID = r % MAX_DOC;
  updateIndex(word, docID, Index);

  // Test if word was inserted into index
  int result = hashtable_lookup(Index, word);
  mu_assert(result == 1, "Error - Could not find inserted word.");

  // Test if we can retrive the word node
  WordNode* wNode = calloc(1, sizeof(WordNode));
  result = hashtable_get(Index, word, wNode);
  mu_assert(result == 1, "Error - Could not retrive WordNode.");
  mu_assert(strcmp(wNode->word, words[r % 2]) == 0, "Error - Incorrect word in WordNode.");

  // Test proper DocumentNode
  result = list_length(wNode->page);
  mu_assert(result == 1, "Error - Incorrect page length for DocumentNode.");

  DocumentNode* dNode = list_head(wNode->page);
  //wNode = NULL;
  mu_assert(dNode->document_id == docID, "Error - Incorrect DocumentNode ID.");
  mu_assert(dNode->page_word_frequency == 1, "error - Incorrect DocumentNode frequency.");

  free(wNode);
  // Value in table should be find
  result = hashtable_lookup(Index, word);
  mu_assert(result == 1, "Error - Removed invariant in Index.");

  hashtable_destroy(Index);
  return NULL;
}



char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_one_word);

  return NULL;
}

RUN_TESTS(all_tests);
