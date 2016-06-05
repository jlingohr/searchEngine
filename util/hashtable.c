#include "hashtable.h"

/*
* default_compare - Default compare function if now comparator
* passed in during initialization. Use only for strings!
*/
static int default_compare(element_t av, element_t bv)
{
  return strcmp((char*)av, (char*)bv);
}

/*
* default_hash - Default hashing function. Use only
* for strings
*/
static uint32_t default_hash(element_t av)
{
  char* key = av;
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

/*
* default_free - Default free function for hashtable if none
* passed in during initialization. Safe only for char*
*/
static void default_free(element_t a)
{
  free((char*)a);
}

/*
* hashtable_new - Initializes a new hashtable
* @ht: Hashtable to initialize
* @elementSize - Size of hash table nodes
* @cmp - Comparator function 
* @hash - hashing function
* freeFn - Function to free the data
*/
void hashtable_new(HashTable* ht, int elementSize, hashtable_compare cmp,
 hashtable_hash hash, freeFunction freeFn)
{
  assert(elementSize > 0);

  for (int i = 0; i < MAX_HASH_SLOT; i++) {
    ht->table[i] = NULL;
  }
  ht->elementSize = elementSize;
  ht->compare = cmp == NULL ? default_compare : cmp;
  ht->hashFn = hash == NULL ? default_hash : hash;
  ht->freeFn = freeFn == NULL ? default_free : freeFn;
  ht->size = 0;
}

/*
* hashtable_destroy - Deallocates the hashtable
* @ht - hashtale to deallocate
*/
void hashtable_destroy(HashTable* ht) 
{
  HashTableNode *cur, *tmp;
  for (int i = 0; i < MAX_HASH_SLOT; i++) {
    cur = ht->table[i];
    while (cur != NULL) {
      tmp = cur;
      cur = cur->next;
      if (ht->freeFn) {
        ht->freeFn(tmp->data);
        tmp->data = NULL;
      }
      free(tmp);
      tmp = NULL;
    }
    ht->table[i]= NULL;
  }
  free(ht->table);
  //free(ht);
  ht = NULL;
}



/*
* hashtable_create_node - private function to create a new
* hashtable node to insert into the hashtable
* @elementSize - size of data to be inserted into node
* @data - data to insert into node
*/
static inline HashTableNode* hashtable_create_node(element_t data, uint32_t hash)
{
  HashTableNode* node = calloc(1, sizeof(HashTableNode));
  //node->data = calloc(1, ht->elementSize);
  node->hash = hash;
  //memcpy(node->data, data, ht->elementSize);
  node->data = data;
  node->next = NULL;
  
  return node;
}

/*
* hashtable_find_bucket - private helper to see if an element
* already exists in the hashtable. 
* @ht- table to search in
* @key - key to hash
* @hash_out - pointer to return full hashed value to caller
*/
static inline int hashtable_find_bucket(HashTable* ht, element_t key, 
  uint32_t *hash_out)
{
  uint32_t hash = ht->hashFn(key);
  int bucket = hash % MAX_HASH_SLOT;
  *hash_out = hash;

  return bucket;
}

/*
* hashtable_find - Find an element in the hashtable
* @ht: hashtable to search inside
* @bucket: First ListNode in ht->table[hash]
* @key- key to hash
* Returns 1 if item found, otherwise returns 0
*/
static inline int hashtable_find(HashTable* ht, HashTableNode* bucket, element_t key,
  uint32_t hash, element_t elem)
{
  // TODO - refactor
  HashTableNode* cur = bucket;
  while (cur) {
    if (cur->hash == hash && ht->compare(key, cur->data)) {
      if (elem != NULL) {
        memcpy(elem, cur->data, ht->elementSize);
      }
      return 1;
    } else {
      cur = cur->next;
    }  
  }
  return 0;
}

/*
* hashtable_insert - Insert an element into the hashtable
* @ht - hashtable to insert into
* @key - key to hash
* @data - data to insert
*/
void hashtable_insert(HashTable* ht, element_t key, element_t data)
{
  uint32_t hash = 0;
  int p = hashtable_find_bucket(ht, key, &hash);
  if (ht->table[p] == NULL) {
    // No item hashed here, so start chaining
    ht->table[p] = hashtable_create_node(data, hash);
  } else {
    HashTableNode* node = NULL;
    if (!hashtable_find(ht, ht->table[p], key,  hash, node)) {
      // Not found, so insert
      node = hashtable_create_node(data, hash);
      node->next = ht->table[p];
      ht->table[p] = node;  // Probably incorrect pointing...
    }
  }
  ht->size++;

}



/*
* hashtable_get - Retrieves an item in the hashtable.
* Allocates space for the return object elem and copies over value if found
* Important: Up to caller to delloacte - TODO: Change this!
* @ht - Hashtable to retrive item in
* @key - key of item to retrieve
* @elem - retrieved item wil be stored in here
* Returns 1 if item successfully retrieved, else 0
*/
int hashtable_get(HashTable* ht, element_t key, element_t elem)
{
  // TODO - refactor: Keep the copy or redirect a pointer?
  uint32_t hash = 0;
  //node = hashtable_find_bucket(ht, key, &hash);
  int p = hashtable_find_bucket(ht, key, &hash);
  if (ht->table[p] == NULL) {
    return 0;
  } else {
    return hashtable_find(ht, ht->table[p], key, hash, elem);
  }
}

/*
* hashtable_lookup - Returns 1 if key is in table,
* otherwise returns 0
*/
int hashtable_lookup(HashTable* ht, const element_t key)
{
  uint32_t hash = 0;
  int p = hashtable_find_bucket(ht, key, &hash);
  if (ht->table[p] == NULL) {
    return 0;
  } else {
    return hashtable_find(ht, ht->table[p], key, hash, NULL);
  }
}


