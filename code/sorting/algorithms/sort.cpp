
// INF-221 - Algoritmos y Complejidad
// Tarea 1 - 2026-2
//
// Implementacion del algoritmo de ordenamiento de la libreria
// estandar de C++ mediante std::sort.
// Referencia: documentacion de la biblioteca estandar de C++.
// https://en.cppreference.com/w/cpp/algorithm/sort

#include <algorithm>
#include <vector>

std::vector<int> sortArray(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());  // std::sort de la STL
    return arr;
}
