/**
* @file client.c
* @author Intelligence Lab (Antonis Vogiatzis, Charilaos Akasiadis, Ioannis Skoulakis).
* @author apfel-das (Konstantinos Pantelis), Christos Trimmas.
* @date 28 Dec, 2020
* @brief Actual entry point and implementation for agent's logic. 
* @brief The client responds to server's move request by sending a <Mffove> object. 
* @see "client.h"
*/



#include "global.h"
#include "board.h"
#include "move.h"
#include "comm.h"
#include "gameutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h> // Needed from isprint().
#include <unistd.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef DEBUG
#define DEBUG 1
#endif



movelist *generate_moves(Position *curr);
int diagonal_move_exists(char row, char col, char player, Position *pos);



/**********************************************************/
Position gamePosition;		// Position we are going to use

//Move moveReceived;			// temporary move to retrieve opponent's choice
Move myMove;				// move to save our choice and send it to the server

char myColor;				// to store our color
int mySocket;				// our socket

char * agentName = "chaos";		// name shouldn't be bigger than MAX_NAME_LENGTH

char * ip = "127.0.0.1";	// default ip (local machine)
/**********************************************************/

/**
 * @brief      Main entry point for client's functionality.
 * @author 	   Intelligence TUC.
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 * 
 */
int main( int argc, char ** argv )
{
	int c;
	opterr = 0;

	while( ( c = getopt ( argc, argv, "i:p:h" ) ) != -1 )
		switch( c )
		{
			case 'h':
				printf( "[-i ip] [-p port]\n" );
				return 0;
			case 'i':
				ip = optarg;
				break;
			case 'p':
				port = optarg;
				break;
			case '?':
				if( optopt == 'i' || optopt == 'p' )
					printf( "Option -%c requires an argument.\n", ( char ) optopt );
				else if( isprint(optopt) )
					printf( "Unknown option -%c\n", ( char ) optopt );
				else
					printf( "Unknown option character -%c\n", ( char ) optopt );
				return 1;
			default:
			return 1;
		}



	connectToTarget( port, ip, &mySocket );

	char msg;

	while( 1 )
	{

		msg = recvMsg( mySocket );
	
		switch ( msg )
		{
			case NM_REQUEST_NAME:		//server asks for our name
				sendName( agentName, mySocket );
				
				break;

			case NM_NEW_POSITION:		//server is trying to send us a new position
				
				getPosition( &gamePosition, mySocket );
				printPosition( &gamePosition );
				
				break;

			case NM_COLOR_W:			//server indorms us that we have WHITE color
				myColor = WHITE;
				printf("My color is %d\n",myColor);
				break;

			case NM_COLOR_B:			//server indorms us that we have BLACK color
				myColor = BLACK;
				printf("My color is %d\n",myColor);
				break;


			/**
			 * Actual Client's functionality when playing. 
			 * AI method calls and functions should have this as an entry point.
			 */

			case NM_REQUEST_MOVE:		//server requests our move
				myMove.color = myColor;
				
				

				if( !canMove( &gamePosition, myColor ) )
				{
					myMove.tile[ 0 ][ 0 ] = -1;		//null move
				}
				else
				{

					/*
						This is the main entry point for the implemented logic..

					*/


					fprintf(stderr, "[Generating Moves]..!\n");
					generate_moves(&gamePosition);
					fprintf(stderr, "[Generated Moves]..!\n");

					
					


				}
			
				
				//sendMove( &myMove, mySocket );			//send our move
				
				break;

			case NM_QUIT:			//server wants us to quit...we shall obey
				close( mySocket );
				return 0;

			default:
				printf("Wrong Signal!\n");
				return 0;

			
		}

	} 

	return 0;
}


/**
 * @brief      Generates possible moves from a given position.
 * @author     apfel-das
 * @note 	   Available moves can be diagonal moves [left, right], jumps [left, right] or both.
 * @note       Jump moves must be prioritized, therefore we insert them initally in the list.
 * @author     apfel-das
 * @param      curr  The current position on the board.
 * @warning    If no moves available a "null move" will be inserted in the movelist.
 * @return     A list of available moves from current Position.
 */

movelist *generate_moves(Position *curr)
{


	int jumpAvailable = FALSE;  //This acts as a jump flag, FALSE is actually 0 [global.h]
	int moveAvail = FALSE;
	char playerColor = curr->turn;		// Player's color (BLACK or WHITE).
	int playerDir = (playerColor == WHITE) ? 1 : -1;	// Movement towards bottom/surface of grid in respect to color.


	movelist *moves = (movelist *)malloc(sizeof(movelist ));

	//parse rows..
	for(int i = 0; i < BOARD_ROWS; i++)
	{
		//parse columns..
		for(int j = 0; j < BOARD_COLUMNS; j++)
		{
			//care only about player's assigned color.
			if(curr->board[i][j] == playerColor)
			{
				

				//if a jump can be made, find where will that be..
				if(canJump(i, j, playerColor, curr))
				{
					
					//if move chain broke, discard previous moves.
					if(!jumpAvailable)
						free_list(moves);

					//mark the event of finding a move [or more].
					jumpAvailable = TRUE;

				}

				moveAvail = diagonal_move_exists(i, j, playerColor, curr);

			
				//Debug printing..
				if(DEBUG)
				{
					char *dirs;

					if(moveAvail == 1)
						dirs = "left";
					else if(moveAvail == 2)
						dirs = "right";
					else if(moveAvail == 3)
						dirs = "left, right";
					else
						dirs = "nowhere";

					if(jumpAvailable && moveAvail)
						fprintf(stderr, "[Tile]: (%d, %d) has available jumps and diagonals.\n",i, j);
					else if (!jumpAvailable && moveAvail)
						fprintf(stderr, "[Tile]: (%d, %d) cannot jump but can move diagonally towards [%s].\n",i, j, dirs);
					else
						fprintf(stderr, "[Tile]: (%d, %d) is pinned down..!!!.\n",i, j);
				}


			}
		}

	}

	


	return NULL;

}




/**
 * @brief      Determines if diagonal move exists in respect with player's color.
 * @author     apfel-das
 * @param[in]  row     The row
 * @param[in]  col     The col
 * @param[in]  player  The player
 * @param      pos     The position
 * @note       Checks if the movements is in grid's bounds as well.
 * @note       Can also be used for an unoccupied piece.
 * @return     One of following values (0, 1, 2 or 3):
 * 
 * 				- 1 if left move exists. 
 * 				- 2 if right move exits. 
 * 				- 3 if both moves can be done. 
 * 				- 0 if no move is possible - pioneer is pinned down.
 */
int diagonal_move_exists(char row, char col, char player, Position *pos)
{
	int retVal = 0;

	//if not valid color, break.
	assert((player == WHITE) || (player == BLACK));

	//white players get to move diagonal downwards
	if(player == WHITE)
	{
		//must ensure we're still on the grid.
		if(row + 1 < BOARD_ROWS)
		{
			//we can move left diagonally
			if(col - 1 >= 0)
				if((pos->board[row+1][col-1] == EMPTY) || (pos->board[row+1][col-1] == RTILE))
					retVal += 1;
			
			//we can move right diagonally
			if(col + 1 > BOARD_COLUMNS)
				if((pos->board[row+1][col+1] == EMPTY) || (pos->board[row+1][col+1] == RTILE))
					retVal += 2;		

		}
	}
	else
	{
		//black player gets to move diagonal upwards

		//must ensure we're still on the grid.
		if(row - 1 >= 0)
		{
			//we can move left diagonally
			if(col - 1 >= 0)
				if((pos->board[row-1][col-1] == EMPTY) || (pos->board[row+1][col-1] == RTILE))
					retVal += 1;
			
			//we can move right diagonally
			if(col + 1 < BOARD_COLUMNS)
				if((pos->board[row-1][col+1] == EMPTY) || (pos->board[row+-1][col+1] == RTILE))
					retVal += 2;		

		}

	}



	return retVal;

}

