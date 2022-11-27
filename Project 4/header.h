#include <stdio.h>

typedef struct matrix
{
    size_t row;
    size_t col;
    float *data;
} matrix;

matrix *genMat(size_t row, size_t col, float *data);

matrix *matmul_plain(matrix *mat1, matrix *mat2);

matrix *matmul_access_kj(matrix *mat1, matrix *mat2);

matrix *matmul_access_trans(matrix *mat1, matrix *mat2);

matrix *matmul_avx2(matrix *mat1, matrix *mat2);

matrix *matmul_openmp(matrix *mat1, matrix *mat2);

matrix *matmul_avx2_openmp(matrix *mat1, matrix *mat2);

matrix *matmul_OPENBLAS(matrix *mat1, matrix *mat2);

matrix *randMat(size_t size);

void printMatrix(matrix *source);

void measureTime(matrix *(*p)(matrix *, matrix *), matrix *mat1, matrix *mat2, char s[]); //以函数指针类型传入参数调用函数，减少代码的大量重复

int compare(matrix *mat1,matrix *mat2);

#define delMat(source)                                 \
    {                                                  \
        free(source->data);                            \
        printf("Matrix_%p has been freed!\n", source); \
        free(source);                                  \
        source = NULL;                                 \
    }
