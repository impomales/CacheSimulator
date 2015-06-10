#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.c"
#define NUM_OF_PARAM1 2
#define NUM_OF_PARAM2 16

// checks if number is a power of 2.
int isPow2(unsigned long long x);
// gets integer representation of the assoc string.
unsigned long long getAssoc(char *assoc, unsigned long long size, 
							unsigned long long blockSize);
// takes the logrithm of x, base 2.
unsigned long long logTwo(unsigned long long x);
// makes mask to extract bits from address.
unsigned long long makeMask(unsigned long long x);
// input error handling.
int error(int x);
// creates a struct containing the tag and setindex extracted from address.
addData makeData(unsigned long long tag, unsigned long long setIndex);
// takes address and manipulates it to get data using makeData and makeMask.
addData readAdd(pCache currCache, unsigned long long address,
									unsigned long long bitsAdd);
// scans file and sends each address to cache system.
pCache readFile(pCache currCache, char *file);
// searches cache, if hit return else go to next level cache.
void searchCache(pCache currCache, unsigned long long address, 
							unsigned long long bitsAdd);
// prints results.
void printResults(pCache FA, pCache currCache);
