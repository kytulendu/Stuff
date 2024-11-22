/*
* ============================================================================
* Print a given binary file to text.
* Use for visualize display font ROM.
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

/* https://stackoverflow.com/questions/7863499/conversion-of-char-to-binary-in-c */
void printbincharpad(char c)
{
    int i;
    //for (i = 0; i <= 7; i++)
    for (i = 7; i >= 0; i--)
    {
        putchar((c & (1 << i)) ? '#' : '.');
    }
}

int main(int argc, char *argv[])
{
    FILE *fptr;
    char read;
    int count = 0;

    if ((argc < 2) || (argc > 2))
    {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    fptr = fopen(argv[1],"rb");
    if (fptr == NULL)
    {
        printf("Error reading file %s", argv[1]);
        return 1;
    }

    //printf("# 0 = .; 1 = #; bit order B0-B7\n");
    printf("# 0 = .; 1 = #; bit order B7-B0\n");

    while (1)
    {
        read = getc(fptr);
        if (feof(fptr))
        {
            break;
        }
        printf("%04x : ", count);
        printbincharpad(read);
        putchar('\n');
        count++;
    }

    fclose(fptr);

    return 0;
}
