#include <stdlib.h>
#include <mem.h>
#include <alloc.h>

#include "vgalib.h"
#include "thaivga.h"
#include "keyboard.h"
#include "menu.h"

int menu_pt = 0;
int menu_slide = 0;
int *_check_menu = NULL, n_check = 0;
int *_disable_menu = NULL, n_disable = 0;
int max_check_menu = 5, max_disable_menu = 5;
char check_st[] = "†";

int Is_Check_Menu( int com )
{
    int i;
    for ( i = 0; i<n_check; i++ )
        if ( _check_menu[i] == com ) return 1;
    return 0;
}

void Check_Menu( int com )
{
    if ( n_check == 0 )
    {
        _check_menu = calloc( max_check_menu, sizeof( *_check_menu ) );
        n_check = 1;
        _check_menu[0] = com;
    }
    else
    {
        if ( Is_Check_Menu( com ) ) return;
        if ( n_check >= max_check_menu ) return;
        _check_menu[n_check] = com;
        n_check++;
    }
}

void UnCheck_Menu( int com )
{
    int i;
    if ( !Is_Check_Menu( com ) ) return;
    for ( i = 0; i<n_check; i++ )
        if ( _check_menu[i] == com ) break;
    memcpy( _check_menu + i, _check_menu + i + 1, ( n_check - i - 1 )*sizeof( int ) );
    n_check--;
    if ( n_check == 0 ) free( _check_menu );
}

int Is_Disable_Menu( int com )
{
    int i;
    for ( i = 0; i<n_disable; i++ )
        if ( _disable_menu[i] == com ) return 1;
    return 0;
}

void Disable_Menu( int com )
{
    if ( n_disable == 0 )
    {
        _disable_menu = calloc( max_disable_menu, sizeof( *_disable_menu ) );
        n_disable = 1;
        _disable_menu[0] = com;
    }
    else
    {
        if ( Is_Disable_Menu( com ) ) return;
        if ( n_disable >= max_disable_menu ) return;
        _disable_menu[n_disable] = com;
        n_disable++;
    }
}

void UnDisable_Menu( int com )
{
    int i;
    if ( !Is_Disable_Menu( com ) ) return;
    for ( i = 0; i<n_disable; i++ )
        if ( _disable_menu[i] == com ) break;
    memcpy( _disable_menu + i, _disable_menu + i + 1, ( n_disable - i - 1 )*sizeof( int ) );
    n_disable--;
    if ( n_disable == 0 ) free( _disable_menu );
}

void go_next_menu( MenuItem mnu[] )
{
    if ( mnu[menu_pt].com != M_POPUP ) menu_pt++;
    else
    {
        int level = 1;
        for ( menu_pt++; level != 0; menu_pt++ )
            if ( mnu[menu_pt].com == M_POPUP ) level++;
                else if ( mnu[menu_pt].com == M_END ) level--;
    }
}

void go_before_menu( MenuItem mnu[] )
{
    menu_pt--;
    if ( mnu[menu_pt].com == M_END )
    {
        int level = 1;
        for ( menu_pt--; level != 0; menu_pt-- )
            if ( mnu[menu_pt].com == M_END ) level++;
                else if ( mnu[menu_pt].com == M_POPUP ) level--;
            menu_pt++;
    }
}

int nb_color = WHITE;
int n_color = BLACK;
int i_color = MAGENTA;
int ib_color = YELLOW;
int d_color = LIGHTGRAY;

void Set_Menu_Color( int normal, int normal_bk, int inverse, int inverse_bk, int disable )
{
    nb_color = normal_bk;
    n_color = normal;
    i_color = inverse;
    ib_color = inverse_bk;
    d_color = disable;
}

int Menu( int x, int y, MenuItem mnu[] )
{
    int width = 0, hight = Thai_Hight( ), i, ch, old_tcolor = Get_TColor( ), ret = 0, n;
    int old_eng_key = Get_EngOnlyKey( );
    char st[80], stc[2];

    Set_EngOnlyKey( 1 );

    for ( menu_pt = i = 0; mnu[menu_pt].com != M_END; i++, go_next_menu( mnu ) )
    {
        if ( width<Thai_Width( mnu[menu_pt].st ) )
            width = Thai_Width( mnu[menu_pt].st );
    }
    width += 16;
    n = i;
    Save_Screen( x - 1, y - 1, x + width + 1, y + i*hight + 1 );
    Rectangle( x, y, x + width + 1, y + n*hight + 1, d_color );
    Rectangle( x - 1, y - 1, x + width, y + n*hight, n_color );
    for ( menu_pt = i = 0; mnu[menu_pt].com != M_END; i++, go_next_menu( mnu ) )
    {
        Solid_Box( x, y + i*hight, x + width, y + ( i + 1 )*hight - 1, nb_color );
        if ( mnu[menu_pt].com == M_SEPARATOR )
        {
            int yy = y + i*hight + hight / 2;
            Line( x - 1, yy, x + width, yy, n_color );
        }
        else
        {
            Set_TColor( n_color );
            if ( Is_Disable_Menu( mnu[menu_pt].com ) )
                Set_TColor( d_color );
            Out_ThaiV( x + 8, y + i*hight, mnu[menu_pt].st );
            if ( Is_Check_Menu( mnu[menu_pt].com ) )
                Out_ThaiV( x, y + i*hight, check_st );
            if ( mnu[menu_pt].com == M_POPUP )
                Out_ThaiV( x + width - 8, y + i*hight, ">" );
            if ( mnu[menu_pt].hotkey >= 0 ) {
                strcpy( st, mnu[menu_pt].st );
                st[mnu[menu_pt].hotkey] = 0;
                stc[0] = mnu[menu_pt].st[mnu[menu_pt].hotkey];
                stc[1] = 0;
                Set_TColor( i_color );
                Out_ThaiV( x + 8 + 8 * Thai_Len( st ), y + i*hight, stc );
            }
        }
    }
    i = 0;
    ch = 0;
    menu_pt = 0;
    while ( ( mnu[menu_pt].com == M_SEPARATOR ) || ( Is_Disable_Menu( mnu[menu_pt].com ) ) )
    {
        go_next_menu( mnu ); i++;
        if ( mnu[menu_pt].com == M_END )
        {
            Write_KB( KEY_ESC );
            go_before_menu( mnu );
            i--;
            break;
        }
    }
    do {
        Solid_Box( x, y + i*hight, x + width, y + ( i + 1 )*hight - 1, ib_color );
        Set_TColor( i_color );
        if ( mnu[menu_pt].com == M_SEPARATOR )
        {
            int yy = y + i*hight + hight / 2;
            Line( x - 1, yy, x + width, yy, n_color );
        }
        else
        {
            if ( Is_Disable_Menu( mnu[menu_pt].com ) )
                Set_TColor( d_color );
            Out_ThaiV( x + 8, y + i*hight, mnu[menu_pt].st );
            if ( Is_Check_Menu( mnu[menu_pt].com ) )
                Out_ThaiV( x, y + i*hight, check_st );
            if ( mnu[menu_pt].com == M_POPUP )
                Out_ThaiV( x + width - 8, y + i*hight, ">" );
            if ( mnu[menu_pt].hotkey >= 0 ) {
                strcpy( st, mnu[menu_pt].st );
                st[mnu[menu_pt].hotkey] = 0;
                stc[0] = mnu[menu_pt].st[mnu[menu_pt].hotkey];
                stc[1] = 0;
                Set_TColor( i_color );
                Out_ThaiV( x + 8 + 8 * Thai_Len( st ), y + i*hight, stc );
            }
        }
        ch = Get_KB( );
        if ( menu_slide && ( Alt2Chr( ch ) != 0 ) ) { Write_KB( KEY_ESC ); Write_KB( ch ); }

        Solid_Box( x, y + i*hight, x + width, y + ( i + 1 )*hight - 1, nb_color );
        Set_TColor( n_color );
        if ( mnu[menu_pt].com == M_SEPARATOR )
        {
            int yy = y + i*hight + hight / 2;
            Line( x - 1, yy, x + width, yy, n_color );
        }
        else
        {
            if ( Is_Disable_Menu( mnu[menu_pt].com ) )
                Set_TColor( d_color );
            Out_ThaiV( x + 8, y + i*hight, mnu[menu_pt].st );
            if ( Is_Check_Menu( mnu[menu_pt].com ) )
                Out_ThaiV( x, y + i*hight, check_st );
            if ( mnu[menu_pt].com == M_POPUP )
                Out_ThaiV( x + width - 8, y + i*hight, ">" );
            if ( mnu[menu_pt].hotkey >= 0 ) {
                strcpy( st, mnu[menu_pt].st );
                st[mnu[menu_pt].hotkey] = 0;
                stc[0] = mnu[menu_pt].st[mnu[menu_pt].hotkey];
                stc[1] = 0;
                Set_TColor( i_color );
                Out_ThaiV( x + 8 + 8 * Thai_Len( st ), y + i*hight, stc );
            }
        }

        switch ( ch )
        {
        case KEY_UP:
k_up:
            if ( i>0 ) { go_before_menu( mnu ); i--; } else if ( i == 0 )
            {
                i = n - 1;
                while ( mnu[menu_pt].com != M_END ) go_next_menu( mnu );
                go_before_menu( mnu );
            }
            if ( ( mnu[menu_pt].com == M_SEPARATOR ) || ( Is_Disable_Menu( mnu[menu_pt].com ) ) )
                goto k_up;
            break;
        case KEY_DOWN:
k_dn:
            go_next_menu( mnu ); i++;
            if ( mnu[menu_pt].com == M_END )
            {
                i = 0;
                menu_pt = 0;
            }
            if ( ( mnu[menu_pt].com == M_SEPARATOR ) || ( Is_Disable_Menu( mnu[menu_pt].com ) ) )
                goto k_dn;
            break;
        case KEY_RIGHT:
            if ( mnu[menu_pt].com == M_POPUP )
                Write_KB( KEY_ENTER );
            else if ( menu_slide )
            {
                ch = KEY_ESC;
            }
            break;
        case KEY_LEFT:
            if ( menu_slide ) {
                ch = KEY_ESC;
            }
            break;
        case KEY_ENTER:
        {
            int old_menu_pt = menu_pt, old_i = i;
            if ( mnu[menu_pt].com == M_POPUP )
            {
                ret = Menu( x + width, y + i*hight, mnu + menu_pt + 1 );
                Set_TColor( n_color );
                menu_pt = old_menu_pt;
                i = old_i;
                if ( ret == 0 ) ch = 0;
                if ( menu_slide && ( Get_LastKey( ) == KEY_RIGHT ) )
                {
                    ch = KEY_ESC;
                    ret = 0;
                }
            }
        }
                       break;
        default:
        {   
            int old_menu_pt = menu_pt, old_i = i;
            ch = toupper( ch );
            if ( isalnum( ch ) )
            {
                menu_pt = i = 0;
                while ( mnu[menu_pt].com != M_END )
                {
                    if ( ( ch == toupper( mnu[menu_pt].st[mnu[menu_pt].hotkey] ) )
                        && ( mnu[menu_pt].com != M_SEPARATOR )
                        && !Is_Disable_Menu( mnu[menu_pt].com ) ) {
                        Write_KB( KEY_ENTER );
                        old_menu_pt = menu_pt;
                        old_i = i;
                        break;
                    }
                    go_next_menu( mnu );
                    i++;
                }
            }
            menu_pt = old_menu_pt;
            i = old_i;
        }
        }
    } while ( ( ch != KEY_ENTER ) && ( ch != KEY_ESC ) );

    Set_TColor( old_tcolor );
    Set_EngOnlyKey( old_eng_key );
    Restore_Screen( );
    if ( ch == KEY_ESC ) return 0;
    if ( mnu[menu_pt].com == M_POPUP ) return ret;
    return( mnu[menu_pt].com );
}

int Pull_Menu( int x, int y, int sp, MenuItem mnu[] )
{
    int width = 0, hight = Thai_Hight( ), i, ch, old_tcolor = Get_TColor( ), ret = 0, n;
    int xx[15];
    int old_eng_key = Get_EngOnlyKey( );
    char st[80], stc[2];

    Set_EngOnlyKey( 1 );

    for ( menu_pt = i = 0; mnu[menu_pt].com != M_END; i++, go_next_menu( mnu ) )
    {
        if ( mnu[menu_pt].com != M_END )
            width += Thai_Width( mnu[menu_pt].st ) + sp;
    }
    n = i;
    Set_TColor( n_color );
    xx[0] = x;
    width = x;
    for ( menu_pt = i = 0; mnu[menu_pt].com != M_END; i++, go_next_menu( mnu ) )
    {
        if ( mnu[menu_pt].com != M_END )
            xx[i + 1] = ( width += Thai_Width( mnu[menu_pt].st ) + sp );
        Solid_Box( xx[i], y, xx[i] + Thai_Width( mnu[menu_pt].st ) + 16, y + hight - 1, nb_color );
        Out_ThaiV( xx[i] + 8, y, mnu[menu_pt].st );
        if ( mnu[menu_pt].hotkey >= 0 ) {
            strcpy( st, mnu[menu_pt].st );
            st[mnu[menu_pt].hotkey] = 0;
            stc[0] = mnu[menu_pt].st[mnu[menu_pt].hotkey];
            stc[1] = 0;
            Set_TColor( i_color );
            Out_ThaiV( xx[i] + 8 + 8 * Thai_Len( st ), y, stc );
            Set_TColor( n_color );
        }
    }
    i = 0;
    ch = 0;
    menu_pt = 0;
    do {
        if ( !KB_Hit( ) )
        {
            Solid_Box( xx[i], y, xx[i] + Thai_Width( mnu[menu_pt].st ) + 16, y + hight - 1, ib_color );
            Set_TColor( i_color );
            Out_ThaiV( xx[i] + 8, y, mnu[menu_pt].st );
            if ( mnu[menu_pt].hotkey >= 0 )
            {
                strcpy( st, mnu[menu_pt].st );
                st[mnu[menu_pt].hotkey] = 0;
                stc[0] = mnu[menu_pt].st[mnu[menu_pt].hotkey];
                stc[1] = 0;
                Set_TColor( i_color );
                Out_ThaiV( xx[i] + 8 + 8 * Thai_Len( st ), y, stc );
            }
        }

        ch = Get_KB( );
        if ( Alt2Chr( ch ) != 0 ) ch = Alt2Chr( ch );

        Solid_Box( xx[i], y, xx[i] + Thai_Width( mnu[menu_pt].st ) + 16, y + hight - 1, nb_color );
        Set_TColor( n_color );
        Out_ThaiV( xx[i] + 8, y, mnu[menu_pt].st );
        if ( mnu[menu_pt].hotkey >= 0 )
        {
            strcpy( st, mnu[menu_pt].st );
            st[mnu[menu_pt].hotkey] = 0;
            stc[0] = mnu[menu_pt].st[mnu[menu_pt].hotkey];
            stc[1] = 0;
            Set_TColor( i_color );
            Out_ThaiV( xx[i] + 8 + 8 * Thai_Len( st ), y, stc );
        }

        switch ( ch )
        {
        case KEY_LEFT:
            if ( i>0 )
            {
                go_before_menu( mnu );
                i--;
            }
            else 
                if ( i == 0 )
                {
                    i = n - 1;
                    while ( mnu[menu_pt].com != M_END ) go_next_menu( mnu );
                    go_before_menu( mnu );
                }
            break;
        case KEY_RIGHT:
            go_next_menu( mnu ); i++;
            if ( mnu[menu_pt].com == M_END )
            {
                i = 0;
                menu_pt = 0;
            }
            break;
        case KEY_DOWN:
            Write_KB( KEY_ENTER );
            break;
        case KEY_ENTER:
        {
            int old_menu_pt = menu_pt, old_i = i;
            if ( mnu[menu_pt].com == M_POPUP )
            {
                menu_slide = 1;
                ret = Menu( xx[i], y + hight + 1, mnu + menu_pt + 1 );
                menu_slide = 0;
                Set_TColor( n_color );
                menu_pt = old_menu_pt;
                i = old_i;
                if ( ret == 0 ) ch = 0;
                switch ( Get_LastKey( ) )
                {
                case KEY_LEFT: Write_KB( KEY_LEFT ); Write_KB( KEY_ENTER ); break;
                case KEY_RIGHT: Write_KB( KEY_RIGHT ); Write_KB( KEY_ENTER ); break;
                }
            }
            break;
        }
        default:
        {
            int old_menu_pt = menu_pt, old_i = i;
            ch = toupper( ch );
            if ( isalnum( ch ) )
            {
                menu_pt = i = 0;
                while ( mnu[menu_pt].com != M_END )
                {
                    if ( ( ch == toupper( mnu[menu_pt].st[mnu[menu_pt].hotkey] ) ) && ( mnu[menu_pt].com != M_SEPARATOR ) )
                    {
                        Write_KB( KEY_ENTER );
                        old_menu_pt = menu_pt;
                        old_i = i;
                        break;
                    }
                    go_next_menu( mnu );
                    i++;
                }
            }
            menu_pt = old_menu_pt;
            i = old_i;
        }
        }
    } while ( ( ch != KEY_ENTER ) && ( ch != KEY_ESC ) );

    Set_TColor( old_tcolor );
    Set_EngOnlyKey( old_eng_key );
    //  free(xx);
    if ( ch == KEY_ESC ) return 0;
    if ( mnu[menu_pt].com == M_POPUP ) return ret;
    return( mnu[menu_pt].com );
}


