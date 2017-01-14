capture.exe : capture.o
	gcc -o capture.exe capture.o

gcc.exe : capture.o
	gcc -o gcc.exe capture.o

capture.o : capture.c 
	gcc -c capture.c

clean : 
	del *.o *.exe

publish : capture.exe
	del *.o
