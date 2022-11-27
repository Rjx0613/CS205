#include "header.h"
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <omp.h>
#include "time.h"
#include <immintrin.h>
#include <string.h>
#include <sys/time.h>
#include <cblas.h>

clock_t start, end;
struct timeval t1, t2;

matrix *genMat(size_t row, size_t col, float *data)
{
    matrix *mat = (matrix *)malloc(sizeof(matrix));
    mat->row = row;
    mat->col = col;
    mat->data = data;
    return mat;
}

matrix *subtractMatrix(matrix *left, matrix *right)
{
    matrix *mat = NULL;
    size_t dataSize = left->row * left->col;
    float *dataNew = (float *)malloc(dataSize * sizeof(float));
#pragma omp parallel for
    for (size_t i = 0; i < dataSize; i++)
    {
        dataNew[i] = left->data[i] - right->data[i];
    }
    mat = genMat(left->row, left->col, dataNew);
    return mat;
}

matrix *matmul_plain(matrix *mat1, matrix *mat2) //左矩阵的数据访问为每次加一，按行存储，即顺序访问，对于右矩阵的数据访问处于不连续状态，跳行访问，cpu缓存命中率被极大程度降低，运算较慢
{
    matrix *mat = NULL;
    size_t dataSize = mat1->row * mat2->col;
    float *dataNew = (float *)malloc(dataSize * sizeof(float));
    for (size_t i = 0; i < dataSize; i++)
    {
        for (size_t j = 0; j < mat1->col; j++)
        {
            dataNew[i] += mat1->data[(i / mat2->col) * mat2->col + j] * mat2->data[i % mat2->col + j * mat2->col];
        }
    }
    mat = genMat(mat1->row, mat2->col, dataNew);
    return mat;
}

matrix *matmul_access_kj(matrix *mat1, matrix *mat2) //对比，交换j，k顺序，使内存访问连续
{
    size_t i, j, k;
    matrix *result = (matrix *)malloc(sizeof(matrix));
    result->row = mat1->row;
    result->col = mat2->col;
    result->data = (float *)aligned_alloc(256, mat1->row * mat2->col * sizeof(float));
    for (i = 0; i < mat1->row; i++)
    {
        for (k = 0; k < mat1->col; k++)
        {
            float temp = mat1->data[i * mat1->col + k];
            for (j = 0; j < mat2->col; j++)
            {
                result->data[i * mat2->col + j] += temp * (mat2->data[k * mat2->col + j]);
            }
        }
    }
    return result;
}

//提升cpu缓存命中率，先对右矩阵进行转置：耗时相对于乘法运算可以忽略不计；将行列对应变成行行对应
matrix *matmul_access_trans(matrix *mat1, matrix *mat2)
{
    matrix *mat = NULL;
    size_t dataSize = mat1->row * mat2->col;
    size_t rdataSize = mat2->row * mat2->col;
    float *result = (float *)malloc(dataSize * sizeof(float));
    float *rnewData = (float *)malloc(rdataSize * sizeof(float));

    for (size_t i = 0; i < rdataSize; i++)
    { //转置右矩阵的数据
        size_t n = i / mat2->row;
        size_t N = i % mat2->row;
        rnewData[i] = mat2->data[mat2->col * N + n];
    }

    for (size_t i = 0; i < mat1->row; i++) // to be discussed,why this as slow as the plain?
    {
        for (size_t j = 0; j < mat2->col; j++)
        {
            for (size_t k = 0; k < mat1->col; k++)
            {
                result[i * mat2->col + j] += mat1->data[i * mat1->col + k] * rnewData[j * mat2->row + k];
            }
        }
    }

    mat = genMat(mat1->row, mat2->col, result);
    return mat;
}

matrix *matmul_openmp(matrix *mat1, matrix *mat2) //多线程运行循环加速
{
    matrix *mat = (matrix *)malloc(sizeof(matrix));
    mat->row = mat1->row;
    mat->col = mat2->col;
    size_t dataSize = mat1->row * mat2->col;
    mat->data = (float *)malloc(dataSize * sizeof(float));

    size_t i, j, k;
#pragma omp parallel for
    for (i = 0; i < mat1->row; i++)
    {
        for (k = 0; k < mat1->col; k++)
        {
            float temp = mat1->data[i * mat1->col + k];
            for (j = 0; j < mat2->col; j++)
            {
                mat->data[i * mat2->col + j] += temp * mat2->data[k * mat2->col + j];
            }
        }
    }
    return mat;
}

matrix *matmul_avx2(matrix *mat1, matrix *mat2)
{
    matrix *mat = (matrix *)malloc(sizeof(matrix));
    mat->row = mat1->row;
    mat->col = mat2->col;
    __m256 a, b, c;
    float *result = aligned_alloc(256, mat1->row * mat2->col * sizeof(float));
    memset(result, 0.0f, mat1->row * mat2->col);
    mat->data = result;
    size_t i, j, k;
    for (i = 0; i < mat1->row; i++)
    {
        for (j = 0; j < mat1->col; j++)
        {
            a = _mm256_set1_ps(mat1->data[i * mat1->col + j]);
            for (k = 0; k < mat2->col; k += 8)
            {
                b = _mm256_loadu_ps(&mat2->data[j * mat2->col + k]);
                c = _mm256_loadu_ps(&result[i * mat2->col + k]);
                c = _mm256_fmadd_ps(a, b, c);
                _mm256_storeu_ps(&result[i * mat2->col + k], c);
            }
        }
    }
    return mat;
}

matrix *matmul_avx2_openmp(matrix *mat1, matrix *mat2)
{
    matrix *mat = (matrix *)malloc(sizeof(matrix));
    mat->row = mat1->row;
    mat->col = mat2->col;

    __m256 a, b, c;
    float *result = aligned_alloc(256, mat1->row * mat2->col * sizeof(float));

    memset(result, 0.0f, mat1->row * mat2->col);
    mat->data = result;
    size_t i, j, k;

#pragma omp parallel for
    for (i = 0; i < mat1->row; i++)
    {
        for (j = 0; j < mat1->col; j++)
        {
            a = _mm256_set1_ps(mat1->data[i * mat1->col + j]);
            for (k = 0; k < mat2->col; k += 8)
            {
                b = _mm256_loadu_ps(&mat2->data[j * mat2->col + k]);
                c = _mm256_loadu_ps(&result[i * mat2->col + k]);
                c = _mm256_fmadd_ps(a, b, c);
                _mm256_storeu_ps(&result[i * mat2->col + k], c);
            }
        }
    }
    return mat;
}

matrix *matmul_OPENBLAS(matrix *mat1, matrix *mat2)
{
    matrix *result = malloc(sizeof(matrix));
    result->row = mat1->row;
    result->col = mat2->col;
    result->data = aligned_alloc(256, mat1->row * mat2->col * sizeof(float));
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1->row, mat2->col, mat1->col, 1.0f, mat1->data, mat1->col, mat2->data, mat2->col, 0.0f, result->data, mat2->col);
    return result;
}

matrix *randMat(size_t size)
{
    float *data = aligned_alloc(256, size * size * sizeof(float));
    srand(time(0));
    for (size_t i = 0; i < size * size; i++)
    {
        data[i] = -10.0 + 1.0 * rand() / RAND_MAX * 20.0; //生成[-10.0,10.0]区间的随机浮点数
    }
    return genMat(size, size, data);
}

void printMatrix(matrix *source)
{
    printf("matrix_%p:\n", source);
    for (size_t i = 0; i < source->row; i++)
    {
        for (size_t j = 0; j < source->col; j++)
        {
            printf("%f ", source->data[i * source->col + j]);
        }
        printf("\n");
    }
}

void measureTime(matrix *(*p)(matrix *, matrix *), matrix *mat1, matrix *mat2, char s[])
{
    gettimeofday(&t1, NULL);
    matrix *mat = p(mat1, mat2);
    gettimeofday(&t2, NULL);
    int sec = t2.tv_sec - t1.tv_sec;
    float micsec = (float)(t2.tv_usec - t1.tv_usec) / 1000000;
    printf("%s:%.5fs\n", s, sec + micsec);
    delMat(mat);
}

int compare(matrix *mat1, matrix *mat2)
{
    matrix *difference = subtractMatrix(mat1, mat2);
    int isTrue = 1;
#pragma omp parallel for
    for (size_t i = 0; i < difference->row; i++)
    {
        for (size_t j = 0; j < difference->col; j++)
        {
            if (abs(difference->data[i * difference->col + j]) > 0.0001)
            {
                isTrue = 0;
            }
        }
    }
    return isTrue;
}

int main()
{

    matrix *mat1 = randMat(2048);
    matrix *mat2 = randMat(2048);

    matmul_access_kj(randMat(500), randMat(500));
    matmul_access_kj(randMat(500), randMat(500));
    matmul_access_kj(randMat(500), randMat(500));
    //需要空跑几次预热

    matrix *mat3 = matmul_OPENBLAS(mat1, mat2);
    // printMatrix(mat3);

    measureTime(matmul_plain, mat1, mat2, "plain");
    // printMatrix(subtractMatrix(mat3,matmul_plain(mat1, mat2)));
    matrix *mat4=matmul_plain(mat1, mat2);
    printf("%d\n\n", compare(mat3, mat4));

    measureTime(matmul_access_trans, mat1, mat2, "accesss_trans");
    // printMatrix(subtractMatrix(mat3,matmul_access_trans(mat1, mat2)));
    matrix *mat5=matmul_access_trans(mat1, mat2);
    printf("%d\n\n", compare(mat3, matmul_access_trans(mat1, mat2)));

    measureTime(matmul_access_kj, mat1, mat2, "access:kj");
    // printMatrix(subtractMatrix(mat3,matmul_access_kj(mat1, mat2)));
    matrix *mat6=matmul_access_kj(mat1, mat2);
    printf("%d\n\n", compare(mat3, matmul_access_kj(mat1, mat2)));

    measureTime(matmul_openmp, mat1, mat2, "openmp");
    // printMatrix(subtractMatrix(mat3,matmul_openmp(mat1, mat2)));
    matrix *mat7=matmul_openmp(mat1, mat2);
    printf("%d\n\n", compare(mat3, matmul_openmp(mat1, mat2)));

    measureTime(matmul_avx2, mat1, mat2, "avx2");
    // printMatrix(subtractMatrix(mat3,matmul_avx2(mat1, mat2)));
    matrix *mat8=matmul_avx2(mat1, mat2);
    printf("%d\n\n", compare(mat3, matmul_avx2(mat1, mat2)));

    measureTime(matmul_avx2_openmp, mat1, mat2, "avx2_openmp");
    // printMatrix(subtractMatrix(mat3,matmul_avx2_openmp(mat1, mat2)));
    matrix *mat9=matmul_avx2_openmp(mat1, mat2);
    printf("%d\n\n", compare(mat3, matmul_avx2_openmp(mat1, mat2)));

    measureTime(matmul_OPENBLAS, mat1, mat2, "openblas");

    delMat(mat1);
    delMat(mat2);
    delMat(mat3);
    delMat(mat4);
    delMat(mat5);
    delMat(mat6);
    delMat(mat7);
    delMat(mat8);
    delMat(mat9);
    return 0;
}