#include "queue.h"

pNode makeNode(pLine data, pNode next)
{
	pNode newNode = (pNode)malloc(sizeof(node));
	newNode->data = data;
	newNode->next = next;
	return newNode;
}

pQueue makeQueue()
{
	pQueue q = (pQueue)malloc(sizeof(queue));
	q->rear = NULL;
	return q;
}

void enqueue(pQueue q, pLine item)
{
	pNode newNode = makeNode(item, NULL);
	
	if (q->rear == NULL)
		newNode->next = newNode;
	else
	{
		newNode->next = q->rear->next;
		q->rear->next = newNode;
	}
	q->rear = newNode;
}

void dequeue(pQueue q)
{	
	pNode item;
	if (q->rear == NULL)
		return;
	item = q->rear->next;
	if (q->rear == q->rear->next)
		q->rear = NULL;
	else 
		q->rear->next = q->rear->next->next;
	item->next = NULL;
	free(item->data);
	free(item);
	return;
}

void update(pQueue q, pNode n, pNode p, addData d)
{
	if (n == q->rear)
		return;
	if (n == q->rear->next)
	{
		q->rear = n;
	}
	else
	{
		p->next = n->next;
		n->next = q->rear->next;
		q->rear->next = n;
		q->rear = n;
	}
}

void replaceLine(pQueue q, pLine item)
{	
	dequeue(q);
	enqueue(q, item);
}
