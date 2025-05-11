CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lfl

# Object files
OBJ = scanner.o parser.tab.o ast.o utils.o main.o

# Target
out: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

scanner.o: scanner.l parser.tab.h
	flex -o $@ scanner.l

scanner.o: scanner.c  # Add rule to compile scanner.c to scanner.o
	gcc -c scanner.c -o scanner.o

scanner.c: scanner.l  # Add rule to generate scanner.c from scanner.l
	flex -o scanner.c scanner.l

parser.tab.o: parser.tab.c parser.tab.h
	$(CC) -c parser.tab.c $(CFLAGS)

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

ast.o: ast.c ast.h
	$(CC) -c ast.c $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -c utils.c $(CFLAGS)

main.o: main.c ast.h utils.h
	$(CC) -c main.c $(CFLAGS)

clean:
	rm -f $(OBJ) out deafult.csv parser.tab.c parser.tab.h scanner.c


