#include "board.h"
#include <stdio.h>
#include <stdlib.h>



/**********************************************************/
void initPosition( Position * pos )
{
	int i, j;
	int rowsForEachPlayer;

	/* Calculate how many rows will be filled with pieces for each player */
	rowsForEachPlayer = ( BOARD_SIZE - 2 ) / 2;

	for( i = 0; i < (BOARD_ROWS); i++ )
	{
		for( j = 0; j < BOARD_COLUMNS; j++ )
		{
			pos->board[ i ][ j ] = EMPTY;	// first empty every square

			if( ( i + j ) % 2 == 1 )	// if ( i + j = odd ) then place piece
			{
				if( i < rowsForEachPlayer )
					pos->board[ i ][ j ] = WHITE;
				else if( i >= (BOARD_ROWS) - rowsForEachPlayer )
					pos->board[ i ][ j ] = BLACK;
				
				else if((i==5)||(i==6))	// Place food in central rows
					{
					if((rand() % 2)==0)
						pos->board[ i ][ j ] = RTILE;
					
				}
			}

		}
	}


	/* Reset score */
	pos->score[ WHITE ] = 0;
	pos->score[ BLACK ] = 0;

	/* Black plays first */
	pos->turn = BLACK;

}


/**********************************************************/
void printBoard( char board[ BOARD_ROWS ][ BOARD_COLUMNS ] )
{
	int i, j;

	/* Print the upper section */
	printf( "    " );
	for( i = 0; i < BOARD_COLUMNS; i++ )
		printf( "%d ", i );
	printf( "\n  +" );
	for( i = 0; i < 2 * BOARD_COLUMNS + 1; i++ )
		printf( "-" );
	printf( "+\n" );

	/* Print board */
	for( i = 0; i < BOARD_ROWS; i++ )
	{
		if (i>=10){
			printf( "%d| ", i );
		}
		else{
			printf( "0%d| ", i );
		}
		for( j = 0; j < BOARD_COLUMNS; j++ )
			switch( board[ i ][ j ] )
			{
				case WHITE:
					printf( "W " );
					break;
				case BLACK:
					printf( "B " );
					break;
				case EMPTY:
					printf( ". " );
					break;
				case ILLEGAL:
					printf( "# " );
					break;
				case RTILE:
					printf( "* " );
					break;

				default:
					printf("ERROR: Unknown character in board (printBoard)\n");
					exit( 1 );
			}
		if (i>=10){
			printf( "|%d \n", i );
		}
		else{
			printf( "|0%d \n", i );
		}
	}

	/* Print the lower section */
	printf( "  +" );
	for( i = 0; i < 2 * BOARD_COLUMNS + 1; i++ )
		printf( "-" );
	printf( "+\n" );
	printf( "    " );
	for( i = 0; i < BOARD_COLUMNS; i++ )
		printf( "%d ", i );
	printf( "\n" );

}


/**********************************************************/
void printPosition( Position * pos )
{
	//board
	printBoard( pos->board );

	//turn
	if( pos->turn == WHITE )
		printf( "Turn: WHITE" );
	else if( pos->turn == BLACK )
		printf( "Turn: BLACK" );
	else
		printf( "Turn: -" );

	printf( "\n" );

	//score
	printf( "Score is  W: %d  B: %d\n" , pos->score[ WHITE ], pos->score[ BLACK ] );

}


/**********************************************************/
void doMove( Position * pos, Move * moveToDo )
{
	int i, j;
	int intex = 1;
	int stepI, stepJ;

	assert( pos->turn == moveToDo->color );

	if( moveToDo->tile[ 0 ][ 0 ] == -1 )	//if null move, then simply change turn
	{
		pos->turn = getOtherSide( pos->turn );
		return;
	}

	while( moveToDo->tile[ 0 ][ intex ] != -1 )		//while we have tile available
	{
		pos->board[ moveToDo->tile[ 0 ][ intex - 1 ] ][ moveToDo->tile[ 1 ][ intex - 1 ] ] = EMPTY;	//remove piece

		if( abs( moveToDo->tile[ 0 ][ intex - 1 ] - moveToDo->tile[ 0 ][ intex ] ) > 1 )	//if we had jump
		{
			stepI = ( moveToDo->tile[ 0 ][ intex ] - moveToDo->tile[ 0 ][ intex - 1 ] ) / 2;
			stepJ = ( moveToDo->tile[ 1 ][ intex ] - moveToDo->tile[ 1 ][ intex - 1 ] ) / 2;

			pos->board[ moveToDo->tile[ 0 ][ intex - 1 ] + stepI ][ moveToDo->tile[ 1 ][ intex - 1 ] + stepJ ] = EMPTY;	//remove the captured piece
		}
		// Tile with food?
		if(pos->board[ moveToDo->tile[ 0 ][ intex  ] ][ moveToDo->tile[ 1 ][ intex  ] ] == RTILE){
			// Probability 1/2
			if((rand()%2)==0){
				if(moveToDo->color==BLACK){
					pos->score[ BLACK ]++;	//Black scored (food)!
				}
				else if(moveToDo->color==WHITE){
					pos->score[ WHITE ]++; //White scored (food)!
				}
			}				

		}

		if( moveToDo->tile[ 0 ][ intex ] == 0 )
			pos->score[ BLACK ]++;	//Black scored!
		else if( moveToDo->tile[ 0 ][ intex ] == BOARD_ROWS-1 )
			pos->score[ WHITE ]++;	//White scored!
		else
			pos->board[ moveToDo->tile[ 0 ][ intex ] ][ moveToDo->tile[ 1 ][ intex ] ] = moveToDo->color;	//place piece

		intex++;

		if( intex == MAXIMUM_MOVE_SIZE )	// if all move tiles used
			break;

	}

	/*change turn*/
	pos->turn = getOtherSide( pos->turn );

}


/**********************************************************/
int canJump( char row, char col, char player, Position * pos )
{
	int returnValue = 0;

	assert( ( player ==  WHITE ) || ( player == BLACK ) );
	assert( row >= 0 && row < BOARD_ROWS );
	assert( col >= 0 && col < BOARD_COLUMNS );

	if( player == WHITE )	//white player
	{
		if( row + 2 < BOARD_ROWS)
		{
			if( col - 2 >= 0 )
			{
				if( pos->board[ row + 1][ col - 1] == BLACK &&(( pos->board[ row + 2][ col - 2] == EMPTY) || ( pos->board[ row + 2][ col - 2] == RTILE)))
				{
					returnValue = returnValue + 1;	//left jump possible
				}
			}

			if( col + 2 < BOARD_COLUMNS )
			{
				if( pos->board[ row + 1][ col + 1] == BLACK && ((pos->board[ row + 2][ col + 2] == EMPTY ) || ( pos->board[ row + 2][ col + 2] == RTILE)))
				{
					returnValue = returnValue + 2;	//right jump possible
				}
			}

		}
	}
	else	//black player
	{
		if( row - 2 >= 0 )
		{
			if( col - 2 >= 0 )
			{
				if( pos->board[ row - 1][ col - 1] == WHITE && ((pos->board[ row - 2][ col - 2] == EMPTY )||(pos->board[ row - 2][ col - 2] == RTILE)))
				{
					returnValue = returnValue + 1;	//left jump possible
				}
			}

			if( col + 2 < BOARD_COLUMNS )
			{
				if( pos->board[ row - 1][ col + 1] == WHITE && ((pos->board[ row - 2][ col + 2] == EMPTY )||(pos->board[ row - 2][ col + 2] == RTILE)) )
				{
					returnValue = returnValue + 2;	//right jump possible
				}
			}

		}
	}

	return returnValue;

}

/**********************************************************/
int canJumpTo( char row, char col, char player, Position * pos, char rowDest, char colDest )
{

	int playerDirection;
	int jumpDirection;

	assert( ( player ==  WHITE ) || ( player == BLACK ) );
	assert( row >= 0 && row < BOARD_ROWS );
	assert( col >= 0 && col < BOARD_COLUMNS );
	assert( rowDest >= 0 && rowDest < BOARD_ROWS );
	assert( colDest >= 0 && colDest < BOARD_COLUMNS);


	jumpDirection = canJump( row, col, player, pos );

	if( jumpDirection == 0 )
		return FALSE;

	if( player == WHITE )
		playerDirection = 1;
	else
		playerDirection = -1;


	if( ( jumpDirection % 2 ) == 1 )	//if left jump possible
	{
		if( ( row + 2 * playerDirection == rowDest ) && ( col - 2 == colDest ) )
			return TRUE;
	}

	if( jumpDirection > 1 )	//if right jump possible
	{
		if( ( row + 2 * playerDirection == rowDest ) && ( col + 2 == colDest ) )
			return TRUE;
	}

	return FALSE;

}


/**********************************************************/
int canMove( Position * pos, char player )
{
	int i, j;

	assert( ( player ==  WHITE ) || ( player == BLACK ) );

	for( i = 0; i < BOARD_ROWS; i++ )
	{
		for( j = 0; j < BOARD_COLUMNS; j++ )
		{
			if( pos->board[ i ][ j ] == player )		//when we find a piece of ours
			{
				if( player == WHITE )	//white color
				{
					if( i + 1 < BOARD_ROWS )
					{
						if( j - 1 >= 0 )
							if( (pos->board[ i + 1 ][ j - 1 ] == EMPTY)|| (pos->board[ i + 1 ][ j - 1 ] == RTILE))		//check if we can move to the left
								return TRUE;
						if( j + 1 < BOARD_COLUMNS )
							if( (pos->board[ i + 1 ][ j + 1 ] == EMPTY )||(pos->board[ i + 1 ][ j + 1 ] == RTILE))		//check if we can move to the right
								return TRUE;
						if( canJump( i, j, WHITE, pos ) != 0 )			//check if we can make a jump
							return TRUE;
					}
				}
				else		//black color
				{
					if( i - 1 >= 0 )
					{
						if( j - 1 >= 0 )
							if( (pos->board[ i - 1 ][ j - 1 ] == EMPTY )||(pos->board[ i - 1 ][ j - 1 ] == RTILE))		//check if we can move to the left
								return TRUE;
						if( j + 1 < BOARD_COLUMNS )
							if( (pos->board[ i - 1 ][ j + 1 ] == EMPTY )||(	pos->board[ i - 1 ][ j + 1 ] == RTILE))	//check if we can move to the right
								return TRUE;
						if( canJump( i, j, BLACK, pos ) != 0 )			//check if we can make a jump
							return TRUE;
					}
				}
			}
		}
	}

	return FALSE;

}

/**********************************************************/
int isLegal( Position * pos, Move * moveToCheck )
{
	int i, j;
	int intex;


	if( moveToCheck->color != pos-> turn )	//not this player's turn..
		return FALSE;


	if( !canMove( pos, moveToCheck->color ) )	//if that player cannot move, the only legal move is null
	{
		if( moveToCheck->tile[ 0 ][ 0 ] == -1 )
			return TRUE;
		else
			return FALSE;
	}

	if( moveToCheck->tile[ 0 ][ 0 ] == -1 )	//since we can move, null move is illegal
		return FALSE;


	/* first coordinates must be inside board */
	if( ( moveToCheck->tile[ 0 ][ 0 ] < 0 ) || ( moveToCheck->tile[ 0 ][ 0 ] >= BOARD_ROWS) )
		return FALSE;
	if( ( moveToCheck->tile[ 1 ][ 0 ] < 0 ) || ( moveToCheck->tile[ 1 ][ 0 ] >= BOARD_COLUMNS ) )
		return FALSE;

	/* piece must be ours */
	if( pos->board[ moveToCheck->tile[ 0 ][ 0 ] ][ moveToCheck->tile[ 1 ][ 0 ] ] != moveToCheck->color )
		return FALSE;


	/* second coordinates must be inside board */
	if( ( moveToCheck->tile[ 0 ][ 1 ] < 0 ) || ( moveToCheck->tile[ 0 ][ 1 ] >= BOARD_ROWS) )
		return FALSE;
	if( ( moveToCheck->tile[ 1 ][ 1 ] < 0 ) || ( moveToCheck->tile[ 1 ][ 1 ] >= BOARD_COLUMNS ) )
		return FALSE;

	/* square must be empty */
	if( (pos->board[ moveToCheck->tile[ 0 ][ 1 ] ][ moveToCheck->tile[ 1 ][ 1 ] ] != EMPTY )&&(pos->board[ moveToCheck->tile[ 0 ][ 1 ] ][ moveToCheck->tile[ 1 ][ 1 ] ] != RTILE))
		return FALSE;

	/* if it seems to be a jump */
	if( ( abs( moveToCheck->tile[ 0 ][ 0 ] - moveToCheck->tile[ 0 ][ 1 ] ) == 2 ) && ( abs( moveToCheck->tile[ 1 ][ 0 ] - moveToCheck->tile[ 1 ][ 1 ] ) == 2 ) )
	{
		intex = 1;

		while( 1 )
		{
			if( !canJumpTo( moveToCheck->tile[ 0 ][ intex - 1 ], moveToCheck->tile[ 1 ][ intex -1 ], moveToCheck->color, pos, moveToCheck->tile[ 0 ][ intex ], moveToCheck->tile[ 1 ][ intex ] ) )
				return FALSE;

			/*if we reached all possible jumps for this board*/
			if( intex + 1 == MAXIMUM_MOVE_SIZE )
				return TRUE;

			/*no more jumps - we expect -1 to next row*/
			if( !canJump( moveToCheck->tile[ 0 ][ intex ], moveToCheck->tile[ 1 ][ intex ], moveToCheck->color, pos ) )
			{
				if( moveToCheck->tile[ 0 ][ intex + 1 ] == -1 )
					return TRUE;
				else
					return FALSE;
			}

			/* coordinates must be inside board - and NOT a -1 move */
			if( ( moveToCheck->tile[ 0 ][ intex + 1 ] < 0 ) || ( moveToCheck->tile[ 0 ][ intex + 1 ] >= BOARD_ROWS ) )
				return FALSE;
			if( ( moveToCheck->tile[ 1 ][ intex + 1 ] < 0 ) || ( moveToCheck->tile[ 1 ][ intex + 1 ] >= BOARD_COLUMNS ) )
				return FALSE;

			intex++;

		}

	}/* else if it seems to be a no-jump move */
	else if( ( abs( moveToCheck->tile[ 0 ][ 0 ] - moveToCheck->tile[ 0 ][ 1 ] ) == 1 ) && ( abs( moveToCheck->tile[ 1 ][ 0 ] - moveToCheck->tile[ 1 ][ 1 ] ) == 1 ) )
	{
		/*check direction*/
		if( ( moveToCheck->color == WHITE ) && ( moveToCheck->tile[ 0 ][ 1 ] - moveToCheck->tile[ 0 ][ 0 ] ) != 1 )
			return FALSE;
		if( ( moveToCheck->color == BLACK ) && ( moveToCheck->tile[ 0 ][ 1 ] - moveToCheck->tile[ 0 ][ 0 ] ) != -1 )
			return FALSE;

		/*seems legal..but we cannot be sure until we check if any jump is available*/
		for( i = 0; i < BOARD_ROWS; i++ )
		{
			for( j = 0; j < BOARD_COLUMNS; j++ )
			{
				if( pos->board[ i ][ j ] == moveToCheck->color )
					if( canJump( i, j, moveToCheck->color, pos ) )
						return FALSE;
			}
		}

		if( moveToCheck->tile[ 0 ][ 2 ] == -1 )
			return TRUE;


	}

	return FALSE;

}








