/*
===============================================================================
Thai character code converter test.
Copyright 2014, Khral Steelforge.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#include "ku2std.h"
#include "std2ku.h"

int main( ) {
	int i;
	for ( i = 0; i < 256; i++ ) {
		if ( ku2std( std2ku( i ) ) != i ) {
			printf( "In std->ku  code wrong = %X\n", i );
		}
	}
	for ( i = 0; i < 256; i++ ) {
		if ( std2ku( ku2std( i ) ) != i ) {
			printf( "In ku->std  code wrong = %X\n", i );
		}
	}
	return 0;
}
