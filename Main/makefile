all: Editor

Editor: Editor.cpp
	g++ -o editor.o -O0 Editor.cpp -lncurses

run:
	./editor.o

setupuser:
	g++ -o setupusers.o -O0 setupusers.cpp

setupuser_run:
	./setupusers.o

refreshdata:
	g++ -o refreshdata.o -O0 refreshdata.cpp

resetdata_run:
	./refreshdata.o 0

refreshdata_run:
	./refreshdata.o 1

clean: 
	rm -rf editor.o
	rm -rf setupusers.o
	rm -rf refreshdata.o