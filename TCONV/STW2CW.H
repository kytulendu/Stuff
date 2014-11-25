/*
* ============================================================================
* Convert iRC Standard Word table to CU-Writer table, TIS-620 only.
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

/** Convert iRC Standard Word table to CU-Writer table, TIS-620 only.
*  \param[in]  p_stdcode		STDW table code.
*  \return unsigned char		CW table code. */
unsigned char stw2cwtab( unsigned char p_stdcode ) {
	switch ( p_stdcode ) {
	case 0x087:
		return ( 0x098 );
		break;
	case 0x088:
		return ( 0x09b );
		break;
	case 0x08b:
		return ( 0x099 );
		break;
	case 0x08c:
		return ( 0x09a );
		break;
	case 0x08e:
		return ( 0x096 );
		break;
	case 0x08f:
		return ( 0x095 );
		break;
	default:
		return ( p_stdcode );
	}
}
