/*
* test_list.c - Unit testing for a generic linked-list
*/

#include "../minunit.h"
#include "../../util/list.h"
#include <assert.h>


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

bool int_iterate(element_t data)
{
  printf("%d ", (int)data);
  return TRUE;
}

int int_cmp(element_t av, element_t bv)
{
  intptr_t a = (intptr_t)av;
  intptr_t b = (intptr_t)bv;
  return a<b ? -1 : a==b? 0 : 1;

}

static List* list = NULL;
char* test1 = "test1 data";
char* test2 = "test2 data";
char* test3 = "test3 data";

int is_sorted(List* list)
{
  list_foreach(list, head, next, cur) {
    if (cur->next && int_cmp(cur->data, cur->next->data) > 0) {
      debug("%ld %ld", (intptr_t)cur->data, (intptr_t)(cur->next->data));
      return 0;
    }
  }
  return 1;
}


char* test_new()
{
  // Test list initialization
  list = calloc(1, sizeof(List));
  mu_assert(list != NULL, "Failed to create a new list.");
  list_new(list, sizeof(char*), str_cmp, NULL);
  mu_assert(list->length == 0, "New list should have length of 0.");

  return NULL;
}

char* test_destroy()
{
  list_destroy(list);

  return NULL;
}

char* test_append_dequeue()
{
  // Test dequeue from empty list
  char* val = list_dequeue(list);
  mu_assert(val == NULL, "Error list_dequeue on empty list.");

  // Test append to empty list
  list_append(list, test1);
  mu_assert(list_tail(list) == test1, "Error list_append.");
  mu_assert(list_head(list) == test1, "Error list_append.");

  // Test appending to list of size 1
  list_append(list, test2);
  mu_assert(list_tail(list) == test2, "Error list_append.");
  mu_assert(list_head(list) == test1, "Error list_append.");

  // Test appending to list of size 2
  list_append(list, test3);
  mu_assert(list_tail(list) == test3, "Error list_append.");
  mu_assert(list_head(list) == test1, "Error list_append.");
  mu_assert(list_length(list) == 3, "Incorrect length on list_append.");

  // Test dequeue from list
  val = list_dequeue(list);
  mu_assert(val == test1, "Error list_dequeue.");

  val = list_dequeue(list);
  mu_assert(val == test2, "Error list_dequeue.");

  val = list_dequeue(list);
  mu_assert(val == test3, "Error list_dequeue.");
  mu_assert(list_length(list) == 0, "Error length on list_dequeue.");

  val = list_dequeue(list);
  mu_assert(val == NULL, "Error list_dequeue empty list.");
  mu_assert(list_length(list) == 0, "Error length on empty list_dequeue.");
  val = NULL;

  return NULL;
}

char* test_get()
{
  // Test getting from empty list
  char* val = NULL;
  int result = list_get(list, test1, &val);
  mu_assert(result == 0, "Error list_get on empty.");
  mu_assert(val == NULL, "Error list_get on empty.");

  // Test getting from list of size 1
  list_append(list, test1);
  result = list_get(list, test1, &val);
  mu_assert(result == 1, "Error on list_get single.");
  mu_assert(val == test1, "Error on list_get single.");
  result = list_get(list, test2, &val);
  mu_assert(result == 0, "Error list_get miss on single.");
  mu_assert(val == NULL, "Error on list_get miss on single.");

  // Test getting from head and tail of list
  list_append(list, test2);
  result = list_get(list, test1, &val);
  mu_assert(result == 1, "Error on list_get.");
  mu_assert(val == test1, "Error on list_get.");
  result = list_get(list, test2, &val);
  mu_assert(result == 1, "Error on list_get.");
  mu_assert(val == test2, "Error on list_get.");

  // Test getting from list of size 3
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
  val = NULL;

  return NULL;
}

char* test_merge_sort()
{
  list = calloc(1, sizeof(List));
  list_new(list, sizeof(intptr_t), int_cmp, NULL);
  for (int i = 100; i > 0; i--) {
    list_append(list, (intptr_t)i);
  }

  List* sorted = MergeSort(list, list_length(list), int_cmp);
  mu_assert(is_sorted(sorted), "Error MergeSort.");

  List* sorted2 = MergeSort(sorted, list_length(sorted), int_cmp);
  mu_assert(is_sorted(sorted2), "Error MergeSort on sorted list.");
  list_destroy(sorted2);
  list_destroy(sorted);

  list_destroy(list);
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
