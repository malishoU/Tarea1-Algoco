// C++ program for matrix multiplication using the naive method
// GeeksforGeeks, "C++ Program to Multiply Two Matrices"
// https://www.geeksforgeeks.org/cpp/cpp-program-to-multiply-two-matrices/
//
// Adaptado para la estructura de la Tarea 1.
// Se elimino el main del ejemplo original
// se dejo solamente la funcion de multiplicacion para ser utilizada desde matrix_multiplication.cpp.

#include <vector>

using namespace std;


vector<vector<int>> naiveMultiply(
    vector<vector<int>>& mat1,
    vector<vector<int>>& mat2
) {

    int n = mat1.size();
    int m = mat1[0].size();
    int q = mat2[0].size();

    // Initialize the result matrix with
    // dimensions n x q, filled with 0s
    vector<vector<int>> res(
        n,
        vector<int>(q, 0)
    );

    // Loop through each row of mat1
    for (int i = 0; i < n; i++) {

        // Loop through each column of mat2
        for (int j = 0; j < q; j++) {

            // Compute the dot product of
            // row mat1[i] and column mat2[][j]
            for (int k = 0; k < m; k++) {
                res[i][j] +=
                    mat1[i][k] * mat2[k][j];
            }
        }
    }

    return res;
}