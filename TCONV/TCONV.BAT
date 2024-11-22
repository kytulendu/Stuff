@echo off
echo Thai character code converter
echo =============================
echo Note: Convert TIS-620 to Kaset will lose some character.
echo       Invalid character will replace with space (20h).
echo       Option settings at the end of file will be stripped out.
echo.
echo KUCW2STD    Kaset-CW (Kaset CU-Writer) to TIS-620
echo KURW2STD    Kaset-RW (Kaset Rajavithi Word PC) to TIS-620
echo STD2KUCW    TIS-620 to Kaset-CW
echo STD2KURW    TIS-620 to Kaset-RW
echo STW2CW      iRC Standard Word to CU-Writer, TIS-620 only
echo WS2TXT      WordStar to plain text
echo.
echo Usage: [Command] [input file] [output file]
echo.
