/*
* ============================================================================
* Convert iRC Standard Word table to CU-Writer table, TIS-620 only.
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

/** Convert iRC Standard Word table to CU-Writer table, TIS-620 only.
*  \param[in]  p_stwcode        STDW table code.
*  \return unsigned char        CW table code. */
unsigned char stw2cwtab(unsigned char p_stwcode)
{
    switch (p_stwcode)
    {
        case 0x087:
            return 0x098;
            break;
        case 0x088:
            return 0x09b;
            break;
        case 0x089:
            return 0x08f;
            break;
        case 0x08b:
            return 0x099;
            break;
        case 0x08c:
            return 0x09a;
            break;
        case 0x08e:
            return 0x096;
            break;
        case 0x08f:
            return 0x095;
            break;
        case 0x09c:
            return 0x090;
            break;
        case 0x09d:
            return 0x091;
            break;
        case 0x09e:
            return 0x092;
            break;
        case 0x09f:
            return 0x093;
            break;
        default:
            return p_stwcode;
    }
}

/** Convert iRC Standard Word to CU-Writer, TIS-620 only.
*  \param[in]  p_stwcode        STDW character code.
*  \return unsigned char        CW character code. */
unsigned char stw2cw(unsigned char p_stwcode)
{
    switch (p_stwcode)
    {
        case 0x004:             /* enlarge */
            return 0x005;
            break;
        case 0x00C:             /* italic */
            return 0x017;
            break;
        default:
            return stw2cwtab(p_stwcode);
    }
}
