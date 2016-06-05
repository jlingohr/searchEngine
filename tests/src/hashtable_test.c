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
uint32_t hash = 0;
uint32_t hash1(element_t a)
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
  // Test searching in an empty table
  int rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 0, "Error hashtable_lookup - found data in empty hashtable.");

  // Test inserting into empty hashtable
  hashtable_insert(ht, test1, expect1);
  rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 1, "Error hashtable_insert.");
  // Test searching for non-existant data
  rc = hashtable_lookup(ht, test2);
  mu_assert(rc == 0, "Error hashtable_lookup - Found non-existant data.");

  // Test inserting 2 with collisions and searching head and tail
  hashtable_insert(ht, test2, expect2);
  rc = hashtable_lookup(ht, test2);
  mu_assert(rc == 1, "Error hashtable_insert.");
  rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 1, "Error hashtable_insert - Couldn't find existing data.");

  // Test inserting 3 with collisions and searching
  hashtable_insert(ht, test3, expect3);
  rc = hashtable_lookup(ht, test3);
  mu_assert(rc == 1, "Error hashtable_insert.");
  rc = hashtable_lookup(ht, test2);
  mu_assert(rc == 1, "Error hashtable_insert - Couldn't find existing data.");
  rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 1, "Error hashtable_insert - Couldn't find existing data.");

  return NULL;
}

char* test_get()
{
  // Test getting from tail of the table when there are collisions
  char* result = NULL;
  int rc = hashtable_get(ht, test1, result);
  mu_assert(result == expect1, "Error hashtable_get.");

  // Test getting from middle of table when there are collisions
  rc = hashtable_get(ht, test2, result);
  mu_assert(result == expect2, "Error hashtable_get.");

  // Test getting from head of table when there are collisions
  rc = hashtable_get(ht, test3, result);
  mu_assert(result == expect3, "Error hashtable_get.");
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