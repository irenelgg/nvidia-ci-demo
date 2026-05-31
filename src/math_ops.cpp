#include "math_ops.h"
#include <cmath>
#include <stdexcept>

Matrix matmul(const Matrix& A, const Matrix& B) {
    int rows = A.size();
    int inner = A[0].size();
    int cols = B[0].size();

    if (inner != (int)B.size())
        throw std::invalid_argument("incompatible matrix dimensions");

    Matrix C(rows, std::vector<float>(cols, 0.0f));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            for (int k = 0; k < inner; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

Matrix relu(const Matrix& A) {
    Matrix out = A;
    for (auto& row : out)
        for (auto& val : row)
            val = val > 0.0f ? val : 0.0f;
    return out;
}

Matrix softmax(const Matrix& A) {
    Matrix out = A;
    for (auto& row : out) {
        float max_val = *std::max_element(row.begin(), row.end());
        float sum = 0.0f;
        for (auto& val : row) {
            val = std::exp(val - max_val);
            sum += val;
        }
        for (auto& val : row) val /= sum;
    }
    return out;
}
