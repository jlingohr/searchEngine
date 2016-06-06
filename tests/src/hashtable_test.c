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
char* test4 = "test4 data";
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

int compare(element_t av, element_t bv)
{
  char* a = av;
  char* b = bv;
  return strcmp(a, b) == 0;
}

char* test_new()
{
  ht = calloc(1, sizeof(HashTable));
  hashtable_new(ht, sizeof(char*), compare, hash1, NULL);
  mu_assert(ht != NULL, "Error hashtable_new.");
  return NULL;
}

char* test_insert_lookup()
{
  // Create objects to insert
  char* obj1 = malloc(10*sizeof(char*));
  strcpy(obj1, "Data 1");
  char* obj2 = malloc(10*sizeof(char*));
  strcpy(obj2, "Data 2");
  char* obj3 = malloc(10*sizeof(char*));
  strcpy(obj3, "Data 3");

  // Test searching in an empty table
  int rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 0, "Error hashtable_lookup - found data in empty hashtable.");

  // Test inserting into empty hashtable
  hashtable_insert(ht, test1, obj1);
  rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 1, "Error hashtable_insert.");
  // Test searching for non-existant data
  rc = hashtable_lookup(ht, test2);
  mu_assert(rc == 0, "Error hashtable_lookup - Found non-existant data.");

  // Test inserting 2 with collisions and searching head and tail
  hashtable_insert(ht, test2, obj2);
  rc = hashtable_lookup(ht, test2);
  mu_assert(rc == 1, "Error hashtable_insert.");
  rc = hashtable_lookup(ht, test1);
  mu_assert(rc == 1, "Error hashtable_insert - Couldn't find existing data.");

  // Test inserting 3 with collisions and searching
  hashtable_insert(ht, test3, obj3);
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
  char* result = calloc(1, sizeof(10*sizeof(char*)));
  int rc = hashtable_get(ht, test1, result);
  mu_assert(result == expect1, "Error hashtable_get.");
  free(result);
  result = NULL;

  // Test getting from middle of table when there are collisions
  result = calloc(1, sizeof(10*sizeof(char*)));
  rc = hashtable_get(ht, test2, result);
  mu_assert(result == expect2, "Error hashtable_get.");
  free(result);
  result = NULL;

  // Test getting from head of table when there are collisions
  result = calloc(1, sizeof(10*sizeof(char*)));
  rc = hashtable_get(ht, test3, result);
  mu_assert(result == expect3, "Error hashtable_get.");
  free(result);
  result = NULL;

  // Test getting item not in hashtable
  result = calloc(1, sizeof(10*sizeof(char*)));
  rc = hashtable_get(ht, test4, result);
  mu_assert(result == NULL, "Error hashtable_get non-existant object.");
  mu_assert(rc == 0, "Error hashtable_get non-existant object.");
  free(result);
  result = NULL;

  return NULL;
}

char* test_destroy()
{
  // Test if hashtable properly destroyed and deallocated objects
  hashtable_destroy(ht);
  //free(ht); 
  ht = NULL; 
  
  return NULL;
}

char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_new);
  mu_run_test(test_insert_lookup);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);