
#ifndef _UTIL_H
#define _UTIL_H
#include "move.h"

/* 
A double linked list to assist.
*/

typedef struct Node
{

	Move* data;
	struct Node *next;
} Node;


typedef struct 
{
	Node *head;
	Node *tail;
} list;


//functions to perform ops of a double linked list.

void initList(list *l);
void push(list *l, Move* data);
Move* pop(list *l);
void freeList(list*);
list* merge(list *, list *);
Move* top(list *l);
void printList(list *);
void printMove(Move *someMove);
void emptyList(list *);
int max(int, int);
int min(int, int);
#endif
