COMP_FLAGS := "-O2"

run: phase1
	./phase1
phase1: phase1.o
	gcc -o phase1 -L./ -lmatrix -Wl,-rpath=./ phase1.o
phase1.o: src/proj.c libmatrix.so
	gcc -c src/proj.c -o phase1.o
libmatrix.so: libmatrix.o
	gcc -shared -o libmatrix.so libmatrix.o	
libmatrix.o: src/matrix.c src/matrix.h
	gcc -Wall -fPIC -c src/matrix.c -o libmatrix.o
clean:
	rm libmatrix.o libmatrix.so phase1.o phase1 
