// C++ program for the implementation of Quick Sort
// GeeksforGeeks, "C++ Program For QuickSort"
// https://www.geeksforgeeks.org/cpp/cpp-program-for-quicksort/
// Se modifico el codigo original para utilizar un pivote central y no perjudicar los tiempos tanto para casos ascendentes como descendentes.
// 

#include <algorithm>
#include <vector>

using namespace std;


// Partitions the subarray vec[low..high] around a pivot.
int partition(vector<int>& vec, int low, int high) {

    // Select the middle element as the pivot
    int middle = low + (high - low) / 2;

    // Move the selected pivot to the end so that
    // the standard Lomuto partition scheme can be used
    swap(vec[middle], vec[high]);

    int pivot = vec[high];

    // Index of the last element smaller than or equal to the pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {

        // If the current element is smaller than
        // or equal to the pivot
        if (vec[j] <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Place the pivot in its correct position
    swap(vec[i + 1], vec[high]);

    // Return the partition index
    return i + 1;
}


// Sorts the subarray in the index range [low..high]
void quickSort(vector<int>& vec, int low, int high) {

    // Base case
    if (low < high) {

        // Partition the array and obtain
        // the final position of the pivot
        int pi = partition(vec, low, high);

        // Sort elements before the pivot
        quickSort(vec, low, pi - 1);

        // Sort elements after the pivot
        quickSort(vec, pi + 1, high);
    }
}


// Interface used by sorting.cpp.
// It follows the same format as the provided sort.cpp:
// receives the array by reference and returns the sorted array.
vector<int> quickSortArray(vector<int>& arr) {

    if (!arr.empty()) {
        quickSort(arr, 0, static_cast<int>(arr.size()) - 1);
    }

    return arr;
}