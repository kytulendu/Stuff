#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define M_POPUP     -1
#define M_END       -2
#define M_SEPARATOR -3

typedef struct {
	char *st;
	int hotkey;
	int com;
} MenuItem;

extern int max_check_menu, max_disable_menu;

int Is_Check_Menu( int com );
void Check_Menu( int com );
void UnCheck_Menu( int com );
int Is_Disable_Menu( int com );
void Disable_Menu( int com );
void UnDisable_Menu( int com );
void Set_Menu_Color( int normal, int normal_bk, int inverse, int inverse_bk, int disable );
int Menu( int x, int y, MenuItem mnu[] );
int Pull_Menu( int x, int y, int sp, MenuItem mnu[] );

#endif