# Documentación

## Entrega

La entrega se realiza vía **aula.usm.cl** en formato `.zip`.

## Multiplicación de matrices

En esta parte se implementaron los algoritmos Naive y Strassen para comparar sus tiempos de ejecución y uso de memoria con distintos tipos y tamaños de matrices.

### Programa principal

El programa principal es matrix_multiplication.cpp y recibe como argumentos el algoritmo que se quiere utilizar y los dos archivos correspondientes a las matrices de entrada.

Los algoritmos disponibles son:

- naive: multiplicación tradicional de matrices
- strassen: algoritmo de Strassen

Por ejemplo, para ejecutar Naive sobre un par de matrices:

./matrix_multiplication naive data/matrix_input/16_densa_D0_a_1.txt data/matrix_input/16_densa_D0_a_2.txt

Para compilar el programa hay que ubicarse en la carpeta matrix_multiplication y ejecutar:

make

Para realizar todas las pruebas:

make run

Las pruebas se realizan con matrices de tamaño 16, 64, 256 y 1024. Para cada tamaño se utilizan matrices dispersas, diagonales y densas, con los dominios D0 y D10 y tres muestras distintas (a, b y c).

Las matrices resultantes quedan guardadas en data/matrix_output/ y los resultados de las mediciones quedan en:

data/measurements/matrix_measurements.csv

En este archivo se guarda el algoritmo utilizado, el tamaño y tipo de la matriz, el dominio, la muestra, el tiempo de ejecución, la memoria utilizada y el estado de la ejecución.

Para evitar que una prueba quede ejecutándose por demasiado tiempo, se utiliza un límite de 60 segundos. Si se supera este tiempo, la prueba queda registrada como TIMEOUT.

### Scripts

Los scripts utilizados se encuentran en la carpeta matrix_multiplication/scripts/.

matrix_generator.py se utiliza para generar las matrices de entrada utilizadas en las pruebas. Estas quedan almacenadas en data/matrix_input/.

plot_generator.py utiliza las mediciones obtenidas para generar los gráficos de tiempo y memoria. Los gráficos quedan guardados en data/plots/.

Para generar las matrices hay que entrar a la carpeta scripts y ejecutar:

python matrix_generator.py

Para generar los gráficos:

python plot_generator.py

Para ejecutar estos scripts se necesita Python 3, numpy y matplotlib.


## Ordenamiento de arreglo unidimensional

En esta parte se implementaron los algoritmos MergeSort, QuickSort, PatienceSort y std::sort para comparar sus tiempos de ejecución y uso de memoria con distintos tipos y tamaños de arreglos.

### Programa principal

El programa principal es sorting.cpp y recibe como argumentos el algoritmo que se quiere utilizar y el archivo de entrada.

Los algoritmos disponibles son:

- merge: MergeSort
- quick: QuickSort
- patience: PatienceSort
- std: std::sort

Por ejemplo, para ejecutar MergeSort sobre uno de los arreglos:

./sorting merge data/array_input/1000_aleatorio_D1_a.txt

Para compilar el programa hay que ubicarse en la carpeta sorting y ejecutar:

make

Para realizar todas las pruebas:

make run

Las pruebas se realizan con arreglos de tamaño 10, 1000, 100000 y 10000000. Para cada tamaño se utilizan arreglos aleatorios, ascendentes y descendentes, con los dominios D1 y D7 y tres muestras distintas (a, b y c).

Los arreglos ordenados quedan guardados en data/array_output/ y los resultados de las mediciones quedan en:

data/measurements/sorting_measurements.csv

En este archivo se guarda el algoritmo utilizado, el tamaño y tipo del arreglo, el dominio, la muestra, el tiempo de ejecución, la memoria utilizada y el estado de la ejecución.

Para evitar que una prueba quede ejecutándose por demasiado tiempo, se utiliza un límite de 30 segundos. Si se supera este tiempo, la prueba queda registrada como TIMEOUT.

### Scripts

Los scripts utilizados se encuentran en la carpeta sorting/scripts/.

array_generator.py se utiliza para generar los arreglos de entrada utilizados en las pruebas. Estos quedan almacenados en data/array_input/.

plot_generator.py utiliza las mediciones obtenidas para generar los gráficos de tiempo y memoria. Los gráficos quedan guardados en data/plots/.

Para generar los arreglos hay que entrar a la carpeta scripts y ejecutar:

python array_generator.py

Para generar los gráficos:

python plot_generator.py

Para ejecutar estos scripts se necesita Python 3, numpy y matplotlib.