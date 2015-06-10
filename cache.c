#include "cache.h"

pLine makeLine()
{
	pLine newLine = (pLine)malloc(sizeof(line));
	newLine->isValid = 0;
	newLine->tag = 0;
	return newLine;
}

pSet makeSet(long long assoc, unsigned long long size, unsigned long long blockSize)
{
	pSet newSet = (pSet)malloc(sizeof(set));
	newSet->lines = makeQueue();
	pLine newLine;
	int i;
	// sets start with a queue of invalid lines. 
	for (i = 0; i < assoc; i++)
	{
		newLine = makeLine();
		enqueue(newSet->lines, newLine);	
	}
	
	return newSet;
}

pCache makeCache(unsigned long long size, unsigned long long blockSize,
					long long assoc, char *algo)
{
	pCache newCache = (pCache)malloc(sizeof(cache));
	newCache->size = size;
	newCache->blockSize = blockSize;
	newCache->assoc = assoc;
	newCache->algo = algo;
	newCache->numOfSets = size/(assoc * blockSize);
	newCache->sets = (pSet)malloc(sizeof(set) * newCache->numOfSets);
	int i;
	// create (numOfSets) sets, each containing a queue with (assoc) lines.
	for (i = 0; i < newCache->numOfSets; i++)
	{
		newCache->sets[i] = *(makeSet(assoc, size, blockSize));
	}
	
	newCache->hits = 0;
	newCache->misses = 0;
	newCache->coldMisses = 0;
	newCache->memAcc = 0;
	newCache->next = NULL;
	newCache->level = 1;

	return newCache;
}

void freeCache(pCache currCache)
{
	int i;
	int j;

	for(i = 0; i < currCache->numOfSets; i++)
	{
		for (j = 0; j < currCache->assoc; j++)
		{
			dequeue(currCache->sets[i].lines);
		}
	}
	free(currCache->sets);
	free(currCache);
}

void printCache(pCache c)
{
	printf("level: %d\n", c->level);
	printf("size: %llu\n", c->size);
	printf("assoc: %llu\n", c->assoc);
	printf("blocksize: %llu\n", c->blockSize);
	int i;
	int j;
	pNode p;

	for (i = 0; i < c->numOfSets; i++)
	{
		printf("set%d: ", i);
		p = c->sets[i].lines->rear->next;
		for (j= 0; j < c->assoc; j++, p = p->next)
		{
			printf("\tline%d: ", j);
			printf("\ttag: %llx\tValid: %d\n", p->data->tag, p->data->isValid);
		}
		printf("\n");
	}
}
