/*
* test_hash.c - Program to test list procedures
* Not thorough; limited to basic funcitonality for now
*/

#include <stdint.h>
#include "../hashtable.h"
#include "../common.h"
#include <string.h>

void test_stringHash();
int str_cmp(element_t av, element_t bv);
uint32_t str_hash(element_t key);
void str_free(element_t data);

int main(void) {

  test_stringHash();
  return 0;
}

void test_stringHash()
{
  int numNames = 5;
  const char* names[] = {"David", "Kevin", "Michael", "Craig", "Jimi"};

  HashTable A;
  hashtable_new(&A, sizeof(char*), str_cmp, str_hash, str_free);

  // Insert names
  char* name;
  for (int i = 0; i < numNames; i++) {
    name = strdup(names[i]);
    hashtable_insert(&A, name, name);
    printf("Inserted %s\n", name);
  }


  // Test getting hashed values
  for (int i = 0; i < numNames; i++) {
    if (hashtable_get(&A, &names[i], name)) {
      printf("Found string value: %s\n", name);
    } else {
      printf("Error: Did not find %s\n", names[i]);
    }
  }

  // Test destroy
  hashtable_destroy(&A);
  for (int i = 0; i < numNames; i++) {
    if (hashtable_get(&A, &names[i], name)) {
      printf("Failure: Found %d when should have been removed...\n", i);
    }
  }
  printf("Successfully freed all names...\n");
}

int str_cmp(element_t av, element_t bv) 
{
  char* a = (char*)av;
  char* b = (char*)bv;
  return strcmp(a, b);
}

uint32_t str_hash(element_t keyv)
{
  // works with *(char**) only///
  char* key = (char*)keyv;
  printf("Hashing %s\n", key);
  size_t len = strlen(key);

  uint32_t hash = 0;
  uint32_t i = 0;

  for (hash = i = 0; i < len; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

void str_free(element_t datav)
{
  char* data = (char*)datav;
  free(data);
}