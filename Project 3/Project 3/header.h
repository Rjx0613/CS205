#include <stdio.h>
#include <malloc.h>

typedef struct matrix
{
    size_t rows;
    size_t cols;
    float *data;
} matrix;

matrix *createMatrix(size_t rows, size_t cols, float *data);

#define deleteMatrix(source)                           \
    {                                                  \
        free(source->data);                            \
        printf("Matrix_%p has been freed!\n", source); \
        free(source);                                  \
        source = NULL;                                 \
    }

matrix *copyMatrix(matrix *source);

matrix *addMatrix(matrix *left, matrix *right);

matrix *subtractMatrix(matrix *left, matrix *right);

matrix *addScalar(matrix *source, size_t scalar);

matrix *subtractScalar(matrix *source, size_t scalar);

matrix *multiplyScalar(matrix *source, size_t scalar);

matrix *multiplyMatrix(matrix *left, matrix *right);

matrix *transpose(matrix *source);

float matrix_Max(matrix *source);

float matrix_Min(matrix *source);

void printMatrix(matrix *source);

#define addError "Dimension error: #Column or #row doesn't equal, causing addError!\n"

#define subtractError "Dimension error: #Column or #row doesn't equal, causing subtractError!\n"

#define multiplyError "Dimention error: #left(col) doesn't equal #right(row), causing multiplyError!\n"

#define printError "Can not print: Error in matrix!\n"

#define maxError "Can not find max in matrix: Error in matrix\n"

#define minError "Can not find min in matrix: Error in matrix\n"

#define transposeError "Can not transpose: Error in matrix\n"