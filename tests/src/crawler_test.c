/*
* crawler_test.c - Try to simulate the crawler, focusing on
* sharing objects between the hashtable and linked-list
*/

#include "../minunit.h"
#include "../../util/hashtable.h"
#include "../../util/list.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "../../util/common.h"
#include <unistd.h>

#define MAX 
#define URL_1 "url_1"
#define URL_2 "url_2"
#define URL_3 "url_3"
#define URL_4 "url_4"
#define URL_5 "url_5"
#define HTML_1 "html_1"
#define HTML_2 "html_2"
#define HTML_3 "html_3"
#define HTML_4 "html_4"
#define HTML_5 "html_5"
#define SEED_URL "seed_url"
#define SEED_HTML "seed_html"

/****
* Hashtable and list functions
****/

int cmp_URL(const element_t av, const element_t bv)
{
  char* a = av;
  char* b = bv;
  while (*a != 0 && *b != 0 && *a == *b) {
    a++;
    b++;
  }
  return !(*a < *b? -1 : *a == *b? 0 : 1);
}

uint32_t hash_URL(const element_t keyv)
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





char* urls[MAX] = {URL_1, URL_2, URL_3, URL_4, URL_5};
char* html[MAX] = {HTML_1, HTML_2, HTML_3, HTML_4, HTML_5};


static void init_test(HashTable** URLSVisited, List** toVisit)
{
  *toVisit = calloc(1, sizeof(List));
  list_new(*toVisit, sizeof(WebPage), cmp_webpage, free_webpage);

  *URLSVisited = calloc(1, sizeof(HashTable));
  hashtable_new(*URLSVisited, MAX_URL_LENGTH, cmp_URL, hash_URL, hash_free);
}

static void sim_crawl(HashTable* URLSVisited, List* toVisit, int depth)
{
  char* buf = NULL;

  for (int i = 0; i < depth; i++) {
    buf = calloc(1, strlen(urls[i])+1);
    strcpy(buf, urls[i]);
    if (!hashtable_lookup(URLSVisited, buf)) {
      WebPage* tmp = calloc(1, sizeof(WebPage));
      strcpy(tmp->url, "");
      strcpy(tmp->url, buf);
      tmp->html = NULL;
      tmp->depth = depth + 1;

      list_append(toVisit, tmp);
    }
    free(buf);
  }
}

char* test_crawl_seed()
{ 
  HashTable* URLSVisited = NULL;
  List* toVisit = NULL;
  init_test(&URLSVisited, &toVisit);
  // Crawl the seed page only
  char* seed_url = calloc(1, strlen(SEED_URL)+1);
  strcpy(seed_url, SEED_URL);

  sim_crawl(URLSVisited, toVisit, 0);
  hashtable_insert(URLSVisited, seed_url, seed_url);
  int result = hashtable_lookup(URLSVisited, seed_url);
  // Seed page should be in hashtbale
  mu_assert(result == 1, "Error - Didn't crawl seed page.");
  // toVisit should be empty
  result = list_length(toVisit);
  mu_assert(result == 0, "Error - toVisit should be empty.");
  hashtable_destroy(URLSVisited);
  list_destroy(toVisit);

  return NULL;
}

char* test_crawl_one()
{
  HashTable* URLSVisited = NULL;
  List* toVisit = NULL;
  init_test(&URLSVisited, &toVisit);
  char* seed_url = calloc(1, strlen(SEED_URL)+1);
  strcpy(seed_url, SEED_URL);

  // Crawl the seed page
  sim_crawl(URLSVisited, toVisit, 1);
  hashtable_insert(URLSVisited, seed_url, seed_url);

  // Crawl page at depth 1
  WebPage* tmp;
  char* url;
  while ((tmp = list_dequeue(toVisit))) {
    if (!hashtable_lookup(URLSVisited, tmp->url)) {
      sim_crawl(URLSVisited, toVisit, 1);
      url = calloc(1, MAX_URL_LENGTH);
      hashtable_insert(URLSVisited, url, url);
    }
    strcpy(tmp->url, "");
    free(tmp);
  }
  // Seed page should be in hashtbale
  int result = hashtable_lookup(URLSVisited, SEED_URL);
  mu_assert(result == 1, "Error - Didn't crawl seed page.");
  // url_1 should be in hashtable
  result = hashtable_lookup(URLSVisited, URL_1);
  mu_assert(result == 1, "Error - url_1 not in hashtable.");
  // rest should not be in hashtable
  result = hashtable_lookup(URLSVisited, URL_2);
  mu_assert(result == 0, "Error - url_2 shouldn't be in hashtable.");



  hashtable_destroy(URLSVisited);
  list_destroy(toVisit);
  return NULL;
}


char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_crawl_seed);
  mu_run_test(test_crawl_one);

  return NULL;
}

RUN_TESTS(all_tests);
