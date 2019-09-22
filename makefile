CC=cc
COMPILEROPTIONS = -c -g

main: src/main.o src/alloc.o src/string.o src/error.o src/input.o src/lex.o src/output.o src/types.o src/sym.o; $(CC) $^ -g -o main


main.o: src/main.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
alloc.o: src/alloc.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
string.o: src/string.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
error.o: src/error.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
input.o: src/input.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
lex.o: src/lex.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
output.o: src/output.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
types.o: src/types.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
sym.o: src/sym.c; $(CC) $(COMPILEROPTIONS) -o $@ $^

clean:
	rm -f src/*.o