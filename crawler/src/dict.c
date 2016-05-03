#include "dict.h"
#include <stdlib.h>
/*
* initDict - Initialize the dictionary data structure
*/
void initDict(DICTIONARY* dict) {
  // TODO
  dict = malloc(sizeof(DICTIONARY));
  dict->start = dict->end = NULL;
}