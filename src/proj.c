#include "matrix.c"
#include <time.h>
int _debvar = 0;
#define deb {puts("----------------"); printf("%s %d \n", __FILE__, __LINE__); puts("----------------");}

matrix *some_opration(matrix *m1, matrix m2);
int printer(matrix *status, uint64_t i, uint64_t j,  void *field);
int intmatrix_printer(matrix *m1);
int matrand(matrix *status, uint64_t i, uint64_t j, void *field);
int zerout(matrix * m, uint64_t i, uint64_t j, void *f){
  int c = 0x12345678;
  *(int*)f = c;
}

int main(){
  matrix *m1;  
  srand(time(NULL));
  m1 = create_matrix(3, 3, sizeof(int));
  printf("%p\n", m1->mat);  
  matrix_map(m1, zerout);
  matrix_map(m1, matrand);
  intmatrix_printer(m1);
  return 0;
}

int printer(matrix *status, uint64_t i, uint64_t j,  void *field){
  int ret = printf("%d  ", *(int*)field);
  if(j == status->column_c - 1) puts("");
  return ret;
}

int intmatrix_printer(matrix* m1){
  int status = 0;
  status = matrix_map(m1, printer);
  return status;
}

int matrand(matrix* mat, uint64_t i, uint64_t j, void* field){
  void* tmp = MATREF(mat, i, j);
  int rnd = rand() % 100;
  *(int*) field = rnd;
  return  0;
}

