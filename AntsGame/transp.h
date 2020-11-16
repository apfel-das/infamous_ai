
#ifndef _TRANSP_H
#define _TRANSP_H
#include "board.h"
#include "move.h"


#define MAX_TABLE_SIZE 10000
#define OPEN_ADDR 1000  //handling possible collisions.



typedef struct {
	unsigned long zobrist_key;
	int upperBound;
	int lowerBound;
	char upperDepth;
	char lowerDepth;
	char validity; //LSB total validity, next lowerBound, next upperbound
}Transp;


typedef struct {
	long zobrist_table[BOARD_COLUMNS*BOARD_ROWS][2];
	long scores[2][16];
} Zobrist_Init;

Zobrist_Init zobristTable;

int hitsUpper;
int hitsLower;
int read_tries;
int overWrite;
int saves;
Transp *hashTable;


void saveTransposition(Position *, int, int, char);
void saveLowerTransposition(Position *, int, char);
void saveUpperTransposition(Position *, int, char);

Transp* retrieveTransposition(Position *);

unsigned int simpleHash(long);
void hash_table_init();
void freeTable();
void zobrist_init();

unsigned long zobrist_hash(Position *);
#endif