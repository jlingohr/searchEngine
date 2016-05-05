/*
* Default use:
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILE NAME]
* 
* For testing ability to reconstruct the inverted index
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITEN FILENAME]
*/

int main(int argc, char** argv) {
  /*

  1. Program parameter processing

  2. Initialize data structures
       allocate Inverted_index, zero it, and set links to NULL.

  3. LOG( "Building the index" );

  4. Index *wordindex=buildIndexFromDirectory("argv[1]");

  5. saveFile (argv[2], wordindex );
     LOG( "done!");
     
  6. CleanDynamicList (wordindex)

  // if testing then proceed

  7.  If (5 == argc) then 

     8. LOG ("Testing index\n");

     9. Reload the index from the file and rewrite it to a new file
        wordindex = readFile(argv[3]);

    10. saveFile (argv[4], wordindex);
        LOG("test complete\n");

    11. CleanDynamicList (wordindex);

  12. Done.

  */
}