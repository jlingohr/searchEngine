/*
* index_test.c - Testing for index.c
*/

#include "../minunit.h"
#include "../../indexer/index.h"
#include <assert.h>

HashTable* index = NULL;


char* test_new()
{

  return NULL;
}



char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_new);
  mu_run_test(test_append_dequeue);
  mu_run_test(test_get);
  mu_run_test(test_destroy);

  mu_run_test(test_merge_sort);

  return NULL;
}

RUN_TESTS(all_tests);
