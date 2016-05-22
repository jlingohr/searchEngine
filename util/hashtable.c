#include "hashtable.h"

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
  /* TODO - Give default uses for these */
  assert(elementSize > 0);
  assert(cmp != NULL);
  assert(hash != NULL);
  assert(freeFn != NULL);

  for (int i = 0; i < MAX_HASH_SLOT; i++) {
    ht->table[i] = NULL;
  }
  ht->elementSize = elementSize;
  ht->compare = cmp;
  ht->hashFn = hash;
  ht->freeFn = freeFn;
}

/*
* hashtable_destroy - Deallocates the hashtable
* @ht - hashtale to deallocate
*/
void hashtable_destroy(HashTable* ht) 
{
  HashTableNode* cur;
  for (int i = 0; i < MAX_HASH_SLOT; i++) {
    while ((cur = ht->table[i]) != NULL) {
      ht->table[i] = cur->next;
      ht->freeFn(cur->data);
      free(cur->data);
      free(cur);
    }
  }
}



/*
* hashtable_create_node - private function to create a new
* hashtable node to insert into the hashtable
* @elementSize - size of data to be inserted into node
* @data - data to insert into node
*/
static inline HashTableNode* hashtable_create_node(int elementSize, element_t data, uint32_t hash)
{
  HashTableNode* node = malloc(elementSize);
  node->data = malloc(elementSize);
  node->hash = hash;
  node->next = NULL;
  memcpy(node->data, data, elementSize);
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

  //HashTableNode* node = ht->table[bucket];
  return bucket;
}

/*
* hashtable_find - Find an element in the hashtable
* @ht: hashtable to search inside
* @key- key to hash
* Returns 1 if item found, otherwise returns 0
*/
static inline int hashtable_find(HashTable* ht, HashTableNode* node, element_t key,
  uint32_t hash, element_t elem)
{
  /*uint32_t p = ht->hashFn(key)%MAX_HASH_SLOT;
  if (ht->table[p] == NULL) {
    return 0;
  } else {
    HashTableNode* cur = ht->table[p];
    while (cur) {
      if (ht->compare(key, cur->data)) {
        return 1;
      }
      cur = cur->next;
    }
    return 0;
  }*/
  /*HashTableNode* node;
  
  uint32_t hash = 0;
  node = hashtable_find_bucket(ht, key, &hash);
  if (node == NULL) {
    return 0;
  } else {
    while (node) {
      if (node->hash == hash && ht->compare(key, node->data)) {
        return 1;
      }
      node = node->next;
    }
    return 0;
  }*/
  HashTableNode* cur = node;
  while (cur->next != NULL) {
    if (cur->hash == hash && ht->compare(key, cur->data)) {
      // Found, copy into elem and return
      if (elem) {
        memcpy(elem, cur->data, ht->elementSize);
      }
      return 1;
    }
    cur = cur->next;
  }
  if (cur->hash == hash && ht->compare(key, cur->data)) {
    if (elem) {
      memcpy(elem, cur->data, ht->elementSize);
    }
    return 1;
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
  /*HashTableNode* node;
  uint32_t p = ht->hashFn(key)%MAX_HASH_SLOT;
  if (ht->table[p] == NULL) {
    node = hashtable_create_node(ht->elementSize, data);
    ht->table[p] = node;
  } else {
    node = ht->table[p];
    while (node->next != NULL) {
      node = node->next;
    }
    HashTableNode* tmp = hashtable_create_node(ht->elementSize, data);
    node->next = tmp;
  }*/
  /*HashTableNode* node;

  uint32_t hash = 0;
  node = hashtable_find_bucket(ht, key, &hash);
  if (node == NULL) {
    // No item hashed here, so start chaining
    node = hashtable_create_node(ht->elementSize, data, hash);
  } else {
    // Scan through until end, then insert
    HashTableNode* cur = node->next;
    while (cur != NULL) {
      if (node->hash == hash && ht->compare(node->data, key)) {
        // Already in hashtable
        return; // NOT CORRECT - notify user or something
      }
      node = cur;
      cur = cur->next;
    }
    // At tail, check once and insert if not equal
    if (node->hash == hash && ht->compare(node->data, key)) {
      return; // BETTER WAY
    } else {
      cur = hashtable_create_node(ht->elementSize, data, hash);
      node->next = cur;
      return;
    }
  }*/
  HashTableNode* node;

  uint32_t hash = 0;
  //node = hashtable_find_bucket(ht, key, &hash);
  int p = hashtable_find_bucket(ht, key, &hash);
  if (ht->table[p] == NULL) {
    // No item hashed here, so start chaining
    node = hashtable_create_node(ht->elementSize, data, hash);
    ht->table[p] = node;
  } else {
    node = ht->table[p];
    if (!hashtable_find(ht, node, key,  hash, NULL)) {
      // Not found, so insert
      HashTableNode* tmp = hashtable_create_node(ht->elementSize, data, hash);
      tmp->next = node;
      node = tmp;  // Probably incorrect pointing...
    }
  }

}



/*
* hashtable_get - Retrieves an item in the hashtable
* @ht - Hashtable to retrive item in
* @key - key of item to retrieve
* @elem - retrieved item wil be stored in here
* Returns 1 if item successfully retrieved, else 0
*/
int hashtable_get(HashTable* ht, element_t key, element_t elem)
{
  /*uint32_t p = ht->hashFn(key)%MAX_HASH_SLOT;
  if (ht->table[p] == NULL) {
    return 0;
  } else {
    HashTableNode* cur = ht->table[p];
    while (cur) {
      if (ht->compare(key, cur->data)) {
        // Passing a pointer - Sure you want to do this?
        elem = cur->data;
        return 1;
      }
      cur = cur->next;
    }
    return 0;
  }*/
  HashTableNode* node;

  uint32_t hash = 0;
  //node = hashtable_find_bucket(ht, key, &hash);
  int p = hashtable_find_bucket(ht, key, &hash);
  if (ht->table[p] == NULL) {
    return 0;
  } else {
    node = ht->table[p];
    return hashtable_find(ht, node, key, hash, elem);
  }
}
