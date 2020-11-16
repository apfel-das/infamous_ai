#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*
	Initialize head and tail pointers of List as NULL.
*/
void initList(list *l)
{
	l->head = NULL;
	l->tail = NULL;
}

/*
	Push a Move object to the beggining of the List.
*/

void push(list *l, Move* data)
{
	//fix linkage.
	Node* p = (Node*) malloc(sizeof(data));
	p = malloc(sizeof(Node));
	p->data = data;
	p->next = l->head;

	// decide where to assign the newly allocated element.
	
	// that's a Singleton Ring.
	if(l->head == NULL);
		l->tail = p;

	l->head = p;
}

/*
	Retrieve head's data.
*/

Move* top(list *l)
{
	
	if(l == NULL || l->head == NULL)
		return NULL;
	return l->head->data;
}

/*
	Pop the head of the list. 
*/

Move* pop(list *l)
{
	Move *retVal;

	if(l == NULL || l->head == NULL)
		return NULL;
	
	
	Node *p = l->head;
	l->head = l->head->next;
	

	if(l->head == NULL)
		l->tail = NULL;
	

	retVal = p->data;
	free(p);
	
	return retVal;
}


list * merge(list* A, list* B){
	assert (!(A==NULL && B==NULL));
	if (A == NULL || A->head == NULL)
		return B;
	if (B == NULL || B->head == NULL)	
		return A;
	A->tail->next = B->head;
	A->tail = B->tail;
	return A;
}

void freeList(list * l){
	while(l->head != NULL)
		free(pop(l));
	free(l);
}
void emptyList(list * l){
	while(l->head != NULL)
		free(pop(l));
}

void printMove(Move *aMove){
	int i;
	for (i = 0; i < MAXIMUM_MOVE_SIZE; i++){
		if(aMove->tile[0][i] == -1)
			break;
		printf("%d %d\n", aMove->tile[0][i], aMove->tile[1][i]);
	}
	printf("--\n");
}
void printList(list * l){
	Node *aNode = l->head;
	while(aNode != NULL){
		printMove(aNode->data);
		aNode = aNode->next;
	}
}

int max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}
int min(int a, int b)
{
	if(a < b)
		return a;
	return b;
}

