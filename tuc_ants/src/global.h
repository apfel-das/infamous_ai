#ifndef _GLOBAL_H
#define _GLOBAL_H


//#define _DEBUG_		//comment this line when you are done

#ifdef _DEBUG_
    #include <assert.h>
#else
    #define assert(p) {}
#endif


#ifndef getOtherSide
	#define getOtherSide( a ) ( 1-(a) )
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/**********************************************************/
#define MAXIMUM_MOVE_SIZE 6

/* The size of our board */
#define BOARD_COLUMNS 8
#define BOARD_ROWS 12
#define BOARD_SIZE 8

/* Values for each possible tile state */
#define WHITE 0
#define BLACK 1
#define EMPTY 2
#define RTILE 3
#define ILLEGAL 4

// max size of our name
#define MAX_NAME_LENGTH 16

//default port for client and server
#define DEFAULT_PORT "6001"


#endif
