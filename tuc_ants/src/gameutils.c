#include "gameutils.h"
#include <stdio.h>
#include <stdlib.h>

/*
 
 	Initializes a list of moves.
 	Args: 
 			- <movelist *l>: Pointer to a list of nodes containing <Move> nodes.
 	Returns:
 			- Nothing.

 */
void list_init(movelist *l)
{
	l->head = NULL;
	l->tail = NULL;
}


/*
 
 	Whipes out inner values of a list..

 	Args:
 			- <movelist *l>: Pointer to a list of nodes containing <Move> nodes.


 */

void empty_list(movelist *l)
{
	//whipe inner values out..
	while(l->head)
		free(pop(l));
	
}
/*
 
 	Whipes out the whole list..

 	Args:
 			- <movelist *l>: Pointer to a list of nodes containing <Move> nodes.


 */
void free_list(movelist *l)
{

	//Itterate through the list, free inner nodes..
	while(l->head)
		free(pop(l));

	//free outter block..
	free(l);
}

/*
 
	Push a <Move> into the movelist.
	
	Args:
			- <movelist *l>: Pointer to a list of nodes containing <Move> nodes.
			- <Move *move>:	 A <Move> to insert.		

 */

void push(movelist *l, Move *move)
{
	//Initialize a node.
	node *n = (node *)malloc(sizeof(node ));
	n->move = move;

	//Insert on top, fix the linkage..
	n->next = l->head;

	//If movelist empty, create a circular linkage..
	if (!l->head)
	{
		l->head = n;
		l->tail = n;	
	}
}


/*
 
	Pops the first element out of the list.
	Args:

			- <movelist *l>: Pointer to a list of nodes containing <Move> nodes.
	Returns:

			- The top of the list as a <* Move>.
			- NULL if empty head or actual object given.

*/

Move *pop(movelist *l)
{
	// check what's given..
	if (!l || !l->head)
		return NULL;
	
	// Instantiate a Move entity, get the head as node.
	Move *curr;
	node *n = l->head;

	//move head on next element..
	l->head = l->head->next;

	//one element list..
	if(!l->head)
		l->tail = NULL;

	curr = n->move;

	//free what's left..
	free(n);

	return curr;




}

/*
 	
 	Returns a reference to the top element..

 */

Move *top(movelist *l)
{
	if(!l->head || !l)
		return NULL;
	return l->head->move;
}