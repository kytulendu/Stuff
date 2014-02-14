#include <stdlib.h>
#include <mem.h>
#include <dos.h>
#include <alloc.h>

#include "vgalib.h"

typedef struct {
	int x1, y1, x2, y2;
	unsigned char far *img[4];
} Screen;

Screen scr_ed[8];

int img_top = 0;

unsigned char far *scr = MK_FP( 0xA000, 0000 );

void Set_Mode( int mode ) {

	union REGS regs;

	regs.h.ah = 0;                      /* set BIOS function number */
	regs.h.al = mode;                   /* set video mode number */
	int86( 0x10, &regs, &regs );          /* set video mode */
}

void Solid_Box( int x1, int y1, int x2, int y2, int col ) {
	int i;
	if ( x1 > x2 )
		_iswap( &x1, &x2 );
	if ( y1 > y2 )
		_iswap( &y1, &y2 );
	for ( i = y1; i <= y2; i++ ) {
		SCANLINE( x1, x2, i, col );
	}
}

void Line( int x1, int y1, int x2, int y2, int col ) {
	if ( x1 > x2 )
		_iswap( &x1, &x2 );
	if ( y1 > y2 )
		_iswap( &y1, &y2 );
	if ( y1 == y2 )
		SCANLINE( x1, x2, y1, col );
	else
		_LINE( x1, y1, x2, y2, col );
}

void Rectangle( int x1, int y1, int x2, int y2, int col ) {
	Line( x1, y1, x1, y2, col );
	Line( x2, y1, x2, y2, col );
	Line( x1, y1, x2, y1, col );
	Line( x1, y2, x2, y2, col );
}

void Save_Screen( int x1, int y1, int x2, int y2 ) {
	register int x, y;
	int plane;
	if ( img_top >= 7 )
		return;
	if ( y1 > y2 )
		_iswap( &y1, &y2 );
	if ( x1 > x2 )
		_iswap( &x1, &x2 );
	x1 /= 8;
	x2 /= 8;
	if ( scr_ed[img_top].img[0] != NULL )
		return;
	scr_ed[img_top].x1 = x1;
	scr_ed[img_top].x2 = x2;
	scr_ed[img_top].y1 = y1;
	scr_ed[img_top].y2 = y2;
	for ( plane = 0; plane < 4; plane++ ) {
		if ( ( scr_ed[img_top].img[plane] = ( unsigned char far * )farmalloc( ( x2 + 1 - x1 ) * ( y2 + 1 - y1 ) ) ) == NULL ) {
			printf( "Not enough memory" );
			exit( 1 );
		}
		set_readplane( plane );
		for ( y = y1; y <= y2; y++ )
			for ( x = x1; x <= x2; x++ )
				scr_ed[img_top].img[plane][( x - x1 ) + ( y - y1 ) * ( x2 - x1 + 1 )] = scr[x + y * 80];
	}
	img_top += 1;
}

void Restore_Screen( ) {
	register int x, y;
	int x1, y1, x2, y2;
	int plane;

	if ( img_top < 0 )
		return;
	img_top -= 1;
	x1 = scr_ed[img_top].x1;
	x2 = scr_ed[img_top].x2;
	y1 = scr_ed[img_top].y1;
	y2 = scr_ed[img_top].y2;
	for ( plane = 0; plane < 4; plane++ ) {
		switch ( plane ) {
		case 0: set_writecolor( 1 );
			break;
		case 1: set_writecolor( 2 );
			break;
		case 2: set_writecolor( 4 );
			break;
		case 3: set_writecolor( 8 );
			break;
		}
		for ( y = y1; y <= y2; y++ )
			for ( x = x1; x <= x2; x++ )
				scr[x + y * 80] = scr_ed[img_top].img[plane][( x - x1 ) + ( y - y1 ) * ( x2 - x1 + 1 )];
		farfree( scr_ed[img_top].img[plane] );
		scr_ed[img_top].img[plane] = NULL;
	}
	set_writecolor( 15 );
}

void Near_Save_Screen( int x1, int y1, int x2, int y2 ) {
	register int x, y;
	int plane;
	if ( img_top >= 7 )
		return;
	if ( y1 > y2 )
		_iswap( &y1, &y2 );
	if ( x1 > x2 )
		_iswap( &x1, &x2 );
	x1 /= 8;
	x2 /= 8;
	if ( scr_ed[img_top].img[0] != NULL )
		return;
	scr_ed[img_top].x1 = x1;
	scr_ed[img_top].x2 = x2;
	scr_ed[img_top].y1 = y1;
	scr_ed[img_top].y2 = y2;
	for ( plane = 0; plane < 4; plane++ ) {
		if ( ( scr_ed[img_top].img[plane] = ( unsigned char * ) malloc( ( x2 + 1 - x1 ) * ( y2 + 1 - y1 ) ) ) == NULL ) {
			printf( "Not enough memory" );
			exit( 1 );
		}
		set_readplane( plane );
		for ( y = y1; y <= y2; y++ )
			for ( x = x1; x <= x2; x++ )
				scr_ed[img_top].img[plane][( x - x1 ) + ( y - y1 ) * ( x2 - x1 + 1 )] = scr[x + y * 80];
	}
	img_top += 1;
}

void Near_Restore_Screen( ) {
	register int x, y;
	int x1, y1, x2, y2;
	int plane;

	if ( img_top < 0 )
		return;
	img_top -= 1;
	x1 = scr_ed[img_top].x1;
	x2 = scr_ed[img_top].x2;
	y1 = scr_ed[img_top].y1;
	y2 = scr_ed[img_top].y2;
	for ( plane = 0; plane < 4; plane++ ) {
		switch ( plane ) {
		case 0: set_writecolor( 1 );
			break;
		case 1: set_writecolor( 2 );
			break;
		case 2: set_writecolor( 4 );
			break;
		case 3: set_writecolor( 8 );
			break;
		}
		for ( y = y1; y <= y2; y++ )
			for ( x = x1; x <= x2; x++ )
				scr[x + y * 80] = scr_ed[img_top].img[plane][( x - x1 ) + ( y - y1 ) * ( x2 - x1 + 1 )];
		free( ( unsigned char * ) scr_ed[img_top].img[plane] );
		scr_ed[img_top].img[plane] = NULL;
	}
	set_writecolor( 15 );
}
