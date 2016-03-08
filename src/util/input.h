#ifndef _INPUT_H
#define _INPUT_H

/*
  isValidSeed: Checks whether seed passed as parameter is a valid url. If
  invalid seed passed, exits program. A valid url will be a valid hostname.
  @param: seed - name of seed to be checked
  @return: url if valid, NULL otherwise.
**/
char* isValidSeed(char* seed);

/*
	isValidDir: Checks if passed directory is valid. Quits if not. A valid
	directory (1) exists, (2) is a directory, (3) is writable.
	@param: dir - name of the directory to be checked
**/
void isValidDir(char* dir);

#endif