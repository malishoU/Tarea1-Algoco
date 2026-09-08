// C++ program for the implementation of Merge Sort
// GeeksforGeeks, "C++ Program For Merge Sort"
// https://www.geeksforgeeks.org/cpp/cpp-program-for-merge-sort/
// Optimizado para el codigo de la tarea 1 (hecho con la misma firma que el codigo sort())



#include <vector>

using namespace std;


// Merges two subarrays of vec.
// First subarray is vec[left..mid]
// Second subarray is vec[mid+1..right]
void merge(vector<int>& vec, int left, int mid, int right) {

    int i, j, k;

    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors
    vector<int> leftVec(n1);
    vector<int> rightVec(n2);

    // Copy data to temporary vectors
    for (i = 0; i < n1; i++) {
        leftVec[i] = vec[left + i];
    }

    for (j = 0; j < n2; j++) {
        rightVec[j] = vec[mid + 1 + j];
    }

    // Merge the temporary vectors back into vec[left..right]
    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {

        if (leftVec[i] <= rightVec[j]) {
            vec[k] = leftVec[i];
            i++;
        }
        else {
            vec[k] = rightVec[j];
            j++;
        }

        k++;
    }

    // Copy the remaining elements of leftVec, if any
    while (i < n1) {
        vec[k] = leftVec[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightVec, if any
    while (j < n2) {
        vec[k] = rightVec[j];
        j++;
        k++;
    }
}


// Sorts the subarray in the index range [left..right]
void mergeSort(vector<int>& vec, int left, int right) {

    if (left < right) {

        // Calculate the midpoint
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        // Merge the sorted halves
        merge(vec, left, mid, right);
    }
}


// Interface used by sorting.cpp.
// It follows the same format as the provided sort.cpp:
// receives the array by reference and returns the sorted array.
vector<int> mergeSortArray(vector<int>& arr) {

    if (!arr.empty()) {
        mergeSort(arr, 0, static_cast<int>(arr.size()) - 1);
    }

    return arr;
}