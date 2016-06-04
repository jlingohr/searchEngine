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

char* test_append_dequeue()
{
  list_append(list, test1);
  mu_assert(list_tail(list) == test1, "Error list_append.");
  mu_assert(list_head(list) == test1, "Error list_append.");

  list_append(list, test2);
  mu_assert(list_tail(list) == test2, "Error list_append.");
  mu_assert(list_head(list) == test1, "Error list_append.");

  list_append(list, test3);
  mu_assert(list_tail(list) == test3, "Error list_append.");
  mu_assert(list_head(list) == test1, "Error list_append.");
  mu_assert(list_length(list) == 3, "Incorrect length on list_append.");

  char* val = list_dequeue(list);
  mu_assert(val == test1, "Error list_dequeue.");

  val = list_dequeue(list);
  mu_assert(val == test2, "Error list_dequeue.");

  val = list_dequeue(list);
  mu_assert(val == test3, "Error list_dequeue.");
  mu_assert(list_length(list) == 0, "Error length on list_dequeue.");

  val = list_dequeue(list);
  mu_assert(val == NULL, "Error list_dequeue empty list.");
  mu_assert(list_length(list) == 0, "Error length on empty list_dequeue.");

  return NULL;
}

char* test_get()
{
  char* val = NULL;
  int result = list_get(list, test1, &val);
  mu_assert(result == 0, "Error list_get on empty.");
  mu_assert(val == NULL, "Error list_get on empty.");

  list_append(list, test1);
  result = list_get(list, test1, &val);
  mu_assert(result == 1, "Error on list_get single.");
  mu_assert(val == test1, "Error on list_get single.");
  result = list_get(list, test2, &val);
  mu_assert(result == 0, "Error list_get miss on single.");
  mu_assert(val == NULL, "Error on list_get miss on single.");

  list_append(list, test2);
  result = list_get(list, test1, &val);
  mu_assert(result == 1, "Error on list_get.");
  mu_assert(val == test1, "Error on list_get.");
  result = list_get(list, test2, &val);
  mu_assert(result == 1, "Error on list_get.");
  mu_assert(val == test2, "Error on list_get.");

  list_append(list, test3);
  result = list_get(list, test1, &val);
  mu_assert(result == 1, "Error on list_get head.");
  mu_assert(val == test1, "Error on list_get.");
  result = list_get(list, test2, &val);
  mu_assert(result == 1, "Error on list_get middle.");
  mu_assert(val == test2, "Error on list_get.");
  result = list_get(list, test3, &val);
  mu_assert(result == 1, "Error on list_get tail.");
  mu_assert(val == test3, "Error on list_get.");

  return NULL;
}


char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_new);
  mu_run_test(test_append_dequeue);
  mu_run_test(test_get);

  return NULL;
}

RUN_TESTS(all_tests);
