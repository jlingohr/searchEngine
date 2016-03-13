#include "crawler.h"
#include "HashTable.h"
#include <stdlib.h>

void initDictionary(DICTIONARY* table) {
	//TODO
	for (int i = 0; i < MAX_HASH_SLOT; i++) {
		table->hash[i] = NULL;
	}
	table->start = NULL;
	table->end = NULL;
}