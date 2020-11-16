

#include "transp.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(x,y) (((x) < (y)) ? (x) : (y))

void hash_table_init(){

	//allocate size.
	hashTable = malloc(sizeof(Transp)*(MAX_TABLE_SIZE+OPEN_ADDR));
	
	// we shouldn't be here.
	if(hashTable == NULL)
	{
		printf("Error on allocating a transposition table. Perhaps lowering the size in .h would solve the issue.\n");
		exit(0);
	}
	

	//initialize on zero.
	for (int i = 0; i < MAX_TABLE_SIZE+OPEN_ADDR; i++)
		hashTable[i].validity = 0;



}

/*
	Frees the hashTable ptr.
*/
void freeTable()
{
	free(hashTable);
}

/*
	Initializes a Zobrist Table by generating random longs.
*/
void zobrist_init()
{

		hitsUpper =0;
		hitsLower = 0;
		read_tries = 1;
		overWrite = 0;
		saves=1; // this is actually the 1 st one.
	
	for (int i=0; i < BOARD_COLUMNS*BOARD_ROWS; i++)
	{
		// moving 32 bits upwards.
		zobristTable.zobrist_table[i][0] = ((((long) rand() <<  0) & 0x00000000FFFFFFFFull) | (((long) rand() << 32) & 0xFFFFFFFF00000000ull));
		zobristTable.zobrist_table[i][1] = ((((long) rand() <<  0) & 0x00000000FFFFFFFFull) | (((long) rand() << 32) & 0xFFFFFFFF00000000ull));
	}

	//treat scoring table the same way.
	for(int i = 0; i < 16; i++)
	{
		zobristTable.scores[0][i] = ((((long) rand() <<  0) & 0x00000000FFFFFFFFull) | (((long) rand() << 32) & 0xFFFFFFFF00000000ull));
		zobristTable.scores[1][i] = ((((long) rand() <<  0) & 0x00000000FFFFFFFFull) | (((long) rand() << 32) & 0xFFFFFFFF00000000ull));
	}
}



/*
	Generates a Zobrist Hash for the current position of the Board.
*/

unsigned long zobrist_hash(Position *curr)
{
	int i,j;
	unsigned long h = 0;
	

	for (int i= 0; i < BOARD_ROWS; i++)
	{
		for (int j=0; j < BOARD_COLUMNS; j++)
		{
			if (curr->board[i][j] == WHITE ||curr->board[i][j] == BLACK )
				h = h ^ zobristTable.zobrist_table[i*BOARD_COLUMNS + j][(unsigned)curr->board[i][j]];
		}

	}

	//hash the scores
	h = h ^ zobristTable.scores[0][curr->score[0]];
	h = h ^ zobristTable.scores[1][curr->score[1]];

	return h;
}

/*
	Creates a plain hash given a seed.
*/

unsigned int simpleHash(long zobrist){

	unsigned int hash;

	long prime  = 948701839;
	long prime2 = 6920451961;

	//spread the values, avoid 'conflicts'.
	long scaled = (zobrist & 0xFFFFFFFF) * prime;

	//fill lower bits

	long shifted = scaled + prime2;

	long filled = shifted + ((shifted & 0xFFFFFFFF00000000) >> 32);

	hash = (unsigned int) (filled & 0xFFFFFFFF);

	hash = hash % MAX_TABLE_SIZE;

	return hash;


}


/*
	Saves any given update on the Transposition table.
*/

void saveTransposition(Position* currPos, int upperBound, int lowerBound, char depth)
{
	unsigned long key = zobrist_hash(currPos);
	unsigned int hash = simpleHash(key);
	saves++;

	int i=0;


	// open addresses.


	while ((hashTable[hash].validity & 0x1) && (hashTable[hash].zobrist_key != key) && (i < OPEN_ADDR))
	{
			i++;
			hash++;
	}

	// overwriten ones.
	if(( hashTable[hash].validity & 0x1) && ( hashTable[hash].zobrist_key != key))
		overWrite++;

	// ivalid.

	if(((hashTable[hash].validity = 0x7) && (hashTable[hash].upperDepth + hashTable[hash].lowerDepth >= 2*depth )) && ( rand() < RAND_MAX/2 ))
		return;

	hashTable[hash].zobrist_key = key;
	hashTable[hash].upperBound = upperBound;
	hashTable[hash].lowerBound = lowerBound;
	hashTable[hash].upperDepth = depth;
	hashTable[hash].lowerDepth = depth;

	hashTable[hash].validity = 0x7;
	return;

}


void saveUpperTransposition(Position* aPos, int upperBound,  char depth){
	unsigned long key = zobrist_hash(aPos);
	unsigned int hash = simpleHash(key);
	int i=0;
	saves++;
	while ((hashTable[hash].validity & 0x1)&&(hashTable[hash].zobrist_key != key)&&(i<OPEN_ADDR))
	{
		i++;
		hash++;
	}
	if((hashTable[hash].validity & 0x1)&&(hashTable[hash].zobrist_key != key))
		overWrite++;

	// update in case of correct positioning.
	if((hashTable[hash].validity & 0x1)&&(hashTable[hash].zobrist_key == key)) 
	{
		//overwrite only on shallower depths
		if(((hashTable[hash].validity & 0x4) && (hashTable[hash].upperDepth >= depth))&&(rand() > RAND_MAX/2))
			return;
		hashTable[hash].upperBound = upperBound;
		hashTable[hash].upperDepth = depth;
		hashTable[hash].validity = hashTable[hash].validity | 0x4;
		return;

	}
	
	hashTable[hash].zobrist_key = key;
	hashTable[hash].upperBound = upperBound;
	hashTable[hash].upperDepth = depth;
	hashTable[hash].validity = 0x5;

}



void saveLowerTransposition(Position* aPos, int lowerBound, char depth){
	unsigned long key = zobrist_hash(aPos);
	unsigned int hash = simpleHash(key);
	saves++;
	int i = 0;

	while ((hashTable[hash].validity & 0x1)&&(hashTable[hash].zobrist_key != key)&&(i<OPEN_ADDR))
	{
		i++;
			hash++;
	}
	if((hashTable[hash].validity & 0x1)&&(hashTable[hash].zobrist_key != key))
		overWrite++;

	if((hashTable[hash].validity & 0x1)&&(hashTable[hash].zobrist_key == key)) //already the correct position, then just update
	{
		if((hashTable[hash].validity & 0x2) && (hashTable[hash].lowerDepth >= depth)&&(rand() > RAND_MAX/2))
			return;
		hashTable[hash].lowerBound = lowerBound;
		hashTable[hash].lowerDepth = depth;
		hashTable[hash].validity = hashTable[hash].validity | 0x2;
		return;

	}

	hashTable[hash].zobrist_key = key;
	hashTable[hash].lowerBound = lowerBound;
	hashTable[hash].lowerDepth = depth;
	hashTable[hash].validity = 0x3;
	return;

}

/*
	Given a board position, following returns the Transposition value in it.
*/
Transp* retrieveTransposition(Position* aPosition){


	

	unsigned long zobrist = zobrist_hash(aPosition);
	unsigned int hash = simpleHash(zobrist);

	int i = 0;
	


	read_tries++;
	while((hashTable[hash].validity & 0x1) && (i < OPEN_ADDR))
	{	//a saved version of our posistion exists and IS VALID.

		if((hashTable[hash].zobrist_key == zobrist))
			return &hashTable[hash];
		i++;

	}
	return NULL;


}