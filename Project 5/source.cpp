#include "header.h"

int main(){
    int *num=new int[18]{1,2,3 ,4,5,6 ,7,8,9 ,10,11,12 ,13,14,15 ,16,17,18};
    // float *num2=new float[24]{1.2,3.4,5.6,7.8,9.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,1.3,2.4,3.5,4.6,5.7,6.8,7.9,8.0,9.1,10.4};
    Matrix<int> intmat(3,2,3,1,0,2,2,num);
    // Matrix<float> floatmat(2,4,3,0,0,2,2,num2);
    cout<<"IntMatrix:"<<endl;
    intmat.printMat();
    Matrix<int> roi=intmat.ROI();
    cout<<"ROI"<<endl;
    roi.printMat();
    intmat*=roi;
    cout<<"IntMatrix*=roi:"<<endl;
    intmat.printMat();
    intmat.setROI(0,0,2,2);
    cout<<"ChangeROI:"<<endl;
    intmat.ROI().printMat();
    cout<<"IntMatrix+2"<<endl;
    (intmat+2).printMat();
    return 0;
}