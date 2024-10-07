// Copyright Necula Mihail 313CAa 2023-2024

#define MAX 10007

// Interschimba intre ele valorile la care pointeaza a si b.
void swp(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

/* Sorteaza crescator vectorul de matrici la care pointeaza w,
in functie de suma elementelor fiecarei matrici, si actualizeaza
matricea (*a) in care se salveaza dimensiunile matriclor din (*w), astfel:
	(*a)[0][i] = numarul de linii ale matricei cu numarul i,
	(*a)[1][i] = numarul de coloane ale matricei cu numarul i.
cnt - numarul de matrici ale tabloului tridimensional */
void sort(int ****w, int cnt, int ***a)
{
	int *sum = malloc_vector(cnt);
	// sum[i] == suma elementelor meatricei cu numarul i
	for (int k = 0; k < cnt; ++k) {
		sum[k] = 0;
		for (int i = 0; i < (*a)[0][k]; ++i)
			for (int j = 0; j < (*a)[1][k]; ++j)
				sum[k] = ((sum[k] + (*w)[k][i][j]) % MAX + MAX) % MAX;
	}

	// Folosim sortarea prin selectie.
	for (int i = 0; i < cnt - 1; ++i)
		for (int j = i + 1; j < cnt; ++j)
			if (sum[i] > sum[j]) {
				swp(&sum[i], &sum[j]);
				swp(&(*a)[0][i], &(*a)[0][j]);
				swp(&(*a)[1][i], &(*a)[1][j]);
				int **tmp = (*w)[i];
				(*w)[i] = (*w)[j];
				(*w)[j] = tmp;
	}

	free(sum);
}

void i_want_matrix_dimensions(int cnt, int **a)
{
	int index;
	scanf("%d", &index);
	if (index < 0 || index >= cnt) {
		printf("No matrix with the given index\n");
		return;
	}

	printf("%d %d\n", a[0][index], a[1][index]);
}

void i_want_to_print_matrix(int ***w, int cnt, int **a)
{
	int index;
	scanf("%d", &index);
	if (index < 0 || index >= cnt) {
		printf("No matrix with the given index\n");
		return;
	}

	print_matrix(w[index], a[0][index], a[1][index]);
}

void i_want_to_resize_matrix(int ***w, int cnt, int **a)
{
	int index;
	scanf("%d", &index);
	if (index < 0 || index >= cnt) {
		printf("No matrix with the given index\n");
		/*  Continuam cu citirea datelor pentru aceasta comanda,
			chiar daca nu poate sa aiba loc. */
		int n, tmp;
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
			scanf("%d", &tmp);
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
			scanf("%d", &tmp);
		return;
	}

	resize_matrix(&w[index], &a[0][index], &a[1][index]);
}

void i_want_to_multiply(int ****w, int *cnt, int ***a)
{
	int ind1, ind2;
	scanf("%d%d", &ind1, &ind2);
	if (ind1 < 0 || ind1 >= *cnt || ind2 < 0 || ind2 >= *cnt) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*a)[1][ind1] != (*a)[0][ind2]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	int n1 = (*a)[0][ind1], m1 = (*a)[1][ind1], m2 = (*a)[1][ind2];
	(*cnt)++;
	add_columns(*a, 2, *cnt - 1, *cnt);
	(*a)[0][*cnt - 1] = n1;
	(*a)[1][*cnt - 1] = m2;
	add_empty_matrix(w, *cnt);
	(*w)[*cnt - 1] = product((*w)[ind1], (*w)[ind2], n1, m1, m2);
}

void i_want_to_transpose(int ***w, int cnt, int **a)
{
	int index;
	scanf("%d", &index);
	if (index < 0 || index >= cnt) {
		printf("No matrix with the given index\n");
		return;
	}

	int **tmp = w[index];
	w[index] = transpose(w[index], a[0][index], a[1][index]);
	free_matrix(&tmp, a[0][index]);
	swp(&a[0][index], &a[1][index]);
}

void i_want_to_power(int ***w, int cnt, int **a)
{
	int index, pow;
	scanf("%d%d", &index, &pow);
	if (index < 0 || index >= cnt) {
		printf("No matrix with the given index\n");
		return;
	}
	if (pow < 0) {
		printf("Power should be positive\n");
		return;
	}
	if (a[0][index] != a[1][index]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	int **tmp = w[index];
	w[index] = power(w[index], a[0][index], pow);
	free_matrix(&tmp, a[0][index]);
}

void i_want_strassen(int ****w, int *cnt, int ***a)
{
	int ind1, ind2;
	scanf("%d%d", &ind1, &ind2);
	if (ind1 < 0 || ind1 >= *cnt || ind2 < 0 || ind2 >= *cnt) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*a)[0][ind1] != (*a)[0][ind2]) {
		printf("Cannot perfom matrix multiplication\n");
		return;
	}
	if ((((*a)[0][ind1]) & ((*a)[0][ind1] - 1)) != 0) {
		printf("The dimesnions of the matrix are not power of 2\n");
		return;
	}

	int n = (*a)[0][ind1];
	++*cnt;
	add_columns(*a, 2, *cnt - 1, *cnt);
	(*a)[0][*cnt - 1] = n;
	(*a)[1][*cnt - 1] = n;
	add_empty_matrix(w, *cnt);
	(*w)[*cnt - 1] = strassen((*w)[ind1], (*w)[ind2], n);
}

