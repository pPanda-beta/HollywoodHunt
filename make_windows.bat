prompt $$
cd obj
g++ -I ..\src\include -std=c++11  -m32 -w -fpermissive -c  ..\src\*.cpp
gcc -I ..\src\include -std=c11  -m32 -w  -c  ..\src\*.c
cd ..
g++ obj\*.o src\libs\win32\*.a -o win32\Hollywood_Hunt.exe -mwindows
