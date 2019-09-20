#ARCH1718.stefano.notari.0000832003
assembler: main.o  hackinstruction.o symboltable.o constants.o
	gcc -o assembler main.o  hackinstruction.o symboltable.o constants.o

main.o: main.o hackinstruction.o symboltable.o constants.o
		gcc -c main.c

hackinstruction.o: hackinstruction.c hackinstruction.h constants.o
	gcc -c hackinstruction.c

symboltable.o: symboltable.c symboltable.h constants.o
	gcc -c symboltable.c

constants.o: constants.c constants.h
	gcc -c constants.c

clean:
	rm *.o assembler