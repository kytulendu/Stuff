/*
* ============================================================================
* Thai character code converter test.
* By Khral Steelforge <https://github.com/kytulendu>.
*
* This is free and unencumbered software released into the public domain.
*
* Anyone is free to copy, modify, publish, use, compile, sell, or
* distribute this software, either in source code form or as a compiled
* binary, for any purpose, commercial or non-commercial, and by any
* means.

* In jurisdictions that recognize copyright laws, the author or authors
* of this software dedicate any and all copyright interest in the
* software to the public domain. We make this dedication for the benefit
* of the public at large and to the detriment of our heirs and
* successors. We intend this dedication to be an overt act of
* relinquishment in perpetuity of all present and future rights to this
* software under copyright law.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* For more information, please refer to <http://unlicense.org/>
*
* ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#include "cwku2std.h"
#include "std2cwku.h"

#include "rwku2std.h"
#include "std2rwku.h"

int main( ) {
	int i;
	for ( i = 0; i < 256; i++ ) {
		if ( cwku2std( std2cwku( i ) ) != i ) {
			printf( "In std2ku, wrong code = %X\n", i );
		}
	}
	for ( i = 0; i < 256; i++ ) {
		if ( std2cwku( cwku2std( i ) ) != i ) {
			printf( "In ku2std, wrong code = %X\n", i );
		}
	}
	for ( i = 0; i < 256; i++ ) {
		if ( rwku2std( std2rwku( i ) ) != i ) {
			printf( "In std2rwku, wrong code = %X\n", i );
		}
	}
	for ( i = 0; i < 256; i++ ) {
		if ( std2rwku( rwku2std( i ) ) != i ) {
			printf( "In rwku2std, wrong code = %X\n", i );
		}
	}
	return 0;
}
