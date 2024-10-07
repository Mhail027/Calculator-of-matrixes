// Copyright Necula Mihail 313CAa 2023-2024

#define MAX 10007

/*  Citeste, de la tastatura, elemnetele de pe  coloana j ale matricei a,
care  are n linii. */
void read_column(int **a, int n, int j)
{
	for (int i = 0; i < n; ++i)
		scanf("%d", &a[i][j]);
}

/* Citeste, de la tastatura, elementele matricei a, ce are
n linii si m coloane */
void read_matrix(int **a, int n, int m)
{
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			scanf("%d", &a[i][j]);
}

// Afiseaza, pe ecran, matricea a, ce are n linii si m coloane.
void print_matrix(int **a, int n, int m)
{
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j)
			printf("%d ", a[i][j]);
		printf("\n");
		}
}

/* Determina si returneaza matricea transpusa lui a,
ce are n linii si m coloane. */
int **transpose(int **a, int n, int m)
{
	int **c = malloc_matrix(m, n);
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			c[i][j] = a[j][i];
	return c;
}

/* Imparte matricea a, ce are n linii si m coloane, in 4 blocuri
de dimensiuni egale si returneaza un vector in care se afla adresele
la blocuirle respective. */
int ***split_in_four(int **a, int n, int m)
{
	/* Vom salva dimensiunile celor 4 blocuri in matricea p, asfel:
		p[0][i] = numarul de linii ale blocului i;
		p[1][i] = numarul de coloane ale blocului i. */
	int **p;
	p = malloc_matrix(2, 4);
	for (int i = 0; i < 4; ++i) {
		p[0][i] = n / 2;
		p[1][i] = m / 2;
	}

	// Cele patru blocuri vor fi salvate in tabloul tridimensional w.
	int ***w = malloc_tridim(4, p);
	for (int i = 0; i < n / 2; ++i) {
		for (int j = 0; j < m / 2; ++j) {
			// w[0] -> blocul din stanga-sus
			w[0][i][j] = a[i][j];
			// w[1] -> blocul din dreapta-sus
			w[1][i][j] = a[i][m / 2 + j];
			// w[2] -> blocul din stanga-jos
			w[2][i][j] = a[n / 2 + i][j];
			// w[3] -> blocul din dreapta-jos
			w[3][i][j] = a[n / 2 + i][m / 2 + j];
		}
	}

	free_matrix(&p, 2);
	return w;
}

/* Imbina intr-o singura matrice cele 4 blocuri, de dimesniune n x m,
care se regasesc in vectorul w */
int **merge_four_in_one(int ***w, int n, int m)
{
	int **a = malloc_matrix(n * 2, m * 2);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			// matricea w[0] merge in stanga-sus
			a[i][j] = w[0][i][j];
			// matricea w[1] merge in dreapta-sus
			a[i][m + j] = w[1][i][j];
			// matricea w[2] merge in stanga-jos
			a[n + i][j] = w[2][i][j];
			// matricea w[3] merge in dreapta-jos
			a[n + i][m + j] = w[3][i][j];
		}
	}
	return a;
}

/* Calculeaza si returneaza suma matricilor a si b,
care au dimensiunea n x m. */
int **sum(int **a, int **b, int n, int m)
{
	int **c = malloc_matrix(n, m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			c[i][j] = (((a[i][j] + b[i][j]) % MAX) + MAX) % MAX;
	return c;
}

/* Calculeaza si returneaza diferenta matricilor a si b,
care au dimensiunea n x m. */
int **difference(int **a, int **b, int n, int m)
{
	int **c = malloc_matrix(n, m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			c[i][j] = ((a[i][j] - b[i][j]) % MAX + MAX) % MAX;
	return c;
}

/* Calculeaza si returneaza produsul matricilor a si b,
care au dimensiunea n1 x m1, respectiv m1 x m2. */
int **product(int **a, int **b, int n1, int m1, int m2)
{
	int **c = malloc_matrix(n1, m2);
	for (int i = 0; i < n1; ++i) {
		for (int j = 0; j < m2; ++j) {
			c[i][j] = 0;
			for (int k = 0; k < m1; ++k) {
				int new_term = ((a[i][k] * b[k][j]) % MAX + MAX) % MAX;
				c[i][j] = ((c[i][j] + new_term) % MAX + MAX) % MAX;
			}
		}
	}
	return c;
}

/*  Folosind algoritmul lui Strassen, calculeaza si returneaza produsul
matricilor a si b, care au dimesniunea n x n. (n este o putere a lui 2) */
int **strassen(int **a, int **b, int n)
{
	// Functia este implementata recursiv.

	/* n == 1 reprezinta conditia de oprire a autoapelarii functiei
	In momentul in care matricile a si b au un singur element, acestea
	nu mai pot fi impartite in blocuri. */
	if (n == 1) {
		int **D = (int **)malloc_matrix(1, 1);
		D[0][0] = a[0][0] * b[0][0];
		return D;
	}

	int m = n / 2;
	int ***tmp = malloc_empty_tridim(14);
	int ***A = split_in_four(a, n, n);
	int ***B = split_in_four(b, n, n);
	int ***M = malloc_empty_tridim(7);

	tmp[0] = sum(A[0], A[3], m, m);
	tmp[1] = sum(B[0], B[3], m, m);
	M[0] = strassen(tmp[0], tmp[1], m);

	tmp[2] = sum(A[2], A[3], m, m);
	M[1] = strassen(tmp[2], B[0], m);

	tmp[3] = difference(B[1], B[3], m, m);
	M[2] = strassen(A[0], tmp[3], m);

	tmp[4] =  difference(B[2], B[0], m, m);
	M[3] = strassen(A[3], tmp[4], m);

	tmp[5] = sum(A[0], A[1], m, m);
	M[4] = strassen(tmp[5], B[3], m);

	tmp[6] = difference(A[2], A[0], m, m);
	tmp[7] = sum(B[0], B[1], m, m);
	M[5] = strassen(tmp[6], tmp[7], m);

	tmp[8] = difference(A[1], A[3], m, m);
	tmp[9] = sum(B[2], B[3], m, m);
	M[6] = strassen(tmp[8], tmp[9], m);

	int ***C = malloc_empty_tridim(4);

	tmp[10] = sum(M[0], M[3], m, m);
	tmp[11] = difference(M[4], M[6], m, m);
	C[0] = difference(tmp[10], tmp[11], m, m);

	C[1] = sum(M[2], M[4], m, m);

	C[2] = sum(M[1], M[3], m, m);

	tmp[12] = difference(M[0], M[1], m, m);
	tmp[13] = sum(M[2], M[5], m, m);
	C[3] = sum(tmp[12], tmp[13], m, m);

	// Unim matricile C[0], C[1], C[2] si C[3].
	int **D = merge_four_in_one(C, m, m);

	/* Salvam intr-un tablou bidimensional dimensiunile matricilor
	din tablourile A, B si tmp. Avem nevoie sa facem asta pentru
	a putea apela functia free_tridim pentru A, B si tmp. */
	int **p;
	p = malloc_matrix(2, 14);
	for (int j = 0; j < 14; ++j) {
		p[0][j] = m;
		p[1][j] = m;
	}
	free_tridim(&tmp, 14, p);
	free_tridim(&A, 4, p);
	free_tridim(&B, 4, p);
	free_tridim(&M, 7, p);
	free_tridim(&C, 4, p);
	free_matrix(&p, 2);
	return D;
}

/* Calculeaza si returneaza matricea a, ce are dimensiuna n x n,
ridicata la puterea p. */
int **power(int **a, int n, int p)
{
	// Functia este implementata recursiv.

    // Daca p este egal cu 0, rezultatul e matricea identica.
	if (p == 0) {
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				if (i == j)
					a[i][j] = 1;
				else
					a[i][j] = 0;
		return a;
	}
	// Daca p est eegal cu 1, rezultaul e matricea insasi.
	if (p == 1)
		return a;

	/* In caz contrar, ne folosim de formulele:
		A^p = A^(p / 2) * A^(p / 2), daca p e par;
		A^p = A^((p -1) / 2) * A^((p - 1) / 2) * A, daca p e impar. */
	int **tmp = power(a, n, p / 2);
	int **b = product(tmp, tmp, n, n, n);
	int **c = b;
	if (p % 2 == 1) {
		c = product(b, a, n, n, n);
		for (int i = 0; i < n; ++i)
			free(b[i]);
		free(b);
	}

	/* Daca p <= 3, atunci &tmp va pointa la matricea a,
	de care este posibil sa mai avem nevoie. */
	if (p > 3)
		free_matrix(&tmp, n);
	return c;
}

