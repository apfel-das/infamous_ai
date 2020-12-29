/**
* @file gameutils.h
* @author apfel-das
* @date 28 Dec, 2020
* @brief Header file for the implementation of a double linked list containing <Move> (s).
* @see "move.h"
*/

#ifndef _GAMEUTILS_H
#define _GAMEUTILS_H
#include "move.h"

/**
 * The basic entity of information concerning moves..
 */
typedef struct node
{
	Move *move;				// a Move entity..
	struct node *next;		// some sort of linkage..

} node;


/**
 * List entity containing encapsulated <Move> (s). Implemented as a circular linked list.
 */
typedef struct 
{
	node *head;				
	node *tail;

} movelist;


/**
 * @brief      Initializes a double linked list of <Moves> by setting fields to NULL.
 * @author     apfel-das
 * @param      l 	The head of the list.
 *     
 */

void list_init(movelist *l);

/**
 * @brief      Empties a list of moves.
 * @author     apfel-das
 * @param      l     The head of the list.
 */
void empty_list(movelist *l);

/**
 * @brief      Frees the whole list. Empties nodes and then whipes the head pointer as well.
 * @author     apfel-das
 * @param      l     The head of the list.
 * @note       Calls the "empty_list" function
 */

void free_list(movelist *l);

/**
 * @brief      Pushes a <Move> inside the list.
 * @author     apfel-das
 * @param      l     The head of the list.
 * @param      move  The move object.
 */
void push(movelist *l, Move *move);

/**
 * @brief      Pops the given l. Moves head on next.
 * @author 	   apfel-das
 * @param      l  	Element to be popped.
 * @return     The popped element's <Move>.
 */
Move *pop(movelist *l);

/**
 * @brief      Returns a reference to the current head's <Move> object.
 * @author 	   apfel-das
 * @param      l   The head of the list.
 * @return     The <Move> object on top.
 */
Move *top(movelist *l);



#endif