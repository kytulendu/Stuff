/*
* ============================================================================
* Convert TIS-620 to Kaset-RW (Kaset Rajavithi Word PC) Thai character code.
* By Khralkatorrix <https://github.com/kytulendu>.
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

#include "std2kurw.h"

void usage(void)
{
    puts("Convert TIS-620 to Kaset-RW (Kaset Rajavithi Word PC) Thai character code.");
    puts("By Khralkatorrix.\n\n" );
    puts("Usage: STD2KURW [input file] [output file]");
}

int main(int argc, char *argv[])
{
    FILE *inFile, *outFile;
    unsigned char character;

    if (argc != 3)
    {
        usage();
        exit(0);
    }

    if ((inFile = fopen(argv[1], "r")) == NULL)
    {
        puts("Can't open input file.");
        exit(0);
    }
    if ((outFile = fopen(argv[2], "w")) == NULL)
    {
        puts("Can't open output file.");
        exit(0);
    }

    do {
        character = fgetc(inFile);
        if (feof(inFile))
        {
            break;
        }

        /* Convert CU-Writer control code to Rajavithi Word PC */
        if (character == 0x12)              /* double-underline */
            character = 0x13;
        else if (character == 0x17)         /* italic */
            character = 0x15;
        else if ((character >= 0x1b) && (character <= 0x1e)) /* multi font control code */
            continue;

        fprintf(outFile, "%c", std2kurw(character));
    } while (1);

    puts("Finished!");

    fclose(inFile);
    fclose(outFile);
    return 0;
}
