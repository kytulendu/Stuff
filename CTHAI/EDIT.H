#ifndef EDIT_H_INCLUDED
#define EDIT_H_INCLUDED

#define G_THAI    1
#define G_ENGLISH 2
#define G_NUM     3

void Set_EditBKColor( int col );
int Get_EditBKColor( );
void Edit_Out( int x, int y, int start, int len, char *st );
void Line_Edit( int x, int y, int n, int max, unsigned char *st );
void Add_Get( int x, int y, int type, int max_len, int buf_len, char *buff );
void Read_Get( );

#endif