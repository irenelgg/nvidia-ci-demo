#include <gtest/gtest.h>
#include "math_ops.h"

TEST(MatmulTest, TwoByTwo) {
    Matrix A = {{1, 2}, {3, 4}};
    Matrix B = {{5, 6}, {7, 8}};
    Matrix C = matmul(A, B);
    EXPECT_FLOAT_EQ(C[0][0], 19.0f);
    EXPECT_FLOAT_EQ(C[0][1], 22.0f);
    EXPECT_FLOAT_EQ(C[1][0], 43.0f);
    EXPECT_FLOAT_EQ(C[1][1], 50.0f);
}

TEST(MatmulTest, IncompatibleDimensions) {
    Matrix A = {{1, 2, 3}};
    Matrix B = {{1, 2}};
    EXPECT_THROW(matmul(A, B), std::invalid_argument);
}

TEST(ReluTest, NegativesClamped) {
    Matrix A = {{-1.0f, 2.0f}, {-3.0f, 4.0f}};
    Matrix out = relu(A);
    EXPECT_FLOAT_EQ(out[0][0], 0.0f);
    EXPECT_FLOAT_EQ(out[0][1], 2.0f);
    EXPECT_FLOAT_EQ(out[1][0], 0.0f);
    EXPECT_FLOAT_EQ(out[1][1], 4.0f);
}

TEST(SoftmaxTest, SumsToOne) {
    Matrix A = {{1.0f, 2.0f, 3.0f}};
    Matrix out = softmax(A);
    float sum = out[0][0] + out[0][1] + out[0][2];
    EXPECT_NEAR(sum, 1.0f, 1e-5);
}

TEST(SoftmaxTest, LargestInputHasHighestProb) {
    Matrix A = {{1.0f, 2.0f, 3.0f}};
    Matrix out = softmax(A);
    EXPECT_GT(out[0][2], out[0][1]);
    EXPECT_GT(out[0][1], out[0][0]);
}
