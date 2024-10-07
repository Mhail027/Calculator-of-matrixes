// Copyright Necula Mihail 313CAa 2023-2024

/* Aloca dinamic memorie pentru un vector cu n elemente de tip int.
Returneaza un pointer catre acea zona de memorie daca alocarea a avut
loc cu succes. In caz contrar, returneaza pointerul NULL. */
int *malloc_vector(int n)
{
	int *v = (int *)malloc(n * sizeof(int));
	if (!v) {
		fprintf(stderr, "malloc_vector() failed\n");
		return NULL;
	}

	return v;
}

/* Aloca dinamic memorie pentru o matrice cu n linii si m coloane.
Returneaza un pointer catre acea zona de memorie daca alocarea a avut
loc cu succes. In caz contrar, returneaza pointerul NULL. */
int **malloc_matrix(int n, int m)
{
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		fprintf(stderr, "malloc_matrix() failed\n");
		return NULL;
	}

	for (int i = 0; i < n; ++i) {
		a[i] = (int *)malloc(m * sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "malloc_matrix() failed\n");
			for (int tmp = 0; tmp <= i; ++tmp)
				free(a[tmp]);
			free(a);
			return NULL;
		}
	}

	return a;
}

/* Aloca dinamic memorie pentru un tablou tridimmensional cu cnt matrici
de dimenisuni 0 x 0.
Returneaza un pointer catre acea zona de memorie daca alocarea a avut
loc cu succes. In caz contrar, returneaza pointerul NULL. */
int ***malloc_empty_tridim(int cnt)
{
	int ***p = (int ***)malloc(cnt * sizeof(int **));
	if (!p) {
		fprintf(stderr, "malloc_tridim() failed");
		return NULL;
	}

	return p;
}

/* Aloca dinamic memorie pentru un tablou tridimmensional cu cnt matrici.
Dimensiunile acestora se reagasesc in a, astfel:
	a[0][i] = numarul de linii ale matricei cu numarul i,
	a[1][i] = numarul de coloane ale matricei cu numarul i.
Returneaza un pointer catre acea zona de memorie daca alocarea a avut
loc cu succes. In caz contrar, returneaza pointerul NULL. */
int ***malloc_tridim(int cnt, int **a)
{
	int ***w = (int ***)malloc(cnt * sizeof(int **));
	if (!w) {
		fprintf(stderr, "malloc_tridim() failed");
		return NULL;
	}

	for (int k = 0; k < cnt; ++k) {
		w[k] = malloc_matrix(a[0][k], a[1][k]);
			if (!w[k]) {
				fprintf(stderr, "malloc_tridim() failed\n");
				for (int i = 0; i < k; ++i) {
					for (int j = 0; j < a[0][k]; ++j)
						free(w[i][j]);
					free(w[i]);
				}
				free(w);
				return NULL;
			}
		}

	return w;
}

/* Elibereaza memoria alocata pentru matricea care se afla la adresa *a.
n = numarul de linii ale maricii */
void free_matrix(int ***a, int n)
{
	for (int i = 0; i < n; ++i)
		free((*a)[i]);
	free(*a);
}

/*  Elibereaza memoria alocata pentru tabloul tridimensional
care se afla la adresa *w.
cnt = numarul de matrici ale tabloului tridimenional
Dimensiunile acestora se reagasesc in a, astfel:
	a[0][i] = numarul de linii ale matricei cu numarul i,
	a[1][i] = numarul de coloane ale matricei cu numarul i. */
void free_tridim(int ****w, int cnt, int **a)
{
	for (int k = 0; k < cnt; ++k)
		free_matrix(&(*w)[k], a[0][k]);
	free(*w);
}

/*  Initial, matricea a are m_old coloane.
Scopul acestei functii este de a a dauga coloane la tabloul respectiv
pana cand numarul lor devine egal cu m_new.
n = numarul de linii ale matricei a */
void add_columns(int **a, int n, int m_old, int m_new)
{
	for (int i = 0; i < n; ++i) {
		a[i] = (int *)realloc(a[i], m_new * sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "add_columns() failed\n");
			for (int j = i - 1; j >= 0; --j)
				a[i] = (int *)realloc(a[i], m_old * sizeof(int));
			break;
		}
	}
}

/*  Aloca o matrice de dimensiune 0 x 0 la sfarsitul tabloului
tridimensional la care pointeaza w, pe pozitia cnt - 1. */
void add_empty_matrix(int ****w, int cnt)
{
	*w = (int ***)realloc(*w, cnt * sizeof(int **));
	if (!*w)
		fprintf(stderr, "add_empty_matrix() failed");
}

/*  Aloca o matrice de dimensiune n x m la sfarsitul tabloului
tridimensional la care pointeaza w, pe pozitia cnt - 1. */
void add_matrix(int ****w, int cnt, int n, int m)
{
	*w = (int ***)realloc(*w, cnt * sizeof(int **));
	if (!*w) {
		fprintf(stderr, "add_matrix() failed");
		return;
	}

	(*w)[cnt - 1] = malloc_matrix(n, m);
	if (!(*w)[cnt - 1]) {
		fprintf(stderr, "add_matrix() failed\n");
		*w = (int ***)realloc(*w, (cnt - 1) * sizeof(int **));
	}
}

/* w - adresa unui pointer la un tablou tridimensional
cnt - face trimitere la numarul de matrici ale tabloului tridimensional
a - pointeaza catre un tablou bidimensional in care se afla
dimensiunile matricilor, asfel:
	(*a)[0][i] = numarul de linii ale matricei cu numarul i,
	(*a)[1][i] = numarul de coloane ale matricei cu numarul i.
Functia elimina matricea cu numarul index din tabloul tridimensional (*w)
si actualizeaza cnt si a, daca matricea respcetiva exsita. In caz contrar,
afiseaza un mesaj de eroare pe ecran si se intoarce in functia de unde a
fost apelata. */
void remove_matrix(int ****w, int *cnt, int ***a)
{
	int index;
	scanf("%d", &index);
	if (index < 0 || index >= *cnt) {
		printf("No matrix with the given index\n");
		return;
	}

	// Eliminam matricea cu numarul index din tabloul tridimensional (*w).
	for (int i = 0; i < (*a)[0][index]; ++i)
		free((*w)[index][i]);
	free((*w)[index]);
	--*cnt;

	/* Mutam matricile si datele despre ele cu o pozitie mai spre stanga,
		daca se aflau in dreapta matricei eliminate. */
	for (int i = index; i < *cnt; ++i) {
		(*w)[i] = (*w)[i + 1];
		(*a)[0][i] = (*a)[0][i + 1];
		(*a)[1][i] = (*a)[1][i + 1];
	}

	// Realocam sau eliberam memoria, in functie de caz.
	if (*cnt > 0) {
		*w = (int ***)realloc(*w, *cnt * sizeof(int **));
		(*a)[0] = (int *)realloc((*a)[0],  *cnt * sizeof(int));
		(*a)[1] = (int *)realloc((*a)[1],  *cnt * sizeof(int));
	} else {
		free(*w);
		free((*a)[0]);
		free((*a)[1]);
		free(*a);
	}
}

/* Realoca memoria matricei la care pointeaza a si apoi actualizeaza
elementele sale in functie de datele citite de la tastatura.
Initial, matricea are *n linii si *m coloane. */
void resize_matrix(int ***a, int *n, int *m)
{
	int n_new;
	scanf("%d", &n_new);
	// n_new = numarul de linii dupa redimensionare
	int **l = malloc_matrix(n_new, *m);
	// l va contine matricea initiala dupa redimensionarea liniilor
	for (int line = 0; line < n_new; ++line) {
		int i;
		scanf("%d", &i);
		// i reprezinta linia pe care vrem sa o copiem in noua matrice, l
		for (int j = 0; j < *m; ++j)
			l[line][j] = (*a)[i][j];
		}

	int m_new;
	scanf("%d", &m_new);
	// m_new = numarul de coloane dupa redimensionare
	int **lc = malloc_matrix(n_new, m_new);
	/*  lc va contine matricea initiala dupa redimensionarea
		liniilor si coloanelor */
	for (int colloum = 0; colloum < m_new; ++colloum) {
		int j;
		scanf("%d", &j);
		// j reprezinta coloana pe care vrem sa o copiem in noua matrice, lc
			for (int i = 0; i < n_new; ++i)
				lc[i][colloum] = l[i][j];
		}

	free_matrix(a, *n);
	free_matrix(&l, n_new);
	*a = lc;
	*n = n_new;
	*m = m_new;
}

