// data extracted from addresses in file.
typedef struct addData
{
	unsigned long long tag;
	unsigned long long setIndex;
} addData;
// holds tag and valid bit.
typedef struct line
{
	unsigned int isValid;
	unsigned long long tag;
} line, *pLine;

typedef struct node
{
	pLine data;
	struct node *next;
} node, *pNode;

typedef struct queue
{
	pNode rear;
} queue, *pQueue;

pNode makeNode(pLine data, pNode next);
pQueue makeQueue();
// adds to the rear.
void enqueue(pQueue q, pLine item);
// removes from the front.
void dequeue(pQueue q);
// brings node n to the rear. 
void update(pQueue q, pNode n, pNode p, addData dS);
// dequeues, then enqueues item.
void replaceLine(pQueue q, pLine item);
