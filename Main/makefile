all: Editor

Editor: Editor.cpp
	g++ -o editor.o -O0 Editor.cpp -lncurses

run:
	./editor.o

setupuser:
	g++ -o setupusers.o -O0 setupusers.cpp

setupuser_run:
	./setupusers.o

clean: 
	rm -rf editor.o
	rm -rf setupusers.o