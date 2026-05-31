#pragma once
#include <vector>

using Matrix = std::vector<std::vector<float>>;

Matrix matmul(const Matrix& A, const Matrix& B);
Matrix relu(const Matrix& A);
Matrix softmax(const Matrix& A);
