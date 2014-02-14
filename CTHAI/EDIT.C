#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vgalib.h"
#include "thaivga.h"
#include "keyboard.h"
#include "edit.h"

extern char tspace[256];

char tlevel[] = {
	5,
	1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 5, 5, 5, 5,
	4, 4, 4, 4, 4, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5
};

char _level_map[6][6] = {
	0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 0, 0,
	0, 1, 1, 0, 0, 0,
	0, 0, 1, 1, 0, 0,
	0, 1, 1, 1, 0, 0,
	0, 1, 1, 1, 0, 0
};

int edit_bk_color = 1;

struct {
	int x, y, type, max_len, buf_len;
	char *buff;
	char old[80];
} *get_data;

int n_get = 0;
int _get = 0;

int Get_TLevel( unsigned char ch ) {
	if ( ch <= 0xA0 )
		return 5;
	if ( ( ch > 0xA0 ) && ( ch < 0xCF ) )
		return 0;
	if ( ch >= 0xF0 )
		return 5;
	return tlevel[ch - 0xCF];
}

int Level_Map( int i, int j ) {
	return _level_map[i][j];
}

void Set_EditBKColor( int col ) {
	edit_bk_color = col;
}
int Get_EditBKColor( ) {
	return edit_bk_color;
}

void Edit_Out( int x, int y, int start, int len, char *st ) {
	char temp[100];

	strcpy( temp, st );
	while ( Thai_Len( temp ) > start )
		temp[strlen( temp ) - 1] = 0;
	strcpy( temp, st + strlen( temp ) );
	while ( Thai_Len( temp ) > len )
		temp[strlen( temp ) - 1] = 0;
	Out_ThaiV( x, y, temp );
}

void Line_Edit( int x, int y, int n, int max, unsigned char *st ) {
	int cur_pos, start, index, ch, i;
	char temp[100];

	max--;
	strcpy( temp, st );
	index = strlen( st );
	if ( Thai_Len( st ) < n ) {
		start = 0;
		cur_pos = Thai_Len( st );
	} else {
		start = 0;
		for ( i = 0; Thai_Len( st + i ) > n; i++ )
			start += tspace[( unsigned char ) st[i]];
		start++;
		cur_pos = n - 1;
	}
	Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
	Edit_Out( x, y, start, n, st );
	while ( 1 ) {
		Show_Cursor( x + cur_pos * CHAR_LEN_X, y );
		ch = Get_KB( );
		if ( _get )
			if ( ( ch == KEY_UP ) || ( ch == KEY_DOWN ) || ( ch == KEY_ENTER ) || ( ch == KEY_ESC ) ) {
				ch = KEY_ENTER;
				Write_KB( Get_LastKey( ) );
			}
		if ( ch > 0xFF )
			switch ( ch ) {
			case KEY_HOME:
			{
				int old_start = start;
				cur_pos = 0;
				start = 0;
				index = 0;
				if ( old_start != start ) {
					Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
					Edit_Out( x, y, start, n, st );
				}
				break;
			}

			case KEY_END:
			{
				int old_start = start;
				index = strlen( st );
				if ( Thai_Len( st ) < n ) {
					start = 0;
					cur_pos = Thai_Len( st );
				} else {
					start = 0;
					for ( i = 0; Thai_Len( st + i ) > n; i++ )
						start += tspace[( unsigned char ) st[i]];
					start++;
					cur_pos = n - 1;
				}
				if ( old_start != start ) {
					Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
					Edit_Out( x, y, start, n, st );
				}
				break;
			}

			case KEY_DEL:
				if ( index < strlen( st ) ) {
					for ( i = index; st[i] != 0; i++ )
						st[i] = st[i + 1];
					while ( ( tspace[( unsigned char ) st[index]] == 0 ) && ( index < strlen( st ) ) )
						for ( i = index; st[i] != 0; i++ )
							st[i] = st[i + 1];
					Solid_Box( x + cur_pos* CHAR_LEN_X, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
					Edit_Out( x + cur_pos * CHAR_LEN_X, y, start + cur_pos, n - cur_pos, st );
				} else
					Beep( );
				break;

			case KEY_LEFT:
				if ( index > 0 ) {
					index--;
					if ( index > 0 )
					while ( tspace[( unsigned char ) st[index]] == 0 )
						index--;
					if ( cur_pos > 0 )
						cur_pos--;
					else {
						start--;
						Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
						Edit_Out( x, y, start, n, st );
					}
				}
				break;

			case KEY_RIGHT:
				if ( ( index < max ) && ( index < strlen( st ) ) ) {
					index++;
					while ( tspace[( unsigned char ) st[index]] == 0 )
						index++;
					if ( cur_pos < n - 1 )
						cur_pos++;
					else {
						start++;
						Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
						Edit_Out( x, y, start, n, st );
					}
				}
				break;

			case KEY_CTRL_LEFT:
			{
				int score = 0;
				do if ( index > 0 ) {
					index--;
					if ( index > 0 )
					while ( tspace[( unsigned char ) st[index]] == 0 )
						index--;
					if ( cur_pos > 0 )
						cur_pos--;
					else {
						start--;
						score = 1;
					}
				}
				while ( ( index > 0 ) && ( ( st[index] <= ' ' ) || ( st[index - 1] != ' ' ) ) );
				if ( score ) {
					Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
					Edit_Out( x, y, start, n, st );
				}
				break;
			}

			case KEY_CTRL_RIGHT:
			{
				int score = 0;
				do if ( ( index < max ) && ( index < strlen( st ) ) ) {
					index++;
					while ( tspace[( unsigned char ) st[index]] == 0 )
						index++;
					if ( cur_pos < n - 1 )
						cur_pos++;
					else {
						start++;
						score = 1;
					}
				}
				while ( ( index < strlen( st ) ) && ( ( st[index] != ' ' ) || ( st[index - 1] <= ' ' ) ) );
				if ( score ) {
					Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
					Edit_Out( x, y, start, n, st );
				}
				break;
			}
		} else
			switch ( ch ) {
			case KEY_ENTER:
				return;

			case KEY_ESC:
				strcpy( st, temp );
				cur_pos = 0;
				start = 0;
				index = 0;
				Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
				Edit_Out( x, y, start, n, st );
				return;

			case KEY_BACKSPACE:
				if ( index > 0 ) {
					int space;
					space = tspace[( unsigned char ) st[index - 1]];
					for ( i = index - 1; st[i] != 0; i++ )
						st[i] = st[i + 1];
					index--;
					if ( cur_pos > 0 ) {
						cur_pos -= space;
						Solid_Box( x + ( cur_pos - 1 + space ) * CHAR_LEN_X, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
						Edit_Out( x + ( cur_pos - 1 + space ) * CHAR_LEN_X, y, start + ( cur_pos - 1 + space ), n - ( cur_pos - 1 + space ), st );
					} else {
						start -= space;
						Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
						Edit_Out( x, y, start, n, st );
					}
				} else
					Beep( );
				break;

			default:
				if ( strlen( st ) >= max ) {
					Beep( );
					break;
				}
				if ( ( ch >= 32 ) && ( index < max ) ) {
					int space, err = 0;
					if ( index == 0 )
						switch ( Get_TLevel( ch ) ) {
						case 1:
						case 2:
						case 3:
							err = 1;
							break;
					} else
						err = Level_Map( Get_TLevel( st[index - 1] ), Get_TLevel( ch ) );
					if ( err ) {
						Beep( );
						break;
					}
					space = tspace[ch];
					for ( i = strlen( st ); i >= index; i-- )
						st[i + 1] = st[i];
					st[index] = ch;
					index++;
					if ( cur_pos < n - 1 ) {
						Solid_Box( x + ( cur_pos - 1 + space ) * CHAR_LEN_X, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
						Edit_Out( x + ( cur_pos - 1 + space ) * CHAR_LEN_X, y, start + ( cur_pos - 1 + space ), n - ( cur_pos - 1 + space ), st );
						cur_pos += space;
					} else {
						start += space;
						Solid_Box( x, y, x + n * CHAR_LEN_X, y + CHAR_LEN_Y, edit_bk_color );
						Edit_Out( x, y, start, n, st );
					}
				}
		}
	}
}

void Add_Get( int x, int y, int type, int max_len, int buf_len, char *buff ) {
	if ( n_get == 0 )
		get_data = malloc( sizeof( *get_data ) );
	else
		get_data = realloc( get_data, sizeof( *get_data ) * ( n_get + 1 ) );
	if ( get_data == NULL ) {
		printf( "Not enough memory" );
		exit( 1 );
	}
	get_data[n_get].x = x;
	get_data[n_get].y = y;
	get_data[n_get].type = type;
	get_data[n_get].max_len = max_len;
	get_data[n_get].buf_len = buf_len;
	get_data[n_get].buff = buff;
	strcpy( get_data[n_get].old, buff );
	n_get++;
}

void Read_Get( ) {
	int cur = 0, ch = 0, old_engonlykey = Get_EngOnlyKey( ), old_numonlykey = Get_NumOnlyKey( ), i;

	if ( n_get == 0 )
		return;
	for ( i = 0; i < n_get; i++ ) {
		Write_KB( KEY_ESC );
		Line_Edit( get_data[i].x, get_data[i].y, get_data[i].max_len, get_data[i].buf_len, get_data[i].buff );
	}
	_get = 1;
	Set_EngOnlyKey( 0 );
	Set_NumOnlyKey( 0 );
	do {
		switch ( get_data[cur].type ) {
		case G_THAI:
			Line_Edit( get_data[cur].x, get_data[cur].y, get_data[cur].max_len, get_data[cur].buf_len, get_data[cur].buff );
			break;
		case G_ENGLISH:
			Set_EngOnlyKey( 1 );
			Line_Edit( get_data[cur].x, get_data[cur].y, get_data[cur].max_len, get_data[cur].buf_len, get_data[cur].buff );
			Set_EngOnlyKey( 0 );
			break;
		case G_NUM:
			Set_NumOnlyKey( 1 );
			Line_Edit( get_data[cur].x, get_data[cur].y, get_data[cur].max_len, get_data[cur].buf_len, get_data[cur].buff );
			Set_NumOnlyKey( 0 );
			break;
		}
		ch = Get_KB( );
		switch ( ch ) {
		case KEY_UP:
			if ( cur > 0 )
				cur--;
			else
				cur = n_get - 1;
			break;
		case KEY_DOWN:
			cur++;
			if ( cur >= n_get )
				cur = 0;
			break;
		}
	} while ( ( ch != KEY_ENTER ) && ( ch != KEY_ESC ) );
	if ( ch == KEY_ESC )
		for ( i = 0; i < n_get; i++ )
			strcpy( get_data[i].buff, get_data[i].old );
	free( get_data );
	Set_EngOnlyKey( old_engonlykey );
	Set_NumOnlyKey( old_numonlykey );
	_get = 0;
	n_get = 0;
}
