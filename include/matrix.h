
#ifndef __matrix__H__
#define __matrix__H__

#include "defs_p.h"

typedef struct 
{

double A[3][3];
} MAT;

typedef struct 
{

double A[3];
} VEC;

MAT mat_mat(MAT mat_a,MAT mat_b);
VEC mat_vec(MAT mat_a,VEC vec_a);
MAT mat_scalar(MAT mat_a, double x);
MAT mat_inv(MAT mat_a);
MAT mat_t(MAT mat_a);
MAT mat_add_mat(MAT mat_a,MAT mat_b);
MAT mat_I();
MAT mat_skew(double x, double y,double z);
MAT mat_inv_GJ(MAT _a,int n);
VEC vec_add_scalar(VEC vec, FLOAT32 x,FLOAT32 modulu);
VEC vec_scalar(VEC vec, FLOAT32 x);
#endif
