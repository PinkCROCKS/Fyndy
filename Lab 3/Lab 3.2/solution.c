#include "solution.h"

int Allocate(double*** norms, Vector*** vectors, int n_vectors, int n_norms, int n, double*** A, char*** result) {
    *norms = (double**)(malloc(sizeof(double*) * n_norms));
    if (!*norms)
        return Memory_leak;
    for (int i = 0; i < n_norms; ++i) {
        (*norms)[i] = (double *)(malloc(sizeof(double ) * n_vectors));
        if (!(*norms)[i]) {
            for (int j = 0; j < i; ++j) {
                free((*norms)[j]);
            }
            free(*norms);
            return Memory_leak;
        }
    }

    *vectors = (Vector**)(malloc(sizeof(Vector*) * n_vectors));
    if (!*vectors) {
        for (int i = 0; i < n_norms; ++i) {
            free((*norms)[i]);
        }
        free(*norms);
        return Memory_leak;
    }

    for (int i = 0; i < n_vectors; ++i) {
        (*vectors)[i] = (Vector*)(malloc(sizeof(Vector)));
        if (!(*vectors)[i]) {
            for (int j = 0; j < n_norms; ++j) {
                free((*norms)[j]);
            }
            free(*norms);
            for (int j = 0; j < i ; ++j) {
                free((*vectors)[j]);
            }
            free(*vectors);
            return Memory_leak;
        }
        (*vectors)[i]->coords = (double *)(malloc(sizeof(double ) * n));
        if (!(*vectors)[i]->coords) {
            for (int j = 0; j < n_norms; ++j) {
                free((*norms)[j]);
            }
            free(*norms);
            for (int j = 0; j < i; ++j) {
                free((*vectors)[j]->coords);
                free((*vectors)[j]);
            }
            free(*vectors);
            return Memory_leak;
        }
    }

    *A = (double**)(malloc(sizeof(double*) * n));
    if (!*A) {
        for (int i = 0; i < n_norms; ++i) {
            free((*norms)[i]);
        }
        free(*norms);
        for (int i = 0; i < n_vectors; ++i) {
            free((*vectors)[i]->coords);
            free((*vectors)[i]);
        }
        free(*vectors);
        return Memory_leak;
    }
    for (int i = 0; i < n; ++i) {
        (*A)[i] = (double*)(malloc(sizeof(double ) * n));
        if (!(*A)[i]) {
            for (int j = 0; j < n_norms; ++j) {
                free((*norms)[j]);
            }
            free(*norms);
            for (int j = 0; j < n_vectors; ++j) {
                free((*vectors)[j]->coords);
                free((*vectors)[j]);
            }
            free(*vectors);
            for (int j = 0; j < i; ++j) {
                free((*A)[j]);
            }
            free(*A);
            return Memory_leak;
        }
    }
    *result = (char**)(malloc(sizeof(char *) * n_norms));
    if (!(*result)) {
        for (int i = 0; i < n; ++i) {
            free((*A)[i]);
        }
        free(*A);
        for (int i = 0; i < n_norms; ++i) {
            free((*norms)[i]);
        }
        free((*norms));
        for (int i = 0; i < n_vectors; ++i) {
            free((*vectors)[i]);
            free((*vectors)[i]->coords);
        }
        free(*vectors);
        return Memory_leak;
    }
    for (int i = 0; i < n_norms; ++i) {
        (*result)[i] = (char *)(malloc(sizeof(char ) * 128));
        if (!(*result)[i]) {
            for (int j = 0; j < n; ++j) {
                free((*A)[i]);
            }
            free(*A);
            for (int j = 0; j < n_norms; ++j) {
                free((*norms)[i]);
            }
            free((*norms));
            for (int j = 0; j < n_vectors; ++j) {
                free((*vectors)[j]);
                free((*vectors)[j]->coords);
            }
            free(*vectors);

            for (int j = 0; j < i; ++j) {
                free((*result)[j]);
            }
            free(*result);
            return Memory_leak;
        }
    }
    return 0;
}

void delete(double*** norms, Vector*** vectors, int n_vectors, int n_norms, int n, double*** A, char*** result) {
    for (int i = 0; i < n_norms; ++i) {
        free((*norms)[i]);
    }
    free(*norms);
    for (int i = 0; i < n_vectors; ++i) {
        free((*vectors)[i]->coords);
        free((*vectors)[i]);
    }
    free(*vectors);
    for (int i = 0; i < n; ++i) {
        free((*A)[i]);
    }
    free(*A);
    for (int i = 0; i < n_norms; ++i) {
        free((*result)[i]);
    }
    free(*result);
}

int generate_positive_definite_matrix(int n, double **P) {
    double **C = (double **)malloc(n * sizeof(double *));
    if (!C)
        return Memory_leak;
    for (int i = 0; i < n; i++) {
        C[i] = (double *)malloc(n * sizeof(double));
        if (!C[i]) {
            for (int j = 0; j < i; ++j) {
                free(C[j]);
            }
            free(C);
            return Memory_leak;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                C[i][j] = 1;
            else
                C[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            P[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                P[i][j] += C[i][k] * C[j][k];
            }
        }
    }

    for (int i = 0; i < n; i++) {
        free(C[i]);
    }
    free(C);
    return 0;
}

double norm1(Vector* v, void* args) {
    double max_coord = INT_MIN;
    for (int i = 0; i < v->n; ++i) {
        if (fabs(v->coords[i]) > max_coord)
            max_coord = fabs(v->coords[i]);
    }
    return max_coord;
}

double norm2(Vector* v, void* args) {
    int p = *(int*)args;
    double sum = 0.0;
    for (int i = 0; i < v->n; ++i) {
        sum += pow(fabs(v->coords[i]), p);
    }
    return pow(sum, 1.0/p);
}

double norm3(Vector* v, void* args) {
    int dimension = v->n;
    double** A = (double**)args;
    double sum = 0.0;
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            sum += A[i][j] * v->coords[j] * v->coords[i];
        }
    }
    return sum;
}

void calculate_norms(Vector** vectors, double** norms, int n_vectors, int n_norms, ...) {
    va_list funcs;
    va_start(funcs, n_norms);

    for (int i = 0; i < n_norms; ++i) {
        double (*norm)(Vector*, void*) = va_arg(funcs, double(*)(Vector*, void*));
        if (norm == norm1) {
            for (int j = 0; j < n_vectors; ++j) {
                norms[i][j] = norm1(vectors[j], NULL);
            }
        } else if (norm == norm2) {
            int p = va_arg(funcs, int);
            for (int j = 0; j < n_vectors; ++j) {
                norms[i][j] = norm2(vectors[j], &p);
            }
        } else if (norm == norm3) {
            double** A = va_arg(funcs, double**);
            for (int j = 0; j < n_vectors; ++j) {
                norms[i][j] = norm3(vectors[j], A);
            }
        }
    }
    va_end(funcs);
}

void max_norm(Vector** vectors, double** norms, int n_vectors, int n_norms, char** result) {
    for (int i = 0; i < n_norms; ++i) {
        double max = INT_MIN;
        int max_index = 0;
        for (int j = 0; j < n_vectors; ++j) {
            if (norms[i][j] >= max) {
                max = norms[i][j];
                max_index = j;
            }
        }
        char buffer[128];
        sprintf(buffer, "norm %d max %f\n", i + 1, max);
        for (int j = 0; j < vectors[max_index]->n; ++j) {
            sprintf(buffer + strlen(buffer), "x%d = %f ", j + 1, vectors[max_index]->coords[j]);
        }
        sprintf(buffer + strlen(buffer), "\n");
        if (result[i])
            free(result[i]);
        result[i] = strdup(buffer);
    }
}