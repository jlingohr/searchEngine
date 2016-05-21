/*
* test_list.c - Program to test list procedures
*/

#include <stdint.h>
#include "../list.h"
#include "../common.h"

#define SIZE 5

bool iter_int(element_t av) {
  printf("Found value: %d\n", *(int*)av);
  return TRUE;
}

void test_listAdd() {
  List A;
  List B;

  list_new(&A, sizeof(int), NULL);
  list_new(&B, sizeof(int), NULL);

  for (int i = SIZE; i > 0; i--) {
    int even = 2*i;
    int odd = 2*i + 1;
    list_append(&A, &even);
    list_prepend(&B, &odd);
  }

  list_for_each(&A, iter_int);
  printf("\n");
  list_for_each(&B, iter_int);
  printf("\n");

  list_destroy(&A);
  list_destroy(&B);
  printf("Successfully freed both lists...\n");
}


int main(void) {

  /* Test listAdd */
  test_listAdd();

  /* Test Merge */
  //test_Merge(A, B, &C);

  /* Test MergeSort */
  //test_MergeSort(C);
  
}