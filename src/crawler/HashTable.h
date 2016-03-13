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
  addNodeToDictionary: Adds node for seed to dict if seed is unique. If dict
  is empty, create DNODE at hashed value and set start = end = DNODE. Otherwise,
  check if dict[hash] is empty. If it is, insert DNODE at dict[hash] and set pointers.
  Otherwise traverse linked-list to check uniqueness.

**/
void addNodeToDictionary(DICTIONARY* dict, char* seed, void* node);

/*
  initDNODE: Initializes a DNODE by copying in values from urlNode
  Set next/prev pointers to NULL, data to urlNode, and copy in key
**/
void initDNODE(DNODE* dnode, char* key, void* urlNode);
#endif