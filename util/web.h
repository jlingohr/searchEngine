/* ========================================================================== */
/* File: web.h
 *
 * Component name: Crawler
 *
 * This file contains the web/html processing functionality.
 *
 */
/* ========================================================================== */
#ifndef WEB_H
#define WEB_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality

// ---------------- Constants
#define MAX_TRY 3                            // maximum attempts to get a webpage

// ---------------- Structures/Types

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * GetWebPage - curl page->url, store into page->html
 * @page: the webpage struct containing the url to curl
 *
 * Returns 1 if the curl was successful; otherwise, 0. If the curl succeeded,
 * then page->html will contain the content retrieved. The WebPage struct should
 * have been allocated by the caller, but the page->html pointer is expected to
 * be NULL. If this function is successful, a new, null-terminated character
 * buffer will be allocated. It is the caller's responsibility to free this
 * memory.
 *
 * Students may find it convenient to modify this function to deal with
 * additional libcurl options or crawler functional requirements.
 *
 * Usage example:
 * WebPage* page = calloc(1, sizeof(WebPage));
 * page->url = "http://www.example.com";
 *
 * if(GetWebPage(page)) {
 *     printf("Found html: %s\n", page->html);
 *     free(page->html);
 *     free(page);
 * }
 */
int GetWebPage(WebPage *page);

/*
 * GetNextURL - returns the next url from html[pos] into result
 * @html: pointer to the html buffer
 * @pos: current position in html buffer
 * @base_url: the base url of the webpage, used to fixup relative links
 * @result: a pointer to character pointer, used to pass the url back out
 *
 * Returns the current position search so far in html; otherwise, returns < 0.
 * The html and base_url should both be valid character buffers. The pos argument
 * should be 0 on the initial call. The result argument should be a NULL pointer.
 * On successful parse of html, result will contain a newly allocated character
 * buffer; may be NULL on failed return. The caller is responsible for free'ing
 * this memory.
 *
 * Usage example: (retrieve all urls in a page)
 * int pos = 0;
 * char* result;
 * char* base_url = "http://www.example.com";
 *
 * while((pos = GetNextURL(html, pos, base_url, &result)) > 0) {
 *     printf("Found url: %s\n", result);
 *     free(result);
 * }
 *
 */
int GetNextURL(char *html, int pos, char *base_url, char **result);

/*
 * NormalizeURL - attempts to normalize the url
 * @url: absolute url to normalize
 *
 * Returns 1 on success; otherwise, 0 if the url can't be parsed or normalized.
 *
 * Usage example:
 * char* url = calloc(100, sizeof(char));
 * strcpy(url, "HTTP://UsEr:PaSs@www.EXAMPLE.com/path/.././file.html?name=val#top");
 *
 * if(NormalizeURL(url)) {
 *     // url should be: http://UsEr:PaSs@www.example.com/file.html?name=val#top
 * }
 */
int NormalizeURL(char *url);

/*
 * GetNextWord - returns the next word from doc[pos] into word
 * @doc: pointer to an html document
 * @pos: current position in the document
 * @word: a pointer to a c-style string, used to pass the word back out
 *
 * Returns the current position searched so far in doc; otherwise, returns < 0;
 * The doc should be a valid character buffer. The pos argument should be 0 on the
 * initial call. The word argument should be a NULL pointer. On successful parse
 * of the html in doc, word will contain a newly allocated character buffer;
 * may be NULL on failed return. The caller is responsible for free'ing this
 * memory.
 *
 * Usage example: (retrieve all words in a page)
 * int pos = 0;
 * char *word;
 * char *doc = "<ruh>Vox clamantis <roh> 3.0 < 5.0 /> in deserto.<raggy>";
 *
 * while((pos = GetNextWord(doc, pos, &word)) > 0) {
 *     // do something with word
 *     free(word);
 * }
 */
int GetNextWord(const char *doc, int pos, char **word);

/*
 * NormalizeWord - lowercases all the alphabetic characters in word
 * @word: the character buffer to normalize
 *
 * Word is modified in-place, with all uppercase letters lowered.
 *
 * Usage example:
 * char *str = "HELLO WORLD!";
 * NormalizeWord(str);
 * // str should now be "hello world!"
 */
void NormalizeWord(char *word);


#endif // WEB_Hwe