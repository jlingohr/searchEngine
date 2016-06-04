#include "../minunit.h"
#include "../../util/list.h"
#include <assert.h>

//typedef int (*list_compare)(element_t av, element_t bv);
//typedef void (*freeFunction)(element_t);

static int str_cmp(element_t av, element_t bv)
{
  char* a = av;
  char* b = bv;
  while (*a != 0 && *b != 0 && *a == *b) {
    a++;
    b++;
  }
  return !(*a < *b? -1 : *a == *b? 0 : 1);
}

static List* list = NULL;
char* test1 = "test1 data";
char* test2 = "test2 data";
char* test3 = "test3 data";



char* test_new()
{
  list = calloc(1, sizeof(List));
  mu_assert(list != NULL, "Failed to create a new list.");
  list_new(list, sizeof(char*), str_cmp, NULL);
  mu_assert(list->length == 0, "New list should have length of 0.");

  return NULL;
}


char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_new);

  return NULL;
}

RUN_TESTS(all_tests);
