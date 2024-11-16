#ifndef VGALIB_H_INCLUDED
#define VGALIB_H_INCLUDED

#if !defined(__COLORS)
#define __COLORS
enum COLORS {
    BLACK,                              /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,                           /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

#endif

#if !defined(__GRAPHICS_H)
enum putimage_ops { /* BitBlt operators for putimage */
    COPY_PUT,       /* MOV */
    XOR_PUT,        /* XOR */
    OR_PUT,         /* OR  */
    AND_PUT,        /* AND */
    NOT_PUT         /* NOT */
};
#endif

#define VM_GRAPHIC 0x12
#define VM_TEXT    0x03

#define set_writecolor(x) outport(0x03c4, 0x0100 * x + 0x0002)
#define set_readplane(x)  outport(0x03ce, 0x0100 * x + 0x0004)

#define Clear_Screen() Solid_Box(0, 0, 639, 479, BLACK);

#define _iswap(a, b) \
{                    \
    int i;           \
    i = *a;          \
    *a = *b;         \
    *b = i;          \
}

void Set_Mode( int mode );
void Get_Mode( );

void SCANLINE( int start, int end, int raster, int color );
void OUT_CH( int x, int y, int col, int chr, int fn_seg, int fn_off );
void _LINE( int x1, int y1, int x2, int y2, int col );
void BITBLT( int X_Srs, int Y_Src, int X_Dst, int Y_Dst, int BWidth, int BHeight, int Fn );

void Set_Cursor( int, int, int, int );
void Move_Cursor( int x, int y );
void Remove_Cursor( );

void Rectangle( int x1, int y1, int x2, int y2, int col );
void Line( int x1, int y1, int x2, int y2, int col );
void Solid_Box( int x1, int y1, int x2, int y2, int col );

void Save_Screen( int x1, int y1, int x2, int y2 );
void Restore_Screen( );
void Near_Save_Screen( int x1, int y1, int x2, int y2 );
void Near_Restore_Screen( );

#endif