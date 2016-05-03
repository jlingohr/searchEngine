#ifndef _CRAWLER_H_
#define _CRAWLER_H_


// *****************Impementation Spec********************************
// File: crawler.c
// This file contains useful information for implementing the crawler:
// - DEFINES
// - MACROS
// - DATA STRUCTURES
// - PROTOTYPES

// DEFINES


#define INTERVAL_PER_FETCH 1

// The URL we crawled should only starts with this prefix. 
// You could remove this limtation but not before testing.
// The danger is a site may block you

#define URL_PREFIX "http://www.cs.dartmouth.edu"




#define MAX_DEPTH 4




//PROTOTYPES used by crawler.c You have to code them.

// getPage: Assumption: if you are dowloading the file it must be unique. 
// Two cases. First its the SEED URL so its unique. Second, wget only getpage 
//once a URL is computed to be unique. Get the HTML file saved in TEMP 
// and read it into a string that is returned by getPage. Store TEMP
// to a file 1..N and save the URL and depth on the first and second 
// line respectively.

char *getPage(char* url, int depth,  char* path);

// extractURL: Given a string of the HTML page, parse it (you have the code 
// for this GetNextURL) and store all the URLs in the url_list (defined above).
// NULL pointer represents end of list (no more URLs). Return the url_list

char **extractURLs(char* html_buffer, char* current);

// setURLasVisited: Mark the URL as visited in the URLNODE structure.

void setURLasVisited(char* url);

// updateListLinkToBeVisited: Heavy lifting function. Could be made smaller. It takes
// the url_list and for each URL in the list it first determines if it is unique.
// If it is then it creates a DDNODE (using malloc) and URLNODE (using malloc).
// It copies the URL to the URNODE and initialises it and then links it into the
// DNODE (and initialise it). Then it links the DNODE into the linked list dict.
// at the point in the list where its key cluster is (assuming that there are
// elements hashed at the same slot and the URL was found to be unique. It does
// this for *all* URL in the ur-list

void updateListLinkToBeVisited(char **url_list, int depth);

// getAddressFromTheLinksToBeVisited: Scan down thie hash table (part of dict) and
// find the first URL that has not already been visit and return the pointer 
// to that URL. Note, that the pointer to the depth is past too. Update the
// depth using the depth of the URLNODE that is next to be visited. 

char *getAddressFromTheLinksToBeVisited(int *depth);

int checkValidInputs(char* seed, char* target_dir, char* max_depth);

#endif
