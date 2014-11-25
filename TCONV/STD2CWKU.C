/*
* ============================================================================
* Convert TIS-620 to CU-Writer KU Thai character code.
* Copyright (c) 2014, Khral Steelforge.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom
* the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
* ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#include "std2cwku.h"

void usage( void ) {
	puts( "Convert TIS-620 to CU-Writer KU Thai character code." );
	puts( "Copyright 2014, Khral Steelforge.\n" );
	puts( "BACKUP BEFORE CONVERT!\n" );
	puts( "usage: STD2KU [input file] [output file]");
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
		fprintf( outFile, "%c", std2cwku( character ) );
	} while ( 1 );

	puts( "Finished!" );

	fclose( inFile );
	fclose( outFile );
	return 0;
}
