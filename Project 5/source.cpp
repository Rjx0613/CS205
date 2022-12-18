#include "header.h"

int main(){
    int *num=new int[18]{1,2,3 ,4,5,6 ,7,8,9 ,10,11,12 ,13,14,15 ,16,17,18};
    Matrix<int> intmat(3,2,3,1,0,2,2,num);
    intmat.printMat();
    Matrix<int> roi=intmat.ROI();
    roi.printMat();
    intmat*=roi;
    intmat.printMat();
    intmat.setROI(0,0,2,2);
    Matrix<int> roi2=intmat.ROI();
    roi2.printMat();
    roi.printMat();
    return 0;
}