#include "matrix.h"
#include <time.h>

matrix *some_opration(matrix *m1, matrix *m2);
int printer(matrix *status, uint64_t i, uint64_t j,  void *field, void* args);
int intmatrix_printer(matrix *m1);
int matrand(matrix *status, uint64_t i, uint64_t j, void* field, void *args);
int zerout(matrix * m, uint64_t i, uint64_t j, void *f){
  int c = 0x000000;
  *(int*)f = c;
  return 1;
}

int main(){
  matrix *m1, *m2;  
  srand(time(NULL));
  m1 = create_matrix(20, 20, sizeof(int));
  matrix_map(m1,NULL, matrand);
  intmatrix_printer(m1);
  deb; 
  m2 = create_matrix(3, 3, sizeof(int));
  matrix_map(m2, NULL, matrand);
  intmatrix_printer(m2);
  return 0;
}

int printer(matrix *status, uint64_t i, uint64_t j,  void *field, void* args){
  int ret = printf("%3d  ", *(int*)field);
  if(j == status->column_c - 1) puts("");
  return ret;
}

int intmatrix_printer(matrix* m1){
  int status = 0;
  status = matrix_map(m1, NULL,printer);
  return status;
}

int matrand(matrix *mat, uint64_t i, uint64_t j, void *field, void *args) {
  void* tmp = MATREF(mat, i, j);
  int rnd = rand() % 1000;
  *(int*) field = rnd;
  return  0;
}
int matorator(matrix* m, uint64_t i, uint64_t j, void *field, void *args){
  matrix* m2 = (matrix*) args;
  
  return 0;
}
matrix *some_opration(matrix *m1, matrix *m2){
  matrix_map(m1, (void*)m2, matorator);
  return m1;
}
