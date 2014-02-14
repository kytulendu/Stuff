#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include <dos.h>

#include "vgalib.h"
#include "thaivga.h"

void T_ShowLogo( int x, int y, char *path ) {
	FILE *logo;
	int plan, i;
	unsigned char *ll;
	unsigned char far *scr;

	if ( ( ll = malloc( 32 ) ) == NULL ) {
		return;
	}
	x = x / 8;
	logo = fopen( path, "rb" );
	if ( logo != NULL ) {
		for ( plan = 0; plan < 2; plan++ ) {
			switch ( plan ) {
			case 0: set_writecolor( 7 );
				break;
			case 1: set_writecolor( 8 );
				break;
			}

			for ( i = 0; i < 256; i++ ) {
				scr = MK_FP( 0xA000, ( i + y ) * 80 + x );
				fread( ll, 1, 32, logo );
				_fmemcpy( scr, ll, 32 );
			}

		}

		fclose( logo );
	}

	free( ll );
}
