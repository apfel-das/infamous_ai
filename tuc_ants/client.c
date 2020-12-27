#include "global.h"
#include "board.h"
#include "move.h"
#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/**********************************************************/
Position gamePosition;		// Position we are going to use

//Move moveReceived;			// temporary move to retrieve opponent's choice
Move myMove;				// move to save our choice and send it to the server

char myColor;				// to store our color
int mySocket;				// our socket

char * agentName = "MyAgent!";		//default name.. change it! keep in mind MAX_NAME_LENGTH

char * ip = "127.0.0.1";	// default ip (local machine)
/**********************************************************/


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
				else if( isprint( optopt ) )
					printf( "Unknown option -%c\n", ( char ) optopt );
				else
					printf( "Unknown option character -%c\n", ( char ) optopt );
				return 1;
			default:
			return 1;
		}



	connectToTarget( port, ip, &mySocket );

	char msg;

/**********************************************************/
// used in random
	srand( time( NULL ) );
	int i, j, k;
	int jumpPossible;
	int playerDirection;
/**********************************************************/

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



			case NM_REQUEST_MOVE:		//server requests our move
				myMove.color = myColor;
				
				

				if( !canMove( &gamePosition, myColor ) )
				{
					myMove.tile[ 0 ][ 0 ] = -1;		//null move
				}
				else
				{

/**********************************************************/
// random player - not the most efficient implementation

					if( myColor == WHITE )		// find movement's direction
						playerDirection = 1;
					else
						playerDirection = -1;

					jumpPossible = FALSE;		// determine if we have a jump available
					for( i = 0; i < BOARD_ROWS; i++ )
					{
						for( j = 0; j < BOARD_COLUMNS; j++ )
						{
							if( gamePosition.board[ i ][ j ] == myColor )
							{
								if( canJump( i, j, myColor, &gamePosition ) )
									jumpPossible = TRUE;
							}
						}
					}

					while( 1 )
					{
						i = rand() % (BOARD_ROWS);
						j = rand() % BOARD_COLUMNS;

						if( gamePosition.board[ i ][ j ] == myColor )		//find a piece of ours
						{

							myMove.tile[ 0 ][ 0 ] = i;		//piece we are going to move
							myMove.tile[ 1 ][ 0 ] = j;

							if( jumpPossible == FALSE )
							{
								myMove.tile[ 0 ][ 1 ] = i + 1 * playerDirection;
								myMove.tile[ 0 ][ 2 ] = -1;
								if( rand() % 2 == 0 )	//with 50% chance try left and then right
								{
									myMove.tile[ 1 ][ 1 ] = j - 1;
									if( isLegal( &gamePosition, &myMove ))
										break;

									myMove.tile[ 1 ][ 1 ] = j + 1;
									if( isLegal( &gamePosition, &myMove ))
										break;
								}
								else	//the other 50%...try right first and then left
								{
									myMove.tile[ 1 ][ 1 ] = j + 1;
									if( isLegal( &gamePosition, &myMove ))
										break;

									myMove.tile[ 1 ][ 1 ] = j - 1;
									if( isLegal( &gamePosition, &myMove ))
										break;
								}
							}
							else	//jump possible
							{
								if( canJump( i, j, myColor, &gamePosition ) )
								{
									k = 1;
									while( canJump( i, j, myColor, &gamePosition ) != 0 )
									{
										myMove.tile[ 0 ][ k ] = i + 2 * playerDirection;
										if( rand() % 2 == 0 )	//50% chance
										{

											if( canJump( i, j, myColor, &gamePosition ) % 2 == 1 )		//left jump possible
												myMove.tile[ 1 ][ k ] = j - 2;
											else
												myMove.tile[ 1 ][ k ] = j + 2;

										}
										else	//50%
										{

											if( canJump( i, j, myColor, &gamePosition ) > 1 )		//right jump possible
												myMove.tile[ 1 ][ k ] = j + 2;
											else
												myMove.tile[ 1 ][ k ] = j - 2;

										}

										if( k + 1 == MAXIMUM_MOVE_SIZE )	//maximum tiles reached
											break;

										myMove.tile[ 0 ][ k + 1 ] = -1;		//maximum tiles not reached

										i = myMove.tile[ 0 ][ k ];		//we will try to jump from this point in the next loop
										j = myMove.tile[ 1 ][ k ];


										k++;
									}
									break;
								}
							}
						}

					}
// end of random
/**********************************************************/

				}
			
				
				sendMove( &myMove, mySocket );			//send our move
				
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






