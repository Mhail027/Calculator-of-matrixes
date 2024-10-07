// Copyright Necula Mihail 313CAa 2023-2024

#include <stdio.h>
#include <stdlib.h>
#include "alloc.c"
#include "matrix.c"
#include "octave.c"

int main(void)
{
	int ***w, **a, cnt = 0;
	char c;
	do {
		scanf("%c", &c);
		if (c == '\n')
			scanf("%c", &c);

		switch (c) {
		case 'L':
			++cnt;
			if (cnt == 1) {
				a = malloc_matrix(2, 1);
				read_column(a, 2, 0);
				w = malloc_tridim(1, a);
				read_matrix(w[0], a[0][0], a[1][0]);
			} else {
				add_columns(a, 2, cnt - 1, cnt);
				read_column(a, 2, cnt - 1);
				add_matrix(&w, cnt, a[0][cnt - 1], a[1][cnt - 1]);
				read_matrix(w[cnt - 1], a[0][cnt - 1], a[1][cnt - 1]);
			}
			break;
		case 'D':
			i_want_matrix_dimensions(cnt, a);
			break;
		case 'P':
			i_want_to_print_matrix(w, cnt, a);
			break;
		case 'C':
			i_want_to_resize_matrix(w, cnt, a);
			break;
		case 'M':
			i_want_to_multiply(&w, &cnt, &a);
			break;
		case 'O':
			sort(&w, cnt, &a);
			break;
		case 'T':
			i_want_to_transpose(w, cnt, a);
			break;
		case 'R':
			i_want_to_power(w, cnt, a);
			break;
		case 'F':
			remove_matrix(&w, &cnt, &a);
			break;
		case 'S':
			i_want_strassen(&w, &cnt, &a);
			break;
		case 'Q':
			if (cnt) {
				free_tridim(&w, cnt, a);
				free_matrix(&a, 2);
			}
			break;
		default:
			printf("Unrecognized command\n");
		}
	} while (c != 'Q');
	return 0;
}

