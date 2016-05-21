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
  ht->hash = hash;
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
static inline HashTableNode* hashtable_create_node(int elementSize, element_t data)
{
  HashTableNode* node = malloc(HashTableNode);
  node->data = malloc(elementSize);
  node->next = NULL;
  memcpy(node->data, data, elementSize);
  return node;
}

/*
* hashtable_insert - Insert an element into the hashtable
* @ht - hashtable to insert into
* @key - key to hash
* @data - data to insert
*/
void hashtable_insert(HashTable* ht, element_t key, element_t data)
{
  HashTableNode* node;
  uint32_t p = ht->hash(key);
  if ((node = ht->table[p]) == NULL) {
    node = hashtable_create_node(ht->elementSize, data);
    ht->table[p] = node;
  } else {
    while (node->next != NULL) {
      node = node->next;
    }
    HashTableNode* tmp = hashtable_create_node(ht->elementSize, data);
    node->next = tmp;
  }
}

/*
* hashtable_find - Find an element in the hashtable
* @ht: hashtable to search inside
* @key- key to hash
* Returns 1 if item found, otherwise returns 0
*/
int hashtable_find(HashTable* ht, element_t key)
{
  uint32_t p = ht->hash(key);
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
  uint32_t p = ht->hash(key);
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
  }
}
