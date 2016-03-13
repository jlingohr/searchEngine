#include "crawler.h"
#include "HashTable.h"
#include <stdlib.h>
#include <string.h>
#include "../util/hash.h"

void initDictionary(DICTIONARY* table) {
	for (int i = 0; i < MAX_HASH_SLOT; i++) {
		table->hash[i] = NULL;
	}
	table->start = NULL;
	table->end = NULL;
}

void addNodeToDictionary(DICTIONARY* dict, char* key, void* urlNode) {
  DNODE* node = malloc(sizeof(DNODE));
  initDNODE(node, key, urlNode);
  unsigned long hashVal = hash1(key)%MAX_HASH_SLOT;
  // Dictionary is empty
  if (dict->start == NULL) {
    dict->hash[hashVal] = node;
    dict->start = dict->end = dict->hash[hashVal];
  } else {
    // URL is unique
    if (dict->hash[hashVal] == NULL) {
      dict->hash[hashVal] = node;
      dict->end->next = dict->end = dict->hash[hashVal];
    } else {
      DNODE* cur = dict->hash[hashVal];
      while (hashVal == (hash1(cur->key)%MAX_HASH_SLOT)) {
        // CHeck if unique
        if (strcmp(cur->key, key) == 0) {
          break;
        }
        // End of dicitonary
        if (cur == dict->end) {
          dict->end->next = node;
          dict->end->next->prev = dict->end;
          dict->end = dict->end->next;
          break;
        }
        // End of DNODE cluster
        // REVIEW THIS
        if (hashVal != (hash1(cur->next->key)%MAX_HASH_SLOT)) {
          DNODE* tmp = cur->next;
          cur->next = node;
          cur->next->prev = cur;
          cur->next->next = tmp;
          tmp->prev = cur->next;
          break;
        }
        cur = cur->next;
      }
    }
  }
}

void initDNODE(DNODE* dnode, char* key, void* urlNode) {
  dnode->next = dnode->prev = NULL;
  dnode->data = urlNode;
  strncpy(dnode->key, key, KEY_LENGTH);
}