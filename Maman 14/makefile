asm: main.o error.o lists.o files.o parser.o pre_asm.o translate.o
	gcc -o asm main.o error.o lists.o files.o parser.o pre_asm.o translate.o


main.o: main.c parser.h lists.h error.h files.h
	gcc -c -ansi -Wall -pedantic main.c
parser.o: parser.h error.h lists.h files.h
	gcc -c -ansi -Wall -pedantic parser.c
lists.o: lists.h
	gcc -c -ansi -Wall -pedantic lists.c
error.o: error.h
	gcc -c -ansi -Wall -pedantic error.c
files.o: files.h error.h lists.h parser.h translate.h
	gcc -c -ansi -Wall -pedantic files.c
pre_asm.o: pre_asm.h
	gcc -c -ansi -Wall -pedantic pre_asm.c
translate.o: translate.h files.h lists.h error.h parser.h
	gcc -c -ansi -Wall -pedantic translate.c
