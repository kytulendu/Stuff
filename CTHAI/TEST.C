#include <stdlib.h>
#include <stdio.h>

#include "vgalib.h"
#include "thaivga.h"
#include "keyboard.h"
#include "showlogo.h"
#include "menu.h"
#include "edit.h"

#define COMMAND1 1
#define COMMAND2 2
#define COMMAND3 3
#define COMMAND4 4
#define COMMAND5 5
#define COMMAND6 6
#define COMMAND7 7
#define EXIT 100

MenuItem main_menu[] = {
    { "Menu 1", 5, M_POPUP },
    { "Menu 2", 5, COMMAND2 },
    { "Menu 3", 5, COMMAND3 },
    { "Menu 4", 5, COMMAND4 },
    { "Menu 5", 5, COMMAND5 },
    { "", 0, M_SEPARATOR },
    { "Menu 6", 5, COMMAND6 },
    { "", 0, M_END },
    { "Menu 7", 5, COMMAND7 },
    { "Exit", 0, EXIT },
    { "", 0, M_END }
};

int TestColorText( font *fn, font *ifn )
{

    int i, j, hi;
    char st[] = "the quick brown fox jumps over the lazy dog";

    Rectangle( 0, 0, 639, 479, WHITE );
    for ( j = 0; j < 16; j++ )
    {
        hi = Thai_Hight( );
        Solid_Box( 0, 0, 639, 479, 0 );
        for ( i = 0; i < 16; i++ )
            Solid_Box( 0, i * hi, Thai_Width( st ), i * hi + hi, i );
        Set_TColor( j );

        Set_TFont( fn );
        for ( i = 0; i < 8; i++ )
        {
            Out_ThaiV( i, i * 20, st );
        }

        Set_TFont( ifn );
        for ( i = 8; i < 16; i++ )
        {
            Out_ThaiV( 0, i * 20, st );
        }

        getch( );
    }
    return 0;
}

int TestThaiText( font *fn )
{

    int i, j, hi;
    char st1[] = "˜•••••••••••••••••••••••••••••••••••••••••••••••••••••••™";
    char st2[] = "–     à»ç¹Á¹ØÉÂìÊØ´»ÃÐàÊÃÔ°àÅÔÈ¤Ø³¤èÒ  ¡ÇèÒºÃÃ´Ò½Ù§ÊÑµÇìà´ÃÑ¨©Ò¹     –";
    char st3[] = "–¨§½èÒ¿Ñ¹¾Ñ²¹ÒÇÔªÒ¡ÒÃ              ÍÂèÒÅéÒ§¼ÅÒ­ÄÒà¢è¹¦èÒºÕ±Òã¤Ã  –";
    char st4[] = "–äÁè¶×Íâ·Éâ¡Ã¸áªè§«Ñ´ÎÖ´ÎÑ´´èÒ        ËÑ´ÍÀÑÂàËÁ×Í¹¡ÕÌÒÍÑª¬ÒÊÑÂ      –";
    char st5[] = "–»¯ÔºÑµÔ»ÃÐ¾ÄµÔ¡®¡ÓË¹´ã¨           ¾Ù´¨ÒãËé ¨êÐ æ ¨ëÒ ¹èÒ¿Ñ§àÍÂÏ  –";
    char st6[] = "š•••••••••••••••••••••••••••••••••••••••••••••••••••••••›";
    char st7[] = "ÊÓ¹Ç¹ÁÒµÃ°Ò¹";

    Rectangle( 0, 0, 639, 479, WHITE );

    Set_TFont( fn );
    Set_TColor( 2 );

    Out_ThaiV( 90, 15, st1 );
    Out_ThaiV( 90, 35, st2 );
    Out_ThaiV( 90, 55, st3 );
    Out_ThaiV( 90, 75, st4 );
    Out_ThaiV( 90, 95, st5 );
    Out_ThaiV( 90, 115, st6 );
    Out_ThaiV( 440, 135, st7 );

    getch( );
    return 0;
}

int TestThaiInput( font *fn )
{

    static char title[80] = "", filename[80] = "";

    Set_TFont( fn );

    Rectangle( 0, 0, 639, 479, WHITE );

    Solid_Box( 150, 170, 490, 270, LIGHTGRAY );
    Rectangle( 150, 170, 490, 270, LIGHTBLUE );
    Set_EditBKColor( 15 );
    Set_TColor( 0 );
    Out_ThaiV( 160, 180, "Title" );
    Out_ThaiV( 160, 220, "Filename" );
    Add_Get( 160, 200, G_THAI, 40, 80, title );
    Add_Get( 160, 240, G_ENGLISH, 40, 80, filename );
    Read_Get( );
    return 0;
}

int TestMenu( font *fn )
{

    int mnu;

    Set_TColor( 15 );
    Set_TFont( fn );
    Disable_Menu( COMMAND4 );
    Check_Menu( COMMAND5 );
    do {
        mnu = Menu( 0, 0, main_menu );
        switch ( mnu )
        {
        case COMMAND5:
            if ( Is_Check_Menu( COMMAND5 ) )
                UnCheck_Menu( COMMAND5 );
            else
                Check_Menu( COMMAND5 );
        case COMMAND1:
        case COMMAND2:
        case COMMAND3:
        case COMMAND4:
        case COMMAND6:
        case COMMAND7:
        {
            char st[80];
            sprintf( st, "You select command %d", mnu );
            Out_ThaiV( 0, 200, st );
            Get_KB( );
            Clear_Screen( );
        }
        }
    } while ( mnu != EXIT );
    return mnu;
}

int main( )
{

    static font fn[256], ifn[256];

    Set_Mode( VM_GRAPHIC );

    if ( Load_TFont( fn, "NORMAL.FON" ) )
        exit( 1 );
    if ( Load_TFont( ifn, "ITALIC.FON" ) )
        exit( 1 );

    TestColorText( fn, ifn );
    Clear_Screen( );

    TestThaiText( fn );
    Clear_Screen( );

    TestThaiInput( fn );
    Clear_Screen( );

    T_ShowLogo( 100, 100, "KMITT.LGO" );
    getch( );
    Clear_Screen( );

    TestMenu( fn );

    Set_Mode( VM_TEXT );
    return 0;
}
