COMP_FLAGS := -O2 -lpthread -march=native
NPROC      := $(shell nproc)

all: better phase1 phase2
phase2: phase2.o
	gcc -o phase2  phase2.o $(COMP_FLAGS) -L./ -lmatrix -Wl,-rpath=./
phase2.o: src/proj.c libmatrix.so
	gcc -c src/proj.c -o phase2.o $(COMP_FLAGS) -DP2 -I./
better: better.o
	gcc -o better better.o $(COMP_FLAGS) -L./ -lmatrix -Wl,-rpath=./
better.o: src/proj.c libmatrix.so
	gcc -c src/proj.c -o better.o $(COMP_FLAGS) -DNPROC=$(NPROC) -I./
phase1: phase1.o
	gcc -o phase1 phase1.o $(COMP_FLAGS)  -L./ -lmatrix -Wl,-rpath=./
phase1.o: src/proj.c libmatrix.so
	gcc -c src/proj.c -o phase1.o $(COMP_FLAGS) -DP1 -I./
libmatrix.so: libmatrix.o
	gcc -shared -o libmatrix.so libmatrix.o $(COMP_FLAGS)
libmatrix.o: src/matrix.c src/matrix.h
	gcc -Wall -fPIC -c src/matrix.c -o libmatrix.o $(COMP_FLAGS) -I./
clean:
	rm libmatrix.o libmatrix.so phase1.o phase1 better.o better phase2.o phase2
