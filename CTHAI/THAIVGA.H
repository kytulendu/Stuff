#ifndef THAIVGA_H_INCLUDED
#define THAIVGA_H_INCLUDED

#define CHAR_LEN_X  8
#define CHAR_LEN_Y  20
#define FONT_LENGTH  256

typedef unsigned char font[CHAR_LEN_Y];      /* One char font */

int load_Tfont( font *fn, char *name );
int Thai_Len( unsigned char st[] );
int Thai_Hight( );
int Thai_Width( unsigned char st[] );
void Set_TColor( int col );
int Get_TColor( );
void Set_TFont( font *fn );
font *Get_TFont( );
void Out_ThaiV( int x, int y, unsigned char st[] );
void Out_ChV( int x, int y, char ch );

#endif