/*
===============================================================================
Convert TIS-620 to KU Thai character code.
Copyright 2014, Khral Steelforge.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#include "std2ku.h"

void usage( void ) {
	puts( "Thai TIS-620 to KU converter" );
	puts( "Copyright 2014, Khral Steelforge.\n" );
	puts( "BACKUP BEFORE CONVERT!\n" );
	puts( "usage: STD2KU [input file] [ourput file]");
}

int main( int argc, char *argv[] ) {

	FILE *inFile, *outFile;

	unsigned char character;

	if ( argc != 3 ) {
		usage( );
		exit( 0 );
	}

	if ( ( inFile = fopen( argv[1], "r" ) ) == NULL ) {
		puts( "Can't open input file." );
		exit( 0 );
	}
	if ( ( outFile = fopen( argv[2], "w" ) ) == NULL ) {
		puts( "Can't open output file." );
		exit( 0 );
	}

	do {
		character = fgetc( inFile );
		if ( feof( inFile ) ) {
			break;
		}
		fprintf( outFile, "%c", std2ku( character ) );
	} while ( 1 );

	puts( "Finished!" );

	fclose( inFile );
	fclose( outFile );
	return 0;
}
