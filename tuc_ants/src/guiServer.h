#ifndef _GUISERVER_H
#define _GUISERVER_H

#include "global.h"
#include <gtk/gtk.h>
/**********************************************************/

typedef struct{
	char x;
	char y;
	char state;
} GuiTile;


/**********************************************************/
#define MAX_EXTERNAL_PLAYERS 10+2
/**********************************************************/
#define ST_EMPTY 0
#define ST_WHITE 1
#define ST_BLACK 2
#define ST_ILLEGAL 3
#define ST_RTILE 4
#define ST_EMPTY_LAST_MOVE_HIGHTLIGHT 5
#define ST_WHITE_LAST_MOVE_HIGHTLIGHT 6
#define ST_BLACK_LAST_MOVE_HIGHTLIGHT 7
#define ST_EMPTY_POSSIBLE_MOVE_HIGHTLIGHT 8
#define ST_EMPTY_SELECTED_HIGHTLIGHT 9
#define ST_WHITE_SELECTED_HIGHTLIGHT 10
#define ST_BLACK_SELECTED_HIGHTLIGHT 11
/**********************************************************/
void printMessage(char * message);

void printToGui( void );

void playRandom( void );

void checkVictoryAndSendMove( void );

void messageFromSocket( void );

void highlightLastMove( void );
void unhighlightLastMove( void );
void highlightSelection( int x, int y );
void unhighlightSelections( void );

void tile_selected( GtkWidget *widget, GdkEventButton *event, gpointer data );

void whiteCombo_changed( void );
void blackCombo_changed( void );

void playButton_clicked( void );
void stopButton_clicked( void );
void resetButton_clicked( void );

void closeConnections( void );
void quit( void );

void swapPlayers( void );

void takeCareDc( void );
void whiteDc( void );
void blackDc( void );

void new_connection( void );

GdkPixbuf *create_pixbuf( const gchar * filename );
/**********************************************************/

#endif
