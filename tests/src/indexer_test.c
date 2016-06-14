/*
* indexer_test.c - Testing for indexer.c
*/

#include "../minunit.h"
#include "../../indexer/src/index.h"
#include "../../util/hashtable.h"
#include "../../util/list.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

HashTable* Index = NULL;
//char* doc_man = "../data_src/1";
//char* doc_bear = "../data_src/2";
//char* doc_pig = "../data_src/3";

char* path = "./data_src/";
char* dir = "./data_src/";

char* test_one()
{
  char* doc_man = calloc(1, strlen(path)+2);
  strcpy(doc_man, path);
  strcat(doc_man, "1");
  char* doc = loadDoc(doc_man);
  int docID = getDocID(doc_man, dir);
  mu_assert(strlen(doc) == 7, "Error - loadDoc reading single word.");
  mu_assert(docID == 1, "Error - getDocID.");
  free(doc);
  return NULL;
}



char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_one);

  return NULL;
}

RUN_TESTS(all_tests);
