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

int cmp_int(element_t av, element_t bv)
{
  intptr_t a = (intptr_t)av;
  intptr_t b = (intptr_t)bv;
  return a == b;
}

void test_intList() {
  List A;
  List B;

  list_new(&A, sizeof(int), cmp_int, NULL);
  list_new(&B, sizeof(int), cmp_int, NULL);

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

int cmp_page(element_t av, element_t bv)
{
  // TODO - Add test for this
  WebPage* a = av;
  WebPage* b = bv;
  return strcmp(a->url, b->url) == 0;
}

void free_page(element_t av) {
  WebPage* a = (WebPage*)av;
  free(a->url);
  free(a->html);
}

bool iter_page(element_t av) {
  WebPage* a = (WebPage*)av;
  printf("Found a webapge - url: %s. HTML: %s\n", a->url, a->html);
  return TRUE;
}

void test_WebPageList() {
  List A;

  list_new(&A, sizeof(WebPage), cmp_page, free_page);

  WebPage page;
  for (int i = 1; i < SIZE; i++) {
    page.url = malloc(MAX_URL_LENGTH);
    page.html = malloc(BUF_SIZE);
    strcpy(page.url, "url");
    strcpy(page.html, "html");

    list_append(&A, &page);
  }

  list_for_each(&A, iter_page);

  int result;
  if ((result = list_dequeue(&A, &page))) {
    printf("Dequed a webpage...\n");
    iter_page(&page);
  }
  printf("\n");
  list_destroy(&A);
  printf("Successfully freed all webpabes...\n");


}


int main(void) {

  /* Test listAdd */
  test_intList();
  test_WebPageList();

  /* Test Merge */
  //test_Merge(A, B, &C);

  /* Test MergeSort */
  //test_MergeSort(C);
  
}