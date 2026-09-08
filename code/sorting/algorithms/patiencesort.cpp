// Explicacion de patience sort:
// Funciona como el sotitario, se revisa el vector uno por uno, si el valor de la variable es menor que un numero en una pila anterior, se coloca arriba, sino, se hace una nueva pila con ese valor almacenado
// Esto se hace con todo el arreglo (toma n tiempo), despues, se toman los arreglos. 
// Rosettacode.org, Patience sort.
// https://rosettacode.org/wiki/Sorting_algorithms/Patience_sort#C++
// Extraido de seccion C++
//
// Adaptado a estructura de tarea 

#include <algorithm>
#include <cassert>
#include <iterator>
#include <stack>
#include <vector>

using namespace std;


// Comparator used to order piles according to their top element
template <class E>
struct pile_less {

    bool operator()(const stack<E>& pile1,
                    const stack<E>& pile2) const {

        return pile1.top() < pile2.top();
    }
};


// Comparator used to build a min-heap of piles
template <class E>
struct pile_greater {

    bool operator()(const stack<E>& pile1,
                    const stack<E>& pile2) const {

        return pile1.top() > pile2.top();
    }
};


// Patience Sort implementation
template <class Iterator>
void patience_sort(Iterator first, Iterator last) {

    typedef typename iterator_traits<Iterator>::value_type E;
    typedef stack<E> Pile;

    vector<Pile> piles;


    // Sort elements into piles
    for (Iterator it = first; it != last; it++) {

        E& x = *it;

        Pile newPile;
        newPile.push(x);

        typename vector<Pile>::iterator i =
            lower_bound(
                piles.begin(),
                piles.end(),
                newPile,
                pile_less<E>()
            );

        if (i != piles.end()) {
            i->push(x);
        }
        else {
            piles.push_back(newPile);
        }
    }


    // Priority queue behavior allows us to merge
    // the piles efficiently.
    //
    // A greater-than comparator is used so that
    // the smallest top element is extracted first.
    make_heap(
        piles.begin(),
        piles.end(),
        pile_greater<E>()
    );


    for (Iterator it = first; it != last; it++) {

        pop_heap(
            piles.begin(),
            piles.end(),
            pile_greater<E>()
        );

        Pile& smallPile = piles.back();

        *it = smallPile.top();
        smallPile.pop();


        if (smallPile.empty()) {

            piles.pop_back();
        }
        else {

            push_heap(
                piles.begin(),
                piles.end(),
                pile_greater<E>()
            );
        }
    }


    assert(piles.empty());
}


// Interface used by sorting.cpp.
// It follows the same format as the other sorting algorithms:
// receives the array by reference and returns the sorted array.
vector<int> patienceSortArray(vector<int>& arr) {

    patience_sort(arr.begin(), arr.end());

    return arr;
}