#include "matrix.h"
#include <time.h>
#define RLIMIT 3
#define FI 3
#define FJ 3
#define SI 2
#define SJ 2


matrix *some_opration(matrix *m1, matrix *m2);
int printer(matrix *status, uint64_t i, uint64_t j,  void *field, void* args);
int intmatrix_printer(matrix *m1);
int matrand(matrix *status, uint64_t i, uint64_t j, void* field, void *args);
/* matrix* mult_overlap(matrix *m1, matrix *m2){ */

/*   return m1; */
/* } */

int main(){
  matrix *m1, *m2;
  srand(time(NULL));
  m1 = create_matrix(FI, FJ, sizeof(int));
  matrix_map(m1,NULL, matrand);
  intmatrix_printer(m1);
  /* printf("%p\n", m1->mat); */
  /* for(int i = 0; i < m1->row_c; i++){ */
  /*   for(int j = 0; j < m1->column_c; j++){ */
  /*     //printf("%p ", MATREF_P(m1, i, j)); */
  /*     printf("%p: %d ",  MATREF_P(m1, i, j),  *(int*)MATREF_P(m1, i, j)); */
  /*   } */
  /*   puts(""); */
  /* } */
  /* return 0;   */
  deb; 
  m2 = create_matrix(SI, SJ, sizeof(int));
  matrix_map(m2, NULL, matrand);
  intmatrix_printer(m2);
  some_opration(m1, m2);
  deb;
  intmatrix_printer(m1);
  return 0;
}

int printer(matrix *status, uint64_t i, uint64_t j,  void *field, void* args){
  int ret = printf("%2d ", *(int*)field);
  if(j == status->column_c - 1) puts("");
  return ret;
}
int intmatrix_printer(matrix* m1){
  int status = 0;
  status = matrix_map(m1, NULL,printer);
  return status;
}

int matrand(matrix *mat, uint64_t i, uint64_t j, void *field, void *args) {
  void* tmp = MATREF_P(mat, i, j);
  int rnd = rand() % RLIMIT;
  *(int*) field = rnd;
  return  0;
}

int inner_matorator(matrix* m, uint64_t i, uint64_t j, void* field, void* args){
  *(int*) args += (* (int*) field) * (* (int*) MATREF_P(m, i, j));
  return 1;
}

int matorator(matrix* m, uint64_t i, uint64_t j, void *field, void *args){
  matrix *m2 = (matrix *)args;
  uint64_t res = 0;
  //int* i, int* j, m2*, int* res
  //void* state [4];
  //matrix_map(m2, &state, inner_matorator);
  for (int x = 0; x < m2->row_c; x++) {
    if(x + i >= m->row_c) continue;
    for (int y = 0; y < m2->row_c; y++) {
      if(y + j >= m->column_c) continue;//{puts("JUMPED"); continue;}
      res += (*(int *)(MATREF_P(m, i+x, j+y))) * (*(int *)(MATREF_P(m2, x, y)));
      //printf("did: %"PRIu64 " = m1: %"PRIu64 " + %"PRIu64 " * m2: %"PRIu64 " + %"PRIu64"\n" , res, i+x, j+y, i, j);
    }
  }
    *(int *) field = res;  
  return 1;  
}
matrix *some_opration(matrix *m1, matrix *m2) {
  matrix_map(m1, (void *)m2, matorator);
  return m1;
}
