/*
* hashtable_test.c - Unit testing for a generic hashtable
* Note: Change MAX_HASH_SLOT to smaller value (2)
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

/*
* Dummy hash function and value so we always hash to the 
* same spot for testing
*/
static uint32_t hash = 0;
static uint32_t hash1(element_t a)
{
  return hash;
}


char* test_new()
{
  ht = calloc(1, sizeof(HashTable));
  hashtable_new(ht, sizeof(char*), NULL, hash1, NULL);
  mu_assert(ht != NULL, "Error hashtable_new.");
  return NULL;
}

char* test_insert_lookup()
{
  hashtable_insert(ht, test1, expect1);
  int rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 1, "Error hashtable_insert.");

  hashtable_insert(ht, test2, expect2);
  rc = hashtable_lookup(ht, test2);
  mu_assert(rc == 1, "Error hashtable_insert.");

  hashtable_insert(ht, test3, expect3);
  rc = hashtable_lookup(ht, test3);
  mu_assert(rc == 1, "Error hashtable_insert.");

  return NULL;
}

char* test_get()
{
  char* result = NULL;
  int rc = hashtable_get(ht, test1, result);
  mu_assert(result == expect1, "Error hashtable_get.");

  rc = hashtable_get(ht, test2, result);
  mu_assert(result == expect2, "Error hashtable_get.");

  rc = hashtable_get(ht, test3, result);
  mu_assert(result == expect3, "Error hashtable_get.");
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
  mu_run_test(test_insert_lookup);

  return NULL;
}

RUN_TESTS(all_tests);