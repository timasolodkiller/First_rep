#include <iostream>
#include <vector>
class Matrix {
public:
    Matrix(int rows, int cols);
    Matrix(const std::vector<std::vector<double>>& data);
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator/(const Matrix& other) const;
    Matrix operator+(double scalar) const;
    Matrix operator-(double scalar) const;
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
    Matrix& operator*=(const Matrix&);
    Matrix& operator*=(double);
    Matrix& operator/=(double);
    bool operator==(const Matrix& other) const;
    Matrix get_inverse() const;
    double get_determinant() const;
    void print() const;

private:
    int rows_;
    int cols_;
    std::vector<std::vector<double>> data_;
};

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows, std::vector<double>(cols, 0.0)) {}

Matrix::Matrix(const std::vector<std::vector<double>>& data) : rows_(data.size()), cols_(data[0].size()), data_(data) {}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrices must have the same dimensions to be added");
    }

    Matrix result(rows_, cols_);

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            result.data_[i][j] = data_[i][j] + other.data_[i][j];
        }
    }

    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrices must have the same dimensions to be subtracted");
    }

    Matrix result(rows_, cols_);

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            result.data_[i][j] = data_[i][j] - other.data_[i][j];
        }
    }

    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Number of columns in first matrix must be equal to number of rows in second matrix");
    }

    Matrix result(rows_, other.cols_);

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
            for (int k = 0; k < cols_; ++k) {
                result.data_[i][j] += data_[i][k] * other.data_[k][j];
            }
        }
    }

    return result;
}

Matrix Matrix::operator/(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrices must have the same dimensions to be divided");
    }

    Matrix result(rows_, cols_);

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            result.data_[i][j] = data_[i][j] / other.data_[i][j];
        }
    }

    return result;
}

Matrix Matrix::operator+(double scalar) const {
   
Matrix result(rows_, cols_);

for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
        result.data_[i][j] = data_[i][j] + scalar;
    }
}

return result;
} 

Matrix Matrix::operator-(double scalar) const {
Matrix result(rows_, cols_);
for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
        result.data_[i][j] = data_[i][j] - scalar;
    }
}

return result;
}
Matrix Matrix::operator*(double scalar) const {
Matrix result(rows_, cols_);


for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
        result.data_[i][j] = data_[i][j] * scalar;
    }
}

return result;
}

Matrix Matrix::operator/(double scalar) const {
if (scalar == 0) {
throw std::invalid_argument("Cannot divide by zero");
}

Matrix result(rows_, cols_);

for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
        result.data_[i][j] = data_[i][j] / scalar;
    }
}

return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrices must have the same dimensions");
    }

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            data_[i][j] += other.data_[i][j];
        }
    }

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrices must have the same dimensions");
    }

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            data_[i][j] -= other.data_[i][j];
        }
    }

    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix dimensions are incompatible");
    }

    Matrix result(rows_, other.cols_);

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
            for (int k = 0; k < cols_; ++k) {
                result.data_[i][j] += data_[i][k] * other.data_[k][j];
            }
        }
    }

    *this = result;
    return *this;
}

void Matrix::print() const {
for (int i = 0; i < rows_; ++i) {
for (int j = 0; j < cols_; ++j) {
std::cout << data_[i][j] << " ";
}
std::cout << std::endl;
}
}


int main() {
Matrix m1({{1, 2}, {3, 4}});
Matrix m2({{5, 6}, {7, 8}});

std::cout << "m1:\n";
m1.print();

std::cout << "m2:\n";
m2.print();

Matrix m3 = m1 + m2;
std::cout << "m1 + m2:\n";
m3.print();

Matrix m4 = m1 - m2;
std::cout << "m1 - m2:\n";
m4.print();

Matrix m5 = m1 * m2;
std::cout << "m1 * m2:\n";
m5.print();

Matrix m6 = m1 / m2;
std::cout << "m1 / m2:\n";
m6.print();

Matrix m7 = m1 + 2.0;
std::cout << "m1 + 2.0:\n";
m7.print();

Matrix m8 = m1 - 2.0;
std::cout << "m1 - 2.0:\n";
m8.print();

Matrix m9 = m1 * 2.0;
std::cout << "m1 * 2.0:\n";
m9.print();

Matrix m10 = m1 / 2.0;
std::cout << "m1 / 2.0:\n";
m10.print();

return 0;
}
