#include "math/matrix_nd.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

MatrixND CreateIdentityND(int dim) {
    MatrixND mat;
    mat.dim = dim;
    mat.size = dim + 1;
    mat.data = (double*)calloc(mat.size * mat.size, sizeof(double));
    
    for (int i = 0; i < mat.size; i++) {
        mat.data[i * mat.size + i] = 1.0;
    }
    return mat;
}

void FreeMatrixND(MatrixND *mat) {
    if (mat->data) {
        free(mat->data);
        mat->data = NULL;
    }
}

// Eltolás beállítása az utolsó oszlopban
void SetTranslationND(MatrixND *mat, double *translation_vector) {
    for (int i = 0; i < mat->dim; i++) {
        mat->data[i * mat->size + (mat->size - 1)] = translation_vector[i];
    }
}

// Forgatás egy konkrét koordináta-sík (i és j tengelyek között) mentén
void SetHyperRotationND(MatrixND *mat, int axis_i, int axis_j, double radians) {
    double c = cos(radians);
    double s = sin(radians);
    
    // Alaphelyzetbe hozzuk a mátrixot (Identity)
    for (int row = 0; row < mat->size; row++) {
        for (int col = 0; col < mat->size; col++) {
            mat->data[row * mat->size + col] = (row == col) ? 1.0 : 0.0;
        }
    }
    
    mat->data[axis_i * mat->size + axis_i] = c;
    mat->data[axis_i * mat->size + axis_j] = -s;
    mat->data[axis_j * mat->size + axis_i] = s;
    mat->data[axis_j * mat->size + axis_j] = c;
}

// Homogén vektor transzformáció: out = M * in
void MultiplyMatrixVectorND(MatrixND *mat, const double *in_vec, double *out_vec) {
    double *tmp_in = (double*)malloc(mat->size * sizeof(double));
    memcpy(tmp_in, in_vec, mat->dim * sizeof(double));
    tmp_in[mat->size - 1] = 1.0; // Homogén koordináta (W_n = 1)

    double *tmp_out = (double*)calloc(mat->size, sizeof(double));

    for (int row = 0; row < mat->size; row++) {
        for (int col = 0; col < mat->size; col++) {
            tmp_out[row] += mat->data[row * mat->size + col] * tmp_in[col];
        }
    }

    // Homogén osztás és visszaírás a kimeneti N-dimenziós vektorba
    double w = tmp_out[mat->size - 1];
    if (fabs(w) < 0.000001) w = 1.0;
    
    for (int i = 0; i < mat->dim; i++) {
        out_vec[i] = tmp_out[i] / w;
    }

    free(tmp_in);
    free(tmp_out);
}
