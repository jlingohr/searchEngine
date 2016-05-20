/*
* test_list.c - Program to test list procedures
*/

#include <stdint.h>
#include "../list.h"
#include "../common.h"

#define SIZE 2

/*
* cmpInt - ListNodes whose elements are integers */
int cmpInt(element_t av, element_t bv) {
  intptr_t a = (intptr_t)av;
  intptr_t b = (intptr_t)bv;

  return (a <= b);
}

void printInt(element_t av) {
  intptr_t a = (intptr_t)av;
  printf("%ld ", a);
}

void test_listAdd(List* A) {
  printf("Testing listAdd...\n");
  listForEach(printInt, A);
  printf("\n");
}

void test_Merge(List* A, List* B, List** C) {
  printf("Testing Merge...\n");
  *C = Merge(A, B, cmpInt);
  listForEach(printInt, *C);
  printf("\n");
  //listDelete(free, tmp);

}

void test_MergeSort(List* C) {
  printf("Testing MergSort...\n");
  MergeSort(C, C->len, cmpInt);
  listForEach(printInt, C);
  printf("\n");
}

int main(void) {

  /* create lists */
  List* A = initList();
  List* B = initList();
  List* C = NULL;

  for (int i = SIZE; i > 0; i--) {
    intptr_t even = 2*i;
    intptr_t odd = 2*i + 1;
    listAdd(A, (element_t)even);
    listAdd(B, (element_t)odd);
  }

  /* Test listAdd */
  test_listAdd(A);

  /* Test Merge */
  test_Merge(A, B, &C);

  /* Test MergeSort */
  test_MergeSort(C);
  
}