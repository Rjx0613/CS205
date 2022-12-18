#include <iostream>
#include <memory>
#include <omp.h>
#include <malloc.h>
#include <immintrin.h>
#include <cstring>
using namespace std;

template <typename T>
class Matrix
{
private:
    size_t row;
    size_t col;
    int channels;
    size_t ROI_R;
    size_t ROI_C;
    size_t ROI_LengthR;
    size_t ROI_LengthC;
    size_t access;
    // shared_ptr<T[]> data;
    T *data;
    int count;

public:
    size_t getRow() { return this->row; }

    size_t getCol() { return this->col; }

    int getChannels() { return this->channels; }

    Matrix(size_t row = 0, size_t col = 0, int channels = 0, size_t ROI_R = 0, size_t ROI_C = 0, size_t ROI_LengthR = 0, size_t ROI_LengthC = 0, T *data = nullptr);

    Matrix(const Matrix &mat);

    ~Matrix();

    Matrix operator+(const Matrix<T> &mat) const;

    Matrix operator+(const T) const;

    Matrix &operator+=(const Matrix<T> &mat);

    Matrix &operator+=(T num);

    friend Matrix<T> operator+(T num, const Matrix<T> &mat) // 友元函数为非成员函数，放在类中，可以调用private变量，不能通过成员运算符::来调用，必须在类中实现
    {
        return mat + num;
    }

    Matrix operator-(const Matrix<T> &mat) const;

    Matrix operator-(T num) const;

    Matrix &operator-=(const Matrix<T> &mat);

    Matrix &operator-=(T num);

    friend Matrix<T> operator-(T num, const Matrix<T> &mat) // 友元函数为什么不能加const？
    {
        return mat - num;
    }

    Matrix operator*(const Matrix<T> &mat) const;

    Matrix operator*(T num) const;

    Matrix &operator*=(const Matrix<T> &mat);

    Matrix &operator*=(T num);

    friend Matrix<T> operator*(T num, const Matrix<T> &mat)
    {
        return mat * num;
    }

    Matrix &operator=(const Matrix<T> &mat); // 采用引用返回，在连续赋值的情况下不会调用多次copyConstructor以及析构函数，提升效率

    bool operator==(const Matrix &mat);

    Matrix ROI() const;

    void setAccess(size_t access);

    void setROI(size_t ROI_R, size_t ROI_C, size_t ROI_LengthR, size_t ROI_LengthC);

    void ChangeSize(size_t ROI_LengthR, size_t ROI_LengthC);

    void ChangePosition(size_t ROI_R, size_t ROI_C);

    void printMat() const;
};

template <typename T>
Matrix<T>::Matrix(size_t row, size_t col, int channels, size_t ROI_R, size_t ROI_C, size_t ROI_LengthR, size_t ROI_LengthC, T *data)
{
    if (row < 0 || col < 0 || ROI_R < 0 || ROI_R > row || ROI_C < 0 || ROI_C > col)
    {
        cerr << "Initialize error: parameter out of bounds" << endl;
    }
    if (channels < 1 || channels > 3)
    {
        cerr << "Initialize error: channels too less or too many" << endl;
    }
    if (ROI_R + ROI_LengthR > row || ROI_C + ROI_LengthC > col)
    {
        cerr << "Initialize error: ROI out of bounds" << endl;
    }
    this->row = row;
    this->col = col;
    this->channels = channels;
    this->ROI_R = ROI_R;
    this->ROI_C = ROI_C;
    this->ROI_LengthR = ROI_LengthR;
    this->ROI_LengthC = ROI_LengthC;
    this->access = channels * col;
    if (data == nullptr)
    { // 若为空指针则将矩阵全部初始化为零
        this->data = new T[row * col * channels];
        count++;
    }
    else
    {
        this->data = data;
        count++;
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix &mat)
{
    this->row = mat.row;
    this->col = mat.col;
    this->channels = mat.channels;
    this->ROI_R = mat.ROI_R;
    this->ROI_C = mat.ROI_C;
    this->ROI_LengthR = mat.ROI_LengthR;
    this->ROI_LengthC = mat.ROI_LengthC;
    this->data = mat.data;
    count++;
}

template <typename T>
Matrix<T>::~Matrix()
{
    this->row = 0;
    this->col = 0;
    this->channels = 0;
    this->ROI_R = 0;
    this->ROI_C = 0;
    this->ROI_LengthR = 0;
    this->ROI_LengthC = 0;
    this->access = 0;
    if (count == 0)
    {
        delete[] data;
        this->data = nullptr;
    }
    else
    {
        count--;
    }
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix &mat) const
{
    if (this->row != mat.row || this->col != mat.col || this->channels != mat.channels)
    {
        cerr << "Add error: size doesn't match" << endl;
    }
    Matrix<T> result(this->row, this->col, this->channels, this->ROI_R, this->ROI_C, this->ROI_LengthR, this->ROI_LengthC, nullptr);
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                result.data[k + i * this->access + j * channels] = this->data[k + i * this->access + j * channels] + mat.data[k + i * this->access + j * channels];
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(T num) const
{
    Matrix<T> result(this->row, this->col, this->channels, this->ROI_R, this->ROI_C, this->ROI_LengthR, this->ROI_LengthC, nullptr);
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                result.data[k + i * this->access + j * channels] = this->data[k + i * this->access + j * channels] + num;
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix &mat)
{
    if (this->row != mat.row || this->col != mat.col || this->channels != mat.channels)
    {
        cerr << "+= error: size doesn't match" << endl;
    }
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                this->data[k + i * this->access + j * channels] += mat.data[k + i * this->access + j * channels];
            }
        }
    }
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(T num)
{
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                this->data[k + i * this->access + j * channels] += num;
            }
        }
    }
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix &mat) const
{
    if (this->row != mat.row || this->col != mat.col || this->channels != mat.channels)
    {
        cerr << "Substract error: size doesn't match" << endl;
    }
    Matrix<T> result(this->row, this->col, this->channels, this->ROI_R, this->ROI_C, this->ROI_LengthR, this->ROI_LengthC, nullptr);
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                result.data[k + i * this->access + j * channels] = this->data[k + i * this->access + j * channels] - mat.data[k + i * this->access + j * channels];
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(T num) const
{
    Matrix<T> result(this->row, this->col, this->channels, this->ROI_R, this->ROI_C, this->ROI_LengthR, this->ROI_LengthC, nullptr);
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                result.data[k + i * this->access + j * channels] = this->data[k + i * this->access + j * channels] - num;
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix &mat)
{
    if (this->row != mat.row || this->col != mat.col || this->channels != mat.channels)
    {
        cerr << "-= error: size doesn't match" << endl;
    }
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                this->data[k + i * this->access + j * channels] -= mat.data[k + i * this->access + j * channels];
            }
        }
    }
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(T num)
{
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                this->data[k + i * this->access + j * channels] -= num;
            }
        }
    }
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &mat) const
{
    if (this->col != mat.row || this->channels != mat.channels)
    {
        cerr << "Multiply error: size doesn't match" << endl;
    }
    Matrix<T> result(this->row, mat.col, this->channels, 0, 0, this->row, mat.col, nullptr);
    size_t i, j, k;
    int ch;
#pragma omp parallel for
    for (ch = 0; ch < this->channels; ch++)
    {
        for (i = 0; i < this->row; i++)
        {
            for (k = 0; k < this->col; k++)
            {
                T temp = this->data[ch + i * this->access + k * channels];
                for (j = 0; j < mat.col; j++)
                {
                    result.data[ch + i * mat.access + j * channels] += temp * mat.data[ch + k * mat.access + j * channels];
                }
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(T num) const
{
    Matrix<T> result(this->row, this->col, this->channels, this->ROI_R, this->ROI_C, this->ROI_LengthR, this->ROI_LengthC, nullptr);
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                result.data[k + i * this->access + j * channels] = this->data[k + i * this->access + j * channels] * num;
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const Matrix &mat)
{
    if (this->col != mat.row || this->channels != mat.channels)
    {
        cerr << "*= error: size doesn't match" << endl;
    }

    T *result = new T[this->row * mat.col * this->channels];
    size_t i, j, k;
    int ch;
#pragma omp parallel for
    for (ch = 0; ch < this->channels; ch++)
    {
        for (i = 0; i < this->row; i++)
        {
            for (k = 0; k < this->col; k++)
            {
                T temp = this->data[ch + i * this->access + k * channels];
                for (j = 0; j < mat.col; j++)
                {
                    result[ch + i * mat.access + j * channels] += temp * mat.data[ch + k * mat.access + j * channels];
                }
            }
        }
    }

    this->col = mat.col;
    delete[] this->data;
    setROI(0, 0, this->row, mat.col);
    this->data = result;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(T num)
{
    for (int k = 0; k < channels; k++)
    {
        for (size_t i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                this->data[k + i * this->access + j * channels] *= num;
            }
        }
    }
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix &mat)
{
    if (this != &mat)
    {
        this->row = mat.row;
        this->col = mat.col;
        this->channels = mat.channels;
        this->ROI_R = mat.ROI_R;
        this->ROI_C = mat.ROI_C;
        this->ROI_LengthR = mat.ROI_LengthR;
        this->ROI_LengthC = mat.ROI_LengthC;
        this->data = mat.data;
        this->count++;
        this->access = mat.access;
    }
    return *this;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix &mat)
{
    if (this->row == mat.row && this->col == mat.col && this->channels == mat.channels && this->ROI_R == mat.ROI_R && this->ROI_C == mat.ROI_C && this->ROI_LengthR == mat.ROI_LengthR && this->ROI_LengthC == mat.ROI_LengthC)
    {
        for (int ch = 0; ch < this->channels; ch++)
        {
            for (size_t i = 0; i < this->row; i++)
            {
                for (size_t j = 0; j < this->col; j++)
                {
                    if (this->data[ch * channels + i * this->col + j] != mat.data[ch * channels + i * mat.col + j])
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

template <typename T>
Matrix<T> Matrix<T>::ROI() const
{
    if (ROI_R < 0 || ROI_R > row || ROI_C < 0 || ROI_C > col)
    {
        cerr << "GetROI error: parameter out of bounds" << endl;
    }
    if (ROI_R + ROI_LengthR > row || ROI_C + ROI_LengthC > col)
    {
        cerr << "GetROI error: ROI out of bounds" << endl;
    }
    Matrix<T> result(this->ROI_LengthR, this->ROI_LengthC, this->channels, 0, 0, this->ROI_LengthR, this->ROI_LengthC, nullptr);
    // size_t dataSize = this->row * this->col;
    // size_t ROISize = ROI_LengthR * ROI_LengthC;
    // for (int i = 0; i < channels; i++)//hard copy实现
    // {
    //     for (size_t j = ROI_R; j < ROI_LengthR; j++)
    //     {
    //         for (size_t k = ROI_C; k < ROI_LengthC; k++)
    //         {
    //             result.data[i * ROISize + j * ROI_LengthC + k] = this->data[i * dataSize + j * this->col + k];
    //         }
    //     }
    // }
    result.data = this->data + (ROI_R * this->col + ROI_C) * this->channels;
    result.setAccess(this->channels * this->col);
    return result;
}

template <typename T>
void Matrix<T>::setAccess(size_t access)
{
    this->access = access;
}

template <typename T>
void Matrix<T>::setROI(size_t ROI_R, size_t ROI_C, size_t ROI_LengthR, size_t ROI_LengthC)
{
    if (ROI_R < 0 || ROI_R > this->row || ROI_C < 0 || ROI_C > this->col)
    {
        cerr << "SetROI error: parameter out of bounds" << endl;
    }
    if (ROI_R + ROI_LengthR > this->row || ROI_C + ROI_LengthC > this->col)
    {
        cerr << "SetROI error: ROI out of bounds" << endl;
    }
    this->ROI_R = ROI_R;
    this->ROI_C = ROI_C;
    this->ROI_LengthR = ROI_LengthR;
    this->ROI_LengthC = ROI_LengthC;
}

template <typename T>
void Matrix<T>::ChangeSize(size_t ROI_LengthR, size_t ROI_LengthC)
{
    if (ROI_R + ROI_LengthR > this->row || ROI_C + ROI_LengthC > this->col)
    {
        cerr << "ChangeSize error: ROI out of bounds" << endl;
    }
    this->ROI_LengthR = ROI_LengthR;
    this->ROI_LengthC = ROI_LengthC;
}

template <typename T>
void Matrix<T>::ChangePosition(size_t ROI_R, size_t ROI_C)
{
    if (ROI_R < 0 || ROI_R > this->row || ROI_C < 0 || ROI_C > this->col)
    {
        cerr << "ChangePosition error: parameter out of bounds" << endl;
    }
    this->ROI_R = ROI_R;
    this->ROI_C = ROI_C;
}

template <typename T>
void Matrix<T>::printMat() const
{
    printf("matrix_%p:\n", this);
    size_t dataSize = this->row * this->col;
    for (int i = 1; i <= this->channels; i++)
    {
        cout << "Channel " << i << ":\n";
        for (size_t j = 0; j < this->row; j++)
        {
            for (size_t k = 0; k < this->col; k++)
            {
                cout << this->data[i - 1 + j * this->access + k * channels] << " ";
            }
            cout << endl;
        }
    }
    cout << endl;
}