
/*
* Associated location for each word containing
*   1- page identifer
*   2- Number of occurences
*/
typedef struct _DocumentNode {
  struct _DocumentNode* next;   /* Pointer to next member of list */
  int document_id;              /* Document identifer */
  int page_word_frequency;      /* Number of occurrences of the word */
} DocumentNode;

/*
* Each word in a page is represented as a node
* in a list
*/

typedef struct _WordNode {
  struct _WordNode* prev;       /* Pointer to previous word */
  struct _WordNode* next;       /* Pointer to the next word */
  char word[WORD_LENGTH];       /* The word */
  DocumentNode* page;           /* Pointer to first element of the page list */
} WordNode;

/*
* Inverted list is a sorted list of locations where the term
* appears in the collection. The location consists of:
*   1- page identifier
*   2- Position of term in the page
*/
typedef struct _InvertedIndex {
  WordNode* start;              /* Start and end pointer of dynamic links */
  WordNode* end;                /* Start of the list */
  WordNode* hash[MAX_NUMBER_OF_SLOTS] /* Hash slot */
} InvertedIndex;