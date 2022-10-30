#include "header.h"

int main(){
    float data1[]={1.2f,3.4f,5.6f,7.8f,9.1f,1.3f,2.4f,3.6f,4.8f,5.0f,1.4f,2.5f};
    float data2[]={1.2f,3.4f,5.6f,2.4f,3.6f,4.8f,5.0f,9.1f,1.3f,7.8f,1.4f,2.5f};
    float data3[]={8.92f,6.59f,5.16f,3.24f};
    float data4[]={9.56f,1.14f,5.14f,9.18f};
    matrix *mat1=createMatrix(3,4,data1);
    matrix *mat2=createMatrix(4,3,data2);
    matrix *mat3=createMatrix(2,2,data3);
    matrix *mat4=createMatrix(2,2,data4);
    printf("Before delete:\n");
    printMatrix(mat2);
    printf("\n");
    matrix *mat5=copyMatrix(mat2);
    deleteMatrix(mat2);
    printf("After delete:\n");
    // printMatrix(mat2);
    printf("\n");
    printMatrix(addMatrix(mat3,mat4));
    printf("\n");
    printMatrix(subtractMatrix(mat3,mat4));
    printf("\n");
    printMatrix(addScalar(mat3,4));
    printf("\n");
    printMatrix(subtractScalar(mat3,9));
    printf("\n");
    printMatrix(multiplyScalar(mat3,10));
    printf("\n");
    printMatrix(multiplyMatrix(mat1,mat5));
    printf("\n");
    printMatrix(transpose(mat1));
    printf("\n");
}