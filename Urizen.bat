:: A3 SCRIPT
echo off
echo '--------------------------------------------'
echo '-      ALGONQUIN COLLEGE - COM - 26W       -'
echo '--------------------------------------------'
echo '-                                          -'
echo '-    ====                                  -'
echo '-   =                                      -'
echo '-  =       ==   = =  ==  = =   =   ==  ==  -'
echo '-   =     =  = = = = = = = =   ==  =  ==   -'
echo '-    ====  ==  = = = =   = === === =   ==  -'
echo '-(too lazy to change this intricate ASCII) -'
echo '--------------------------------------------'
echo '-  [A3: Scanner - Team: David Jacob]   
echo '--------------------------------------------'

echo COMPILER LOADING....

del ENCRYPTED.txt DECRYPTED.txt out.txt err.txt 2>nul
gcc Main1Coder.c Main2Reader.c Compilers.c Step1Coder.c Step2Reader.c main3Scanner.c Step3Scanner.c -o Urizen
timeout /t 1 /nobreak >nul 
Urizen 1 1 CLEAR.txt ENCRYPTED.txt > out.txt 2> err.txt
timeout /t 1 /nobreak >nul
Urizen 3 ENCRYPTED.txt  >> out.txt 2>> err.txt
timeout /t 1 /nobreak >nul
dir out.txt err.txt
type out.txt
pause
exit