#include "queue.c"

typedef struct set 
{
	pQueue lines;	// lines queue.
} set, *pSet;

typedef struct cache
{
	int level;
	unsigned long long size;
	unsigned long long blockSize;
	unsigned long long assoc;
	char *algo;							// LRU or FIFO
	pSet sets;							// set array.
	unsigned long long numOfSets;
	unsigned long long hits;
	unsigned long long misses;
	unsigned long long coldMisses;
	unsigned long long memAcc;			// only used with L1.
	struct cache *next;					// next level cache, else null.
} cache, *pCache;

// creates a line with invalid tag.
pLine makeLine();
// creates a set with a queue of size based on associativity.
pSet makeSet(long long assoc, unsigned long long size, unsigned long long blockSize);
// creates sets and lines for cache given param.
pCache makeCache(unsigned long long size, unsigned long long blockSize, 
												long long assoc, char *algo);

void freeCache(pCache currCache);
void printCache(pCache c);
