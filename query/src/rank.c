#include "rank.h"



/*
* printResults - Handle results from query by
* opening the relevant document corresponding to each
* DocumentNode in results and fetching the url to
* print to the user
*/
void handleResults(List* results, char* path)
{
  DocumentNode* dNode;
  char filename[MAXLINE];
  int path_len = strlen(path);
  FILE* fd;
  ssize_t read;
  size_t len = 0;
  char* line = NULL;

  strcpy(filename, path);

  ListNode* cur = results->head;
  while (cur) {
    dNode = cur->data;
    sprintf(filename + path_len, "%d", dNode->document_id);

    //open file and retrieve URL
    fd = fopen(filename, "r");
    if ((read = getline(&line, &len, fd)) != -1) {
      if (line) {
        printf("%d  %d %s", dNode->document_id, dNode->page_word_frequency, line);
      }
    }
    fclose(fd);
    cur = cur->next;

  }
}



/*
* intersect - intersect two lists of DocumentNodes
* Returns pointer to a new list
*/
List* intersect(List* A, List* B) {
  List* list;

  if (A == NULL || B == NULL) // empty sets 
    return NULL;

  assert(A != NULL && B != NULL);

  // Initialize new list 
  list = calloc(1, sizeof(List));
  list_new(list, sizeof(DocumentNode), cmpDNode_ID, NULL);

  // Find highest docID of smaller list
  int maxID = 0;
  // Set current pointer to smaller list 
  List* smaller = (list_length(A) < list_length(B)) ? A : B;
  List* larger = (smaller == A) ? B : A;

  ListNode *cur, *_node;
  for (cur = smaller->head, _node = cur; _node != NULL; cur = _node = _node->next) {
    DocumentNode* dNode = (DocumentNode*)cur->data;
    maxID = (dNode->document_id > maxID) ? dNode->document_id : maxID;
  }

  DocumentNode* Intersect[maxID];
  for (int i = 0; i < maxID; i++) {
    Intersect[i] = NULL;
  }
  // Copy DocumentNodes in smaller list into an array
  for (cur = smaller->head, _node = cur; _node != NULL; cur = _node = _node->next) {
    DocumentNode* dNode = (DocumentNode*)cur->data;
    Intersect[dNode->document_id] = dNode;
  }
  // Go through longer list and find matches. If match, copy into a new list
  for (cur = larger->head, _node = cur; _node != NULL; cur = _node = _node->next) {
    DocumentNode* dNode = (DocumentNode*)cur->data;
    if (Intersect[dNode->document_id] != NULL) {
      // Intersect, so copy over
      DocumentNode* in_smaller = Intersect[dNode->document_id];
      DocumentNode* newDNode = calloc(1, sizeof(DocumentNode));
      newDNode->document_id = dNode->document_id;
      newDNode->page_word_frequency = (in_smaller->page_word_frequency < dNode->page_word_frequency) ? in_smaller->page_word_frequency : dNode->page_word_frequency;
      list_append(list, newDNode);
    }
  }
  list_destroy(A);
  list_destroy(B);
  return list;
}

/*
* cmpDNode_freq - Helper function to compare DocumentNode types based
* on the number of word frequencies
*
*/
int cmpDNode_freq(const element_t av, const element_t bv) {
  DocumentNode* a = (DocumentNode*)av;
  DocumentNode* b = (DocumentNode*)bv;

  return (b->page_word_frequency - a->page_word_frequency);
}

/*
* cmpDNode_ID - Helper function to compare DocumentNode types
* based on document ID
*/
int cmpDNode_ID(const element_t av, const element_t bv) {
  DocumentNode* a = (DocumentNode*)av;
  DocumentNode* b = (DocumentNode*)bv;

  return (a->document_id == b->document_id);
}