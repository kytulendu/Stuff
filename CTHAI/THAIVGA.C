#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <malloc.h>
#include <io.h>
#include <fcntl.h>

#include "vgalib.h"
#include "thaivga.h"

char tspace[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0
};

font *default_tfont;

int default_tcolor = 0x0f;

/*
Load Font 256 char into *name

return 0 if succsessful
return -1 if error
*/
int Load_TFont( font *fn, char *name ) {
	int fi, i;
	font buff;
	if ( ( fi = open( name, O_RDONLY | O_BINARY ) ) == -1 )
		return( -1 );
	for ( i = 0; i < FONT_LENGTH; i++ ) {
		read( fi, &( buff ), CHAR_LEN_Y );
		movedata( FP_SEG( buff ), FP_OFF( buff ), FP_SEG( fn[i] ), FP_OFF( fn[i] ), sizeof( font ) );
	}
	close( fi );
	return 0;
}

void Set_TFont( font *fn ) {
	default_tfont = fn;
}

font *Get_TFont( ) {
	return default_tfont;
}

void Set_TColor( int col ) {
	default_tcolor = col;
}

int Get_TColor( ) {
	return default_tcolor;
}

int Thai_Hight( ) {
	return CHAR_LEN_Y;
}

int Thai_Width( unsigned char st[] ) {
	return Thai_Len( st ) * CHAR_LEN_X;
}

int Thai_Len( unsigned char st[] ) {
	int i, l = 0;

	for ( i = 0; st[i]; i++ )
		l += tspace[st[i + 1]];

	return l;
}

void Out_ThaiV( int x, int y, unsigned char st[] ) {
	int i, j = 1;

	if ( st == NULL )
		return;
	for ( i = 0; st[i]; i++ ) {
		OUT_CH( x * j, y, default_tcolor, st[i], FP_SEG( default_tfont ), FP_OFF( default_tfont ) );
		x += tspace[st[i + 1]] * CHAR_LEN_X;
	}
}

void Out_ChV( int x, int y, char ch ) {
	OUT_CH( x, y, default_tcolor, ch, FP_SEG( default_tfont ), FP_OFF( default_tfont ) );
}
