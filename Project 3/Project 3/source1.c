#include "header.h"
#include <stdlib.h>

matrix *createMatrix(size_t rows, size_t cols, float *data)
{
    matrix *mat = (matrix *)malloc(sizeof(matrix));
    mat->rows = rows;
    mat->cols = cols;
    size_t dataSize = rows * cols;
    mat->data = (float *)malloc(dataSize * sizeof(float));
    for (int i = 0; i < dataSize; i++)
    {
        mat->data[i] = data[i];
    }
    return mat;
}

matrix *copyMatrix(matrix *source)
{
    matrix *cpMat = createMatrix(source->rows, source->cols, source->data);
    return cpMat;
}

matrix *addMatrix(matrix *left, matrix *right)
{
    matrix *mat = NULL;
    if (left->cols == right->cols && left->rows == right->rows)
    {
        size_t dataSize = left->rows * left->cols;
        float *dataNew = (float *)malloc(dataSize * sizeof(float));
        for (int i = 0; i < dataSize; i++)
        {
            dataNew[i] = left->data[i] + right->data[i];
        }
        mat = createMatrix(left->rows, left->cols, dataNew);
    }
    else
    {
        printf(addError);
    }
    return mat;
}

matrix *subtractMatrix(matrix *left, matrix *right)
{
    matrix *mat = NULL;
    if (left->cols == right->cols && left->rows == right->rows)
    {
        size_t dataSize = left->rows * left->cols;
        float *dataNew = (float *)malloc(dataSize * sizeof(float));
        for (int i = 0; i < dataSize; i++)
        {
            dataNew[i] = left->data[i] - right->data[i];
        }
        mat = createMatrix(left->rows, left->cols, dataNew);
    }
    else
    {
        printf(subtractError);
    }
    return mat;
}

matrix *addScalar(matrix *source, size_t scalar)
{
    size_t dataSize = source->rows * source->cols;
    float *dataNew = (float *)malloc(dataSize * sizeof(float));
    for (int i = 0; i < dataSize; i++)
    {
        dataNew[i] = source->data[i] + scalar;
    }
    return createMatrix(source->rows, source->cols, dataNew);
}

matrix *subtractScalar(matrix *source, size_t scalar)
{
    size_t dataSize = source->rows * source->cols;
    float *dataNew = (float *)malloc(dataSize * sizeof(float));
    for (int i = 0; i < dataSize; i++)
    {
        dataNew[i] = source->data[i] - scalar;
    }
    return createMatrix(source->rows, source->cols, dataNew); 
}

matrix *multiplyScalar(matrix *source, size_t scalar)
{
    size_t dataSize = source->rows * source->cols;
    float *dataNew = (float *)malloc(dataSize * sizeof(float));
    for (int i = 0; i < dataSize; i++)
    {
        dataNew[i] = source->data[i] * scalar;
    }
    return createMatrix(source->rows, source->cols, dataNew);
}

matrix *multiplyMatrix(matrix *left, matrix *right)
{
    matrix *mat = NULL;
    if (left->cols == right->rows)
    {
        size_t dataSize = left->rows * right->cols;
        float *dataNew = (float *)malloc(dataSize * sizeof(float));
        for (int i = 0; i < dataSize; i++)
        {
            for (int j = 0; j < left->cols; j++)
            {
                dataNew[i] += left->data[(i / right->cols) * right->cols + j] * right->data[i % right->cols + j * right->cols];
            }
        }
        mat = createMatrix(left->rows, right->cols, dataNew);
    }
    else
    {
        printf(multiplyError);
    }
    return mat;
}

float matrix_Max(matrix *source)
{
    if (source == NULL)
    {
        printf(maxError);
        return 0;
    }
    else
    {
        size_t dataSize = source->rows * source->cols;
        float max = source->data[0];
        for (int i = 1; i < dataSize; i++)
        {
            if (source->data[i] > max)
            {
                max = source->data[i];
            }
        }
        return max;
    }
}

float matrix_Min(matrix *source)
{
    if (source == NULL)
    {
        printf(minError);
        return 0;
    }
    else
    {
        size_t dataSize = source->rows * source->cols;
        float min = source->data[0];
        for (int i = 1; i < dataSize; i++)
        {
            if (source->data[i] < min)
            {
                min = source->data[i];
            }
        }
        return min;
    }
}

matrix *transpose(matrix *source)
{
    matrix *mat = NULL;
    if (source == NULL)
    {
        printf(transposeError);
    }
    else
    {
        size_t dataSize = source->rows * source->cols;
        float *dataNew = (float *)malloc(dataSize * sizeof(float));
        for (int i = 0; i < dataSize; i++)
        {
            dataNew[i] = source->data[(i % source->rows) * source->cols + i / source->rows];
        }
        mat = createMatrix(source->cols, source->rows, dataNew);
    }
    return mat;
}

void printMatrix(matrix *source)
{
    if (source == NULL)
    {
        printf(printError);
    }
    else
    {
        printf("matrix_%p:\n", source);
        for (int i = 0; i < source->rows; i++)
        {
            for (int j = 0; j < source->cols; j++)
            {
                printf("%f ", source->data[i * source->cols + j]);
            }
            printf("\n");
        }
    }
}