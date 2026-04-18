@echo off
echo --------------------------------------------
echo -      ALGONQUIN COLLEGE - COM - 26W       -
echo -  [A5: Writer - Team: David Jacob]        -
echo --------------------------------------------
echo Program reads floats, assigns variables, and prints them via puts.
echo --------------------------------------------
echo COMPILER LOADING....

del ENCRYPTED.txt HELLO_ENC.txt MIXED_ENC.txt DECRYPTED.txt out.txt err.txt 2>nul

gcc Main1Coder.c Main2Reader.c Main3Scanner.c Main4Parser.c Main5Writer.c ^
    Step1Coder.c Step2Reader.c Step3Scanner.c Step4Parser.c Step5Writer.c ^
    Compilers.c -o Urizen.exe
if errorlevel 1 ( echo Build failed. & pause & exit /b 1 )

echo.
echo ============================================
echo  TEST 1 of 3 : Hello World
echo ============================================
Urizen 1 1 HelloWorld.txt HELLO_ENC.txt
Urizen 5 HELLO_ENC.txt

echo.
echo ============================================
echo  TEST 2 of 3 : Mixed Types (int/string/bool/char)
echo ============================================
Urizen 1 1 MixedTypes.txt MIXED_ENC.txt
Urizen 5 MIXED_ENC.txt

echo.
echo ============================================
echo  TEST 3 of 3 : Float Arithmetic (sphere volume)
echo ============================================
Urizen 1 1 CLEAR.txt ENCRYPTED.txt
Urizen 5 ENCRYPTED.txt

echo.
echo --------------------------------------------
echo  All tests complete.
echo --------------------------------------------
pause