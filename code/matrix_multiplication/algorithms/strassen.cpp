// C++ program for the implementation of Strassen's Matrix Multiplication
// GeeksforGeeks, "Strassen's Matrix Multiplication Algorithm"
// https://www.geeksforgeeks.org/strassens-matrix-multiplication-algorithm-implementation/
//
// Adaptado para la estructura de la Tarea 1.
// Se elimino el main del ejemplo original
// Se modifico el nombre de la funcion principal para utilizarla desde matrix_multiplication.cpp.

#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


// Return the next power of two greater than or equal to n
int nextPowerOfTwo(int n) {
    return pow(2, ceil(log2(n)));
}


// Resize a matrix to newR x newC and
// fill extra space with zeros
vector<vector<int>> resizeMatrix(
    vector<vector<int>>& mat,
    int newR,
    int newC
) {

    vector<vector<int>> resized(
        newR,
        vector<int>(newC, 0)
    );

    for (int i = 0; i < mat.size(); ++i)
        for (int j = 0; j < mat[0].size(); ++j)
            resized[i][j] = mat[i][j];

    return resized;
}


// Perform matrix addition or subtraction
// of size x size matrices
// sign = 1 for addition, -1 for subtraction
vector<vector<int>> add(
    vector<vector<int>> a,
    vector<vector<int>> b,
    int size,
    int sign = 1
) {

    vector<vector<int>> res(
        size,
        vector<int>(size)
    );

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res[i][j] = a[i][j] + sign * b[i][j];

    return res;
}


// Recursive implementation of Strassen's
// matrix multiplication
// Assumes both matrices are size x size
// and size is a power of 2
vector<vector<int>> strassen(
    vector<vector<int>> mat1,
    vector<vector<int>> mat2
) {

    int n = mat1.size();

    // Base case: 1 x 1 matrix multiplication
    vector<vector<int>> res(
        n,
        vector<int>(n, 0)
    );

    if (n == 1) {
        res[0][0] = mat1[0][0] * mat2[0][0];
        return res;
    }

    // Split matrices into 4 submatrices
    int newSize = n / 2;

    vector<vector<int>> a11(newSize, vector<int>(newSize));
    vector<vector<int>> a12(newSize, vector<int>(newSize));
    vector<vector<int>> a21(newSize, vector<int>(newSize));
    vector<vector<int>> a22(newSize, vector<int>(newSize));

    vector<vector<int>> b11(newSize, vector<int>(newSize));
    vector<vector<int>> b12(newSize, vector<int>(newSize));
    vector<vector<int>> b21(newSize, vector<int>(newSize));
    vector<vector<int>> b22(newSize, vector<int>(newSize));

    // Fill the submatrices
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            a11[i][j] = mat1[i][j];
            a12[i][j] = mat1[i][j + newSize];
            a21[i][j] = mat1[i + newSize][j];
            a22[i][j] = mat1[i + newSize][j + newSize];

            b11[i][j] = mat2[i][j];
            b12[i][j] = mat2[i][j + newSize];
            b21[i][j] = mat2[i + newSize][j];
            b22[i][j] = mat2[i + newSize][j + newSize];
        }

    // Compute the 7 products using Strassen's formulas
    auto m1 = strassen(
        add(a11, a22, newSize),
        add(b11, b22, newSize)
    );

    auto m2 = strassen(
        add(a21, a22, newSize),
        b11
    );

    auto m3 = strassen(
        a11,
        add(b12, b22, newSize, -1)
    );

    auto m4 = strassen(
        a22,
        add(b21, b11, newSize, -1)
    );

    auto m5 = strassen(
        add(a11, a12, newSize),
        b22
    );

    auto m6 = strassen(
        add(a21, a11, newSize, -1),
        add(b11, b12, newSize)
    );

    auto m7 = strassen(
        add(a12, a22, newSize, -1),
        add(b21, b22, newSize)
    );

    // Calculate result quadrants from m1...m7
    auto c11 = add(
        add(m1, m4, newSize),
        add(m7, m5, newSize, -1),
        newSize
    );

    auto c12 = add(m3, m5, newSize);

    auto c21 = add(m2, m4, newSize);

    auto c22 = add(
        add(m1, m3, newSize),
        add(m6, m2, newSize, -1),
        newSize
    );

    // Combine result quadrants into final matrix
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            res[i][j] = c11[i][j];
            res[i][j + newSize] = c12[i][j];
            res[i + newSize][j] = c21[i][j];
            res[i + newSize][j + newSize] = c22[i][j];
        }

    return res;
}


// Multiply mat1 and mat2 using Strassen's method
vector<vector<int>> strassenMultiply(
    vector<vector<int>>& mat1,
    vector<vector<int>>& mat2
) {

    int n = mat1.size();
    int m = mat1[0].size();
    int q = mat2[0].size();

    int size = nextPowerOfTwo(
        max(n, max(m, q))
    );

    // Pad both matrices to size x size with zeros
    vector<vector<int>> aPad =
        resizeMatrix(mat1, size, size);

    vector<vector<int>> bPad =
        resizeMatrix(mat2, size, size);

    // Perform Strassen multiplication
    vector<vector<int>> cPad =
        strassen(aPad, bPad);

    // Extract the valid result
    vector<vector<int>> C(
        n,
        vector<int>(q)
    );

    for (int i = 0; i < n; i++)
        for (int j = 0; j < q; j++)
            C[i][j] = cPad[i][j];

    return C;
}