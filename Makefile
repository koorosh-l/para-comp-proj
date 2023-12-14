COMP_FLAGS := -g -lpthread -march=native
NPROC      := $(shell nproc)

all: phase2 phase1 ugly
ugly: ugly.o
	gcc -o ugly -L./ -lmatrix -Wl,-rpath=./ ugly.o $(COMP_FLAGS)
ugly.o: src/proj.c libmatrix.so
	gcc -c src/proj.c -o ugly.o $(COMP_FLAGS) -DUGLY
phase2: phase2.o
	gcc -o phase2 -L./ -lmatrix -Wl,-rpath=./ phase2.o $(COMP_FLAGS)
phase2.o: src/proj.c libmatrix.so
	gcc -c src/proj.c -o phase2.o $(COMP_FLAGS) -DNPROC=$(NPROC)
phase1: phase1.o
	gcc -o phase1 -L./ -lmatrix -Wl,-rpath=./ phase1.o $(COMP_FLAGS)
phase1.o: src/proj.c libmatrix.so
	gcc -c src/proj.c -o phase1.o $(COMP_FLAGS) -DP1
libmatrix.so: libmatrix.o
	gcc -shared -o libmatrix.so libmatrix.o $(COMP_FLAGS)
libmatrix.o: src/matrix.c src/matrix.h
	gcc -Wall -fPIC -c src/matrix.c -o libmatrix.o $(COMP_FLAGS)
clean:
	rm libmatrix.o libmatrix.so phase1.o phase1 phase2.o phase2 ugly.o ugly
