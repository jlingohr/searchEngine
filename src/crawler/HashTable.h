/*
	Defines helper functions for operating on Dictionary
	hashtable
**/

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

/*
PRE: table is a valid DICTIONARY in memory
post: values of table are set to NULL
**/
void initDictionary(DICTIONARY* table);

/*
  addNodeToDictionary: Adds node for seed to dict if seed is unique
  
**/
void addNodeToDictionary(DICTIONARY* dict, char* seed, URLNODE* node);
#endif