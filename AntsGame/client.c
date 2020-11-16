#include "global.h"
#include "board.h"
#include "move.h"
#include "comm.h"
#include "util.h"
#include "transp.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>


#define MAX_VAL 999999999
#define MAX_DEPTH 2
#define MAX_RUN_TIME 1


Move* chooseMove(Position *curr, int depth);
list* retrieveMoves(Position *aPosition);
unsigned int isStable(Position *curr);
int evalPos(Position *currPos);
void jumpAround(list* moveList, Move* move, int k,char i, char j, Position *aPosition);
int alpha_beta(Position *aPosition, char depth, int alpha, int beta, char maximizingPlayer, Move* finalMove);
int itterativeDeep(Position* aPosition, Move* finalMove);
int MTDF(Position *aPosition, int f, char d, Move *finalMove);


/**********************************************************/
Position gamePosition;		// Position we are going to use

Move moveReceived;			// temporary move to retrieve opponent's choice
Move myMove;				// move to save our choice and send it to the server

char myColor;				// to store our color
int mySocket;				// our socket

char * agentName = "chaos";		//default name.. change it! keep in mind MAX_NAME_LENGTH

char * ip = "127.0.0.1";	// default ip (local machine)
/**********************************************************/


int main( int argc, char ** argv )
{
	int c;
	opterr = 0;

	zobrist_init();
	fprintf(stdout, "%s\n","Zobrist Table Initialized" );
	hash_table_init();
	fprintf(stdout, "%s\n","Hash Table Initialized" );


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

	//zobrist_init();

	// TODO!!!
	//init a zobrist array.
	//init a hash table.

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
				fprintf(stdout, "ME PLAYING\n" );
				
				if( !canMove( &gamePosition, myColor ) )
				{
					fprintf(stdout, "%s\n","Hit unbound move error." );
					myMove.tile[0][0] = -1;		//null move
				}
				else
				{
					myMove = *chooseMove(&gamePosition, 0);
					//printf("%s will move from: (%d,%d), to: (%d,%d)\n",agentName,myMove.tile[0][0],myMove.tile[1][0],myMove.tile[0][1],myMove.tile[1][1]);
					/*while(!isLegal(&gamePosition, &myMove))
						myMove = *chooseMove(&gamePosition, 0); */
				}

				
				sendMove(&myMove, mySocket );	//send our move
				doMove(&gamePosition, &myMove);
				printPosition(&gamePosition);	


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


/* 
	Returns an assesment of the current position based on a heuristic and the game's board.

*/

int evalPos(Position *currPos) 
{
    unsigned int eval;
    
    for (int i = 0; i < BOARD_ROWS; i++)
    {
        for (int  j = 0; j < BOARD_COLUMNS; j++)
        {
        	//that's mine, let's asess it possitively.
            if (currPos->board[i][j] == myColor)
            {
            	eval += 100;
        		
        		// assign a small bonus symmetric to the color of the pionner.
            	if(myColor == WHITE)
               		eval += i*1;
               	else
               		eval += (BOARD_ROWS-i-1)*1;


           	}
            else if (currPos->board[i][j] == getOtherSide(myColor))
            {
            	//that's definetlly an opponent's, give it a penalty.
               	eval -= 100;
             
               	if(myColor == BLACK)
               		eval -= i*1;
               	else
               		eval -= (BOARD_ROWS-i-1)*1;
           	}

        }
    }
    // weights got calculated after a long day at work, be gentle with them.
    eval = eval + currPos->score[myColor]*90 - currPos->score[getOtherSide(myColor)]*90;

    return eval;
}

/*

	Decides what the next move will be based on a strategy.

*/



int alpha_beta(Position *aPosition, char depth, int alpha, int beta, char maximizingPlayer, Move* finalMove)
{  	
	//check transposition table
	Transp* curTransp= NULL;

	// case there is a valid transposition for given position.
	if(((curTransp = retrieveTransposition(aPosition)) != NULL) && ( finalMove == NULL ))
	{ 
		
		
		fprintf(stdout,"Debug: Upperdepth: %d, lowerDepth: %d, currentDepth: %d\n", curTransp->upperDepth, curTransp->lowerDepth, depth);
		if((curTransp->validity & 0x2)&&(curTransp->lowerBound >= beta)){
			if(curTransp->lowerDepth >= depth)
			{
				hitsLower++;
				return curTransp->lowerBound;
			}
		}

		if((curTransp->validity & 0x2))
			if(curTransp->lowerDepth >= depth)
				alpha = max(alpha, curTransp->lowerBound);


			
		if((curTransp->validity & 0x4) && (curTransp->upperBound <= alpha)){
			if(curTransp->upperDepth >= depth){
				hitsUpper++;
				return curTransp->upperBound;
			}
		}




		if((curTransp->validity & 0x4))
			if(curTransp->upperDepth >= depth)
				beta = min(beta, curTransp->upperBound);
	}
	

	// max depth of recursion has been reached.
	if (depth <= 0)
	{ 		

		//state is stable, a quiscence search says so!
		if(!isStable(aPosition))
		{
			//exiting.
			return evalPos(aPosition); 
		}
		
	}


	// Attempting to find any legal moves in this position.
	list* moveList = (list *)retrieveMoves(aPosition);   
	Move* tempMove = NULL;
	


	// moves not available, exiting.
	if (top(moveList) == NULL)
	{     
		freeList(moveList);
		return evalPos(aPosition);
	}

	Position* tempPosition = malloc(sizeof(Position));
	
	int tempScore, g;

	int a, b;


	if(maximizingPlayer)
	{

		g = -MAX_VAL;
		
		while((g<beta) && ((tempMove = pop(moveList)) != NULL ) )
		{ 
		//for each child position

			
			memcpy(tempPosition, aPosition, sizeof(Position));
			doMove(tempPosition, tempMove);

			tempScore = alpha_beta(tempPosition, depth-1, a, beta, 0, NULL);
			

			if(g <= tempScore)
			{
				
				g = tempScore;

				
				if(finalMove != NULL)
				{
					//if(isLegal(&gamePosition, tempMove))
					memcpy(finalMove, tempMove, sizeof(Move));
					


				}
					
				
				
			}
			
			printf("Debug on move(CHILD g<b).. move from: (%d,%d), to: (%d,%d) with g = %d\n",tempMove->tile[0][0],tempMove->tile[1][0],tempMove->tile[0][1],tempMove->tile[1][1], g);

			a = max(a, g);
			//free(tempMove);
			
			
		}
		
		return alpha;
	}
	else
	{
		g = MAX_VAL;
		b = beta;
		
		while( (g>alpha) && ((tempMove = pop(moveList)) != NULL) ) 
		{ 

		//for each child position

		

			memcpy(tempPosition, aPosition, sizeof(Position));
			doMove(tempPosition, tempMove);

			tempScore = alpha_beta(tempPosition, depth-1, alpha, b, 1, NULL);
			
			g = min(g, tempScore);
			b = min(b, g);

			printf("Debug on move(CHILD g>a).. move from: (%d,%d), to: (%d,%d) with g = %d\n",tempMove->tile[0][0],tempMove->tile[1][0],tempMove->tile[0][1],tempMove->tile[1][1] ,g);
			//free(tempMove);
			
		}
		
	}

	freeList(moveList);
	free(tempPosition);
	
	if(g <= alpha)
		saveUpperTransposition(aPosition, g, depth);
	if(g > alpha && g < beta)
		saveTransposition(aPosition, g, g, depth);
	if (g>= beta)
		saveLowerTransposition(aPosition, g, depth);
	



	return g;
}

/*
	Performs a quiescence search on the given state space / board.

*/


unsigned int isStable(Position *curr)
{
		
		//quiescence search
		// determine if we have a jump available
		for(int  i = 0; i < BOARD_ROWS; i++ )
		{
			for(int  j = 0; j < BOARD_COLUMNS; j++ )
			{
				if( curr->board[i][j] == curr->turn )
				{
					if( canJump(i , j, curr->turn,curr))
					{
						return 1;
					}
				}
			}
		}
		return 0;


}


list* retrieveMoves(Position *aPosition) 
{

	//markers.
	unsigned int jumpPossible = 0;
	unsigned int movePossible = 0;
	int playerDirection;
	list* moveList = malloc(sizeof(list));

	initList(moveList);
	Move *move;

	char curColor = aPosition->turn;

	if( curColor == WHITE )		// find movement's direction
		playerDirection = 1;
	else
		playerDirection = -1;



//Case no jump can be done, attempt to find any other move.

	for(int i = 0; i < BOARD_ROWS; i++ )
	{
		for(int j = 0; j < BOARD_COLUMNS; j++)
			{
				if( aPosition->board[i][j] == curColor )
				{
					// if a jumb can be done, just start from there.
					if(canJump( i, j, curColor, aPosition ) )
					{
						
							 
						move = malloc(sizeof(Move));
						move->color = curColor;
						jumpAround(moveList, move, 0, i, j, aPosition); 
						jumpPossible = 1;
					}
					if(jumpPossible == 0)
					{
						// see where can an ant go.
						movePossible = canPerformMove( i, j, curColor, aPosition);
						fprintf(stdout, "%s\n", "ASSESING" );

						if(movePossible % 2 == 1 && movePossible != 0) 
						{	

							//left move is possible to happen.
							
							move = malloc(sizeof(Move));
							move->color = aPosition->turn;
							move->tile[0][0] = i;
							move->tile[1][0] = j;
							move->tile[0][1] = i + playerDirection;
							move->tile[1][1] = j-1;
							move->tile[0][2] = -1;
							
							//if what came out has a legal form, enlist it.
							if(isLegal(aPosition, move))
							{
								fprintf(stdout, "%s\n","Assesed as VALID left" );
								printf("Debug on move.. move from: (%d,%d), to: (%d,%d)\n",move->tile[0][0],move->tile[1][0],move->tile[0][1],move->tile[1][1]);
								push(moveList, move);
							}
							else
								free(move);
						}
						if(movePossible > 1)
						{
							//right jumping can also be achieved, form it.
							move = malloc(sizeof(Move));
							move->color = aPosition->turn;
							move->tile[0][0] = i;
							move->tile[1][0] = j;
							move->tile[0][1] = i + playerDirection;
							move->tile[1][1] = j+1;
							move->tile[0][2] = -1;

							//enlist a legal action.
							if(isLegal(aPosition, move))
							{
								fprintf(stdout, "%s\n","Accesed as VALID right" );
								printf("Debug on move.. move from: (%d,%d), to: (%d,%d)\n",move->tile[0][0],move->tile[1][0],move->tile[0][1],move->tile[1][1]);
								push(moveList, move);
							}
							else
								free(move);
						}

					}

				}
			}
	}

		// nothing can be done.
	if(top(moveList) == NULL )
	{ 
		fprintf(stdout, "%s\n", "Algo stucked." );
		move = malloc(sizeof(Move));
		move->color = curColor;
		move->tile[0][0] = -1;
		push(moveList, move);
		return moveList;
	}


	return moveList;
						
}


/*
	Main tactic consists of a bunch of jumps. Not pretty smart but it will do the work.
*/

void jumpAround(list* moveList, Move* move, int k ,char i, char j, Position *aPosition){
	
	
	int possibleJumps, playerDirection;
	char pColor = move->color;
	move->tile[0][k] = i;
	move->tile[1][k] = j;

	//assertion step.

	if(MAXIMUM_MOVE_SIZE > k+1)
		return;

	if(!(possibleJumps = canJump(i, j, pColor, aPosition)))
	{
		move->tile[0][k+1] = -1;
		
		if(isLegal(aPosition, move))
		{

			push(moveList, move);
		}
		else
			free(move);
		return;
	}


	// up or down, in repsect to player's color.

	if(pColor == WHITE )		
		playerDirection = 1;
	else
		playerDirection = -1;


	//left jump
	if(possibleJumps == 1) 
	{
		jumpAround(moveList, move, k+1, i + 2*playerDirection, j-2, aPosition);
	}

	//right jump

	if(possibleJumps == 2) 
	{
		jumpAround(moveList, move, k+1, i + 2*playerDirection, j+2, aPosition);
	}

	//both jumps are valid, split them.
	if(possibleJumps == 3) 
	{
		//copy a move.
		Move* newMove = malloc(sizeof(Move));
		memcpy(newMove, move, sizeof(Move));
		
		//follow both dirs.

		jumpAround(moveList, move, k+1, i + 2*playerDirection, j-2, aPosition);
		jumpAround(moveList, newMove, k+1, i + 2*playerDirection, j+2, aPosition);

	}
	return;
}




int MTDF(Position *aPosition, int f, char d, Move *finalMove)
{
	int b;
	int g = f;
	int upperBound = MAX_VAL;
	int lowerBound = -MAX_VAL;

	Move* aMove = malloc(sizeof(Move));
	while (lowerBound < upperBound){
		
		//adapt b.
		if (g == lowerBound)
			b = g+1;
		else
			b = g;
		
		//recurse
		g = alpha_beta(aPosition, d, b-1, b, 1, aMove);
		
		//distinct bounds.
		if (g < b)
		{
			upperBound = g;
			
		}
		else
		{ 
			//remember the bound winner.
			memcpy(finalMove, aMove, sizeof(Move));
			lowerBound = g;
		}
	}
	free(aMove);
	return g;


}

int itterativeDeep(Position* aPosition, Move* finalMove)
{
	// make an initial guess.
	int f = evalPos(aPosition); 
	
	// define a depth as initial constrain and a timeout lenght.
	char d=1;
	clock_t cstart = clock();
	while(1)
	{
		f = MTDF(aPosition, f, d, finalMove);

		//print metrics
		if(((clock() - cstart)/CLOCKS_PER_SEC > MAX_RUN_TIME)||(d > MAX_DEPTH)){
			printf("======================================================\n");
			printf("Maximum f values: %d\n", f);
			printf("Time needed: %ld\n", (clock() - cstart)/CLOCKS_PER_SEC);
			printf("Depth of iteration: %d\n", d);
			printf("Transposition table hit rate. Upper: %f -- Lower: %f\n", (float)hitsUpper/(float)read_tries, (float)hitsLower/(float)read_tries);
			printf("Upper hit count: %d, Lower hit count: %d\n", hitsUpper, hitsLower);
			printf("Overwrite/save Ratio: %f\n", (float)overWrite/(float)saves);
			printf("======================================================\n");
			break;
		}
		d += 1;

	}
	return f;
}


Move* chooseMove(Position *curr, int depth)
{
	int maxGain = -MAX_VAL;
	// allocate space.
	Move *toPlay = (Move *)malloc(sizeof(Move));
	Position *currPos = (Position *)malloc(sizeof(Position));

	//cache the *curr postion ptr, not to affect its value.

	memcpy(currPos, curr, sizeof(Position));

	// TODO. Algo gen.
	int fGain = alpha_beta(currPos, MAX_DEPTH, maxGain, -maxGain, 1, toPlay);
	//itterativeDeep(currPos, toPlay);

	free(currPos);

	


	//return the move chooses from the algo gen.
	fprintf(stdout,"Debug on move(FATHER).. move from: (%d,%d), to: (%d,%d)\n",toPlay->tile[0][0],toPlay->tile[1][0],toPlay->tile[0][1],toPlay->tile[1][1]);
	return toPlay; 
}

