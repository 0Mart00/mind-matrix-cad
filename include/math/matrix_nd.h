#ifndef MMC_MATRIX_ND_H
#define MMC_MATRIX_ND_H

#include "sdk/ncad_sdk.h"

// N-dimenziós transzformációhoz egy (N+1)x(N+1)-es homogén mátrix kell
typedef struct {
    int dim;        // A tér dimenziószáma (pl. 5 esetén a mátrix 6x6-os)
    int size;       // dim + 1
    double *data;   // size * size méretű lapított tömb
} MatrixND;

MatrixND CreateIdentityND(int dim);
void FreeMatrixND(MatrixND *mat);
void SetTranslationND(MatrixND *mat, double *translation_vector);
void SetHyperRotationND(MatrixND *mat, int axis_i, int axis_j, double radians);
void MultiplyMatrixVectorND(MatrixND *mat, const double *in_vec, double *out_vec);

#endif
