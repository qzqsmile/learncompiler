CC=cc
COMPILEROPTIONS = -c -g

main: main.o alloc.o string.o error.o input.o lex.o output.o types.o sym.o tree.o expr.o enode.o
	$(CC) $^ -g -o main
	make clean


main.o: main.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
alloc.o: alloc.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
string.o: string.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
error.o: error.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
input.o: input.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
lex.o: lex.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
output.o: output.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
types.o: types.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
sym.o: sym.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
tree.o: tree.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
expr.o: expr.c; $(CC) $(COMPILEROPTIONS) -o $@ $^
enode.o: enode.c; $(CC) $(COMPILEROPTIONS) -o $@ $^

clean:
	rm -f *.o