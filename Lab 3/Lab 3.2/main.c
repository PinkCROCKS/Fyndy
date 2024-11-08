#include "solution.h"
int main() {
    int n = 3;
    int n_vectors = 3;
    int n_norms = 3;
    Vector** vectors = NULL;
    double** norms = NULL;
    double** A = NULL;
    int p = 3;
    char** result = NULL;
    int err = Allocate(&norms, &vectors, n_vectors, n_norms, n, &A, &result);
    if (err) {
        printf("Memory leak\n");
        return Memory_leak;
    }

    generate_positive_definite_matrix(n, A);

    for (int i = 0; i < n_vectors; ++i) {
        vectors[i]->n = n;
    }
    vectors[0]->coords[0] = 1.0;
    vectors[0]->coords[1] = 7.0;
    vectors[0]->coords[2] = 3.0;

    vectors[1]->coords[0] = -4.0;
    vectors[1]->coords[1] = 5.0;
    vectors[1]->coords[2] = -6.0;

    vectors[2]->coords[0] = -6.0;
    vectors[2]->coords[1] = -2.0;
    vectors[2]->coords[2] = -5.0;

    calculate_norms(vectors, norms, n_vectors, n_norms, norm1, norm2, p, norm3, A);

    max_norm(vectors, norms, n_vectors, n_norms, result);

    for (int i = 0; i < n_norms; ++i) {
        printf("%s", result[i]);
    }
    delete(&norms, &vectors, n_vectors, n_norms, n, &A, &result);
    return 0;
}