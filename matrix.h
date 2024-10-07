// Copyright Necula Mihail 313CAa 2023-2024

#ifndef matrix
#define matrix

void read_colloum(int **a, int n, int j);
void read_matrix(int **a, int n, int m);
void print_matrix(int **a, int n, int m);

int **transpose(int **a, int n, int m);
int ***split_in_four(int **a, int n, int m);
int **merge_four_in_one(int ***w, int n, int m);

int **sum(int **a, int **b, int n, int m);
int **difference(int **a, int **b, int n, int m);
int **product(int **a, int **b, int n1, int m1, int m2);
int **strassen(int **a, int **b, int n);
int **power(int **a, int n, int p);

#endif

