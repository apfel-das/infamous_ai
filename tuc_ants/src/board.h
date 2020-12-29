/**
* @file board.h
* @author Intelligence TUC.
* @date Sometime in 2012
* @brief Header file for the implementation of TUC-Ants board.
*/


#ifndef _BOARD_H
#define _BOARD_H

#include "global.h"
#include "move.h"
/**********************************************************/

/* Position struct to store board, score and player's turn */
typedef struct
{
	char board[ BOARD_ROWS ][ BOARD_COLUMNS ];
	char score[ 2 ];
	char turn;
} Position;


/**********************************************************/


/**
 * @brief      Initializes the position.
 * @author 	   Intelligence TUC.
 * @param      pos   The position
 */

void initPosition( Position * pos );

/**
 * @brief      Prints a board.
 * @author 	   Intelligence TUC.
 * @param      board  The board
 */
void printBoard( char board[ BOARD_ROWS][ BOARD_COLUMNS ] );

/**
 * @brief      Prints a position along with player's turn and score.
 * @author 	   Intelligence TUC.
 * @param      pos   The position
 */

void printPosition( Position * pos );

/**
 * @brief      Does a move on the position specified.
 * @author     Intelligence TUC.
 * @param      pos       The position
 * @param      moveToDo  The move to do
 * @warning    Does not check if move is legal! It simply does the move.
 */

void doMove( Position * pos, Move * moveToDo );

/**
 * @brief      Determines ability to jump.
 * @author 	   Intelligence TUC.
 * @param[in]  row     The row
 * @param[in]  col     The col
 * @param[in]  player  The player
 * @param      pos     The position
 * @note 	   It can also be used to determine if we can make a jump from a position we do not occupy. This would happen by providing the proper [row, col] tuple of the desired cell.
 * @return     One of following values (0, 1, 2 or 3):
 * 
 * 				- 1 if left jump can be done. 
 * 				- 2 if right jump can be done. 
 * 				- 3 if both directions can be jumped. 
 * 				- 0 if no jump is possible.
 * @warning    Does not check if we are inside the board.
 * 
 * 
 */

int canJump( char row, char col, char player, Position * pos );


/**
 * @brief      Determines ability to jump to a specific cell.
 * @author 	   Intelligence TUC.
 * @param[in]  row      The row
 * @param[in]  col      The col
 * @param[in]  player   The player
 * @param      pos      The position
 * @param[in]  rowDest  The row destination
 * @param[in]  colDest  The col destination
 * @note 	   It can also be used to determine if we can make a jump from a position we do not occupy just like canJump().
 * @return     True if able to jump to, False otherwise.
 * @warning    Does not check if we are inside the board.
 */

int canJumpTo( char row, char col, char player, Position * pos, char rowDest, char colDest );


/**
 * @brief      Determines ability to move on any direction.
 * @author     Intelligence TUC.
 * @param      pos     The position
 * @param[in]  player  The player
 *
 * @return     True if able to move, False otherwise.
 */
int canMove( Position * pos, char player );

/**
 * @brief      Determines if a move is legal.
 * @author     Intelligence TUC.
 * @param      pos          The position
 * @param      moveToCheck  The move to check
 * @return     True if legal, False otherwise.
 */
int isLegal( Position * pos, Move * moveToCheck );


#endif
