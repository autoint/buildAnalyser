ifdef ComSpec
	MV = move
	RM = del
	HOST_DEFINE ?= -DWIN32
else
	MV = mv -f
	RM = rm -f
	HOST_DEFINE ?= -DLIN
endif

capture.exe : capture.o
	gcc -o capture.exe capture.o

gcc.exe : capture.o
	gcc -o gcc.exe capture.o

gcc : capture.exe
	$(MV) capture.exe gcc

capture.o : capture.c 
	gcc -c $(HOST_DEFINE) capture.c

clean : 
	$(RM) *.o gcc.exe capture.exe

publish : capture.exe
	$(RM) *.o
