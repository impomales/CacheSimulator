#include "cache-sim.h"

int main(int argc, char** argv)
{
	// error checking of cmd line args.
	if (argc != NUM_OF_PARAM1 && argc != NUM_OF_PARAM2)
		error(1);
	if (argc == NUM_OF_PARAM1)
	{
		if (strcmp(argv[1], "-h") == 0)
		{
			printf("\n\tHELP\n");
			printf("\t----\n");
			printf("Enter command line args in the following form.\n\n");
			printf("-l1size <size> -l1assoc <assoc>\n");
			printf("-l2size <size> -l2assoc <assoc>\n");
			printf("-l3size <size> -l3assoc <assoc>\n");
			printf("<blocksize> <replace> <file>\n\n");
			printf("cache size and blocksize should be a power of two.\n");
			return 0;
		} else error(1);
	}
	
	// error checking.
	if (strcmp(argv[1], "-l1size") || strcmp(argv[3], "-l1assoc") ||
		strcmp(argv[5], "-l2size") || strcmp(argv[7], "-l2assoc") ||
		strcmp(argv[9], "-l3size") || strcmp(argv[11], "-l3assoc"))
		error(1);
	//initialize variables for cache.
	unsigned long long blockSize = atoll(argv[13]);
	char *replace = argv[14];
	unsigned long long sizel1 = atoll(argv[2]);
	unsigned long long sizel2 = atoll(argv[6]);
	unsigned long long sizel3 = atoll(argv[10]);
	long long assoc1 = getAssoc(argv[4], sizel1, blockSize);
	long long assoc2 = getAssoc(argv[8], sizel2, blockSize);
	long long assoc3 = getAssoc(argv [12], sizel3, blockSize);
	int i;
	
	// more error checking.
	if (!isPow2(sizel1) || !isPow2(sizel2) || !isPow2(sizel3) ||
		!isPow2(blockSize))
		error(2);
	if (strcmp(replace, "FIFO") && strcmp(replace, "LRU") &&
			strcmp(replace, "fifo") && strcmp(replace, "lru"))
		error(1);
	for (i = 4; i <=12; i *= 4)
	{
		if (strcmp(argv[i], "direct") && strcmp(argv[i], "assoc") &&
			strncmp(argv[i], "assoc:", 6))
			error(1);
	}
	
	// create 3 level cache where l1 links to l2, l2 to l3.
	pCache L1 = makeCache(sizel1, blockSize, assoc1, replace);
	pCache L2 = makeCache(sizel2, blockSize, assoc2, replace);
	pCache L3 = makeCache(sizel3, blockSize, assoc3, replace);
	L1->next = L2;
	L2->level += L1->level;
	L2->next = L3;
	L3->level += L2->level;
	L3->next = NULL;
	pCache FA = readFile(L1, argv[15]);
	printResults(FA, L1);

	freeCache(L1);
	freeCache(L2);
	freeCache(L3);
	freeCache(FA);
	return 0;
}

int isPow2(unsigned long long x)
{
	return ((x != 0) && ((x & (x - 1)) == 0));
}

unsigned long long getAssoc(char *assoc, unsigned long long size, 
							unsigned long long blockSize)
{
	unsigned long long n = 0;

	if (strcmp(assoc, "direct") == 0)
		n = 1;
	else if (strcmp(assoc, "assoc") == 0)
		n = size/blockSize;
	// get n from the string assoc:n.
	else 
	{
		char str[64];
		strcpy(str, assoc + 6);
		n = atoll(str);
	}
	if (n <= 0)
	{
		error(1);
	}
	return n;
}

int error(int x)
{
	if (x == 1)
		fprintf(stderr, "ERROR: Invalid Arguements\n");
	if (x == 2)
		fprintf(stderr, "ERROR: blocksize and cache sizes must be a power of 2\n");
	if (x == 3)
		fprintf(stderr, "ERROR: file size not found...\n");
	exit(x);
}

addData makeData(unsigned long long tag, unsigned long long setIndex)
{
	addData newData = {tag, setIndex};
	return newData;
}

unsigned long long logTwo(unsigned long long x)
{
	// calculates logx by counting number of places the 1 bit is in a pow2 number.
	unsigned long long exp = 0;
	if (x >> 1 == 0) 
		return exp;
	while (x >> 1 != 0) {
		exp++;
		x >>= 1;
	}
	return exp;
}

unsigned long long makeMask(unsigned long long x)
{	
	// makes x bit mask by shifting 1 x times then subtracting by 1.
	if (x == 0)
		return x;
	return ((unsigned long long)1 << x) - 1;
}

addData readAdd(pCache currCache, unsigned long long address,
									unsigned long long bitsAdd)
{
	unsigned long long bitsOffset = logTwo(currCache->blockSize);
	unsigned long long bitsSet = logTwo(currCache->numOfSets);
	unsigned long long maskS = makeMask(bitsSet);
	unsigned long long maskT = makeMask(bitsAdd - (bitsOffset +bitsSet));
	unsigned long long setIndex = address >> bitsOffset;
	unsigned long long tag = address >> (bitsOffset + bitsSet);

//	assert(tag == (tag & maskT));
	tag &= maskT;
	setIndex &=maskS;
	

	return makeData(tag, setIndex);
}

pCache readFile(pCache currCache, char *file)
{
	FILE *fp;
	fp = fopen(file, "r");
	//FILE *fp2;
	//fp2 = fopen("file_out.txt", "w");
	char str[100];
	char str2[100];
	unsigned long long address;
	unsigned long long bitsAdd;
	pCache FA = makeCache(currCache->size, currCache->blockSize, 
				currCache->size/currCache->blockSize, "LRU");
	FA->next = NULL;
	FA->level = -1;
	int i = 0;

	if (fp == NULL)
		error(3);
	while(fgets(str, 100, fp) != NULL)
	{
		while (str[i] != '\n' && str[i] != '\0')
		{
			i++;
		}
		strncpy(str2, str, i);
		if (str[i] == '\n')
		{
			str2[i] = '\0';
		}
		bitsAdd = (strlen(str2) - 2) * 4;
		address = strtoull(str2, NULL, 16);
		//fprintf(fp2, "0x%llx\n", address);
		searchCache(FA, address, bitsAdd);
		searchCache(currCache, address, bitsAdd);
		// fully assoc cache of same size as l1.
		i = 0;
	}

	fclose(fp);
	//fclose(fp2);
	return FA;
}

void searchCache(pCache currCache, unsigned long long address, 
									unsigned long long bitsAdd)
{
	currCache->memAcc++;
	addData data = readAdd(currCache, address, bitsAdd);
	set s = currCache->sets[data.setIndex];
	pNode n = s.lines->rear->next;
	pNode p = s.lines->rear;
	// search queue at set[setIndex] in currCache.
	do
	{
		// hit if valid and matching tags.
		if (n->data->isValid && n->data->tag == data.tag)
		{
			currCache->hits++;
			// lru brings recent hits to rear.
			if (strcmp(currCache->algo, "LRU") == 0 ||
				strcmp(currCache->algo, "lru") == 0)
				update(s.lines, n, p, data);
			//printf("address-hit: %llx\n", address);
			//printCache(currCache);
			return;
		}
		p = n;
		n = n->next;
	} while (n != s.lines->rear->next);
	// not found, miss. replace.
	// if not full, replaces line at front.
	currCache->misses++;
	if (!n->data->isValid)
		currCache->coldMisses++;
	pLine item = makeLine();
	item->isValid = 1;
	item->tag = data.tag;
	replaceLine(s.lines, item);
	//printf("address-miss: %llx\n", address);
	//printCache(currCache);
	if (currCache->next == NULL)
		return;
	// go to next level cache.
	searchCache(currCache->next, address, bitsAdd);
}

void printResults(pCache FA, pCache currCache)
{
	int i = 0;
	pCache p = currCache;
	unsigned long long conf = 0;
	unsigned long long cap = 0;
	printf("Memory accesses: %llu\n", currCache->memAcc);
	for (i = 1; i <= 3; i++, p = p->next)
	{
		printf("L%d Cache hits: %llu\n", i, p->hits);
		printf("L%d Cache miss: %llu\n", i, p->misses);
	}
	p = currCache;
	for (i = 1; i <= 3; i++, p = p->next)
	{
		printf("L%d Cold misses: %llu\n", i, p->coldMisses);
	}
	conf = currCache->misses - FA->misses;
	cap = currCache->misses - currCache->coldMisses - conf;
	printf("L1 Conflict misses: %llu\n", conf);
	printf("L1 Capacity misses: %llu\n", cap);
}
