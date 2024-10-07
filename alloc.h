// Copyright Necula Mihail 313CAa 2023-2024

#ifndef alloc
#define alloc

int *malloc_vector(int n);
int **malloc_matrix(int n, int m);
int ***malloc_empty_tridim(int cnt);
int ***malloc_tridim(int cnt, int **a);

void free_matrix(int ***a, int n);
void free_tridim(int ****w, int cnt, int **a);

void add_columns(int **a, int n, int m_old, int m);
void add_empty_matrix(int ****w, int cnt);
void add_matrix(int ****w, int cnt, int n, int m);
void remove_matrix(int ****w, int *cnt, int ***a);
void resize_matrix(int ***a, int *n, int *m);

#endif

