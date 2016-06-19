/*
* crawler_test.c - Try to simulate the crawler, focusing on
* sharing objects between the hashtable and linked-list
*/

#include "../minunit.h"
#include "../../util/hashtable.h"
#include "../../util/list.h"
#include "../../util/web.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "../../util/common.h"


/****
* Hashtable and list functions
****/

int cmp_URL(element_t av, element_t bv)
{
  char* a = av;
  char* b = bv;
  while (*a != 0 && *b != 0 && *a == *b) {
    a++;
    b++;
  }
  return !(*a < *b? -1 : *a == *b? 0 : 1);
}

uint32_t hash_URL(element_t keyv)
{
  char* key = keyv;
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

void hash_free(element_t datav)
{
  char* data = datav;
  free(data);
}

int cmp_webpage(element_t av, element_t bv)
{
  WebPage* a = av;
  WebPage* b = bv;
  return strcmp(a->url, b->url) == 0;
}

void free_webpage(element_t elem)
{
  WebPage* page = elem;
  free(page->html);
}

HashTable* URLSVisited = NULL;
List* toVisit = NULL;
WebPage* seed_page = NULL;
char* seed_url = "seed_url";
char* seed_html = "seed_html";

char* url_1 = "url_1";
char* url_2 = "url_2";
char* url_3 = "url_3";
char* urls[] = {url1, url_2, url_3};

char* html_1 = "html_1";
char* html_2 = "html_2";
char* html_3 = "html_3";
char* html[] = {html_1, html_2, html_3};

static void init_test()
{
  seed_page = calloc(1, sizeof(WebPage));
  seed_page->html = NULL;
  strcpy(seed_page->url, seed_url);
  seed_page->depth = 0;

  toVisit = calloc(1, sizeof(List));
  list_new(toVisit, sizeof(WebPage), cmp_webpage, free_webpage);

  URLSVisited = calloc(1, sizeof(HashTable));
  hashtable_new(URLSVisited, MAX_URL_LENGTH, cmp_URL, hash_URL, hash_free);
}

static void sim_crawl(int i)
{
  char* buf = NULL;

  while (i > 0) {
    buf = urls[i];
    if (!hashtable_lookup(URLSVisited, buf)) {
      WebPage* tmp = calloc(1, sizeof(WebPage));
      strcpy(tmp->url, "");
      strcpy(tmp->url, buf);
      tmp->html = NULL;
      tmp->depth = page->depth + 1;

      list_append(toVisit, tmp);
    }
  }
}

char* test_crawl_seed()
{ 
  init_test();
  sim_crawl(1);



  return NULL;
}



char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_crawl_seed);

  return NULL;
}

RUN_TESTS(all_tests);
