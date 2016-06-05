/*
* hashtable_test.c - Unit testing for a generic hashtable
*/
#include "../minunit.h"
#include "../../util/hashtable.h"
#include <assert.h>

HashTable* ht = NULL;

char* test1 = "test1 data";
char* test2 = "test2 data";
char* test3 = "test3 data";
char* expect1 = "Data 1";
char* expect2 = "Data 2";
char* expect3 = "Data 3";


char* test_new()
{
  hashtable_new(ht, sizeof(char*), NULL, NULL, NULL);
  mu_assert(ht != NULL, "Error hashtable_new.");
  return NULL;
}

char* test_insert()
{
  return NULL;
}

char* test_get()
{
  return NULL;
}

char* test_lookup()
{
  return NULL;
}

char* test_destroy()
{
  return NULL;
}

char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_new);

  return NULL;
}

RUN_TESTS(all_tests);