#include "matrix.h"

struct interval {
  matrix *m;  
  uint64_t start;
  uint64_t end;
  void *pass;
  void (*func) (matrix*,uint64_t,uint64_t,void*);
};
typedef struct interval interval;

matrix* create_matrix(uint64_t row_c, uint64_t column_c, size_t memb){
  matrix* m;
  m = malloc(sizeof(matrix));
  if(!m) {
    fprintf(stderr,"first malloc failed %p", m);
    exit(-1);
  }
  m->row_c    = row_c;
  m->column_c = column_c;
  m->memb     = memb;
  m->mat      = malloc(MATBYTES(m));
  if(!m->mat){
    
  }
  return m;
}
void* matref(matrix* m, int i, int j){
  return m->mat + (i * m->column_c * m->memb) + (j * m->memb);
}
void matrix_map(matrix *m, void *args,
		void (*func)(matrix *, uint64_t, uint64_t, void *))
{
  for(uint64_t i = 0; i < m->row_c * m->column_c; i++){
    func(m, i / (m->column_c), i % (m->column_c), args);
  }
}

void *partial_map(interval *args) {
  uint64_t i = ((interval *)args)->start;
  if (args->start == args->end) {
    args->func(args->m, i / args->m->column_c, i % args->m->column_c,
               args->pass);
    return NULL;
  }
  
  for(; i < ((interval*)args)->end; i++){
    args->func(args->m, i/args->m->column_c, i%args->m->column_c, args->pass);
  }
  pthread_exit(NULL);
  return NULL;
}

void para_matrix_map(uint32_t nproc, matrix *m, void *arg,
		     void (*func)(matrix *, uint64_t, uint64_t, void *))
{
  if(nproc > MATSIZE(m)) nproc = MATSIZE(m);
  printf("nproc: %"PRIu32"\n", nproc);
  uint64_t step = (MATSIZE(m))/nproc;
  pthread_t threads[nproc];
  interval inter[nproc];
  for (uint32_t i = 0; i < nproc - 1; i++) {
    inter[i].m     = m;
    inter[i].start = i * step;
    inter[i].end   = (inter[i].start+step)-1;
    inter[i].pass  = arg;
    inter[i].func  = func;
    fprintf(stderr, "partial_map %" PRIu64 " %" PRIu64 "\n", inter[i].start,
            inter[i].end);
    pthread_create(&threads[i], NULL, ((void *(*)(void *))partial_map),
                   &inter[i]);
  }
  inter[nproc - 1].m     = m;
  inter[nproc - 1].start = (nproc - 1) * step;
  inter[nproc - 1].end   = MATSIZE(m)-1;
  inter[nproc - 1].pass  = arg;
  inter[nproc - 1].func = func;
  fprintf(stderr, "partial_map %" PRIu64 " %" PRIu64 "\n",
          inter[nproc - 1].start, inter[nproc - 1].end);
  pthread_create(&threads[nproc - 1], NULL, ((void *(*)(void *))partial_map),
                 &inter[nproc - 1]);
    
  for(uint8_t i = 0; i < nproc; i++){
    pthread_join(threads[i], NULL);
  }
  return ;
}
matrix* matrix_copy(matrix * m){
  matrix* res = create_matrix(m->row_c, m->column_c, m->memb);
  memcpy(res->mat, m->mat, MATBYTES(m));
  return res;
}

void free_matrix(matrix* m){
  free(m->mat);
  free(m);
  return;
}
