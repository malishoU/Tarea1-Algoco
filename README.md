# TAREA 1 INF221 2026-2

## Descripción

Este repositorio contiene el código fuente, los datos experimentales, los resultados y el mini-informe todo correspondiente a la Tarea 1 de Algoritmos y Complejidad.

La tarea estudia experimentalmente algoritmos de ordenamiento de arreglos y de multiplicación de matrices, comparando sus tiempos de ejecución y uso de memoria.

## Nota sobre los archivos de datos

Según la aclaración del ayudante, el ZIP final debe entregarse sin los archivos de input y output. Los scripts generan los datos de prueba y crean las carpetas necesarias. En la carpeta de trabajo se pueden conservar los datos y resultados.

El repositorio de respaldo se encuentra en:

https://github.com/malishoU/Tarea1-Algoco

## Estructura del repositorio

```text
├── assignment_statement
├── code
│   ├── matrix_multiplication
│   └── sorting
├── report
└── README.md
```

### `assignment_statement`

Contiene el enunciado de la tarea y sus archivos fuente.

### `code`

Contiene las implementaciones, programas de medición, scripts de generación de datos y gráficos, además de los resultados experimentales.

Los algoritmos evaluados son:

- Ordenamiento: MergeSort, QuickSort, PatienceSort y `std::sort`.
- Multiplicación de matrices: Naive y Strassen.

Mas adelante se presentan las instrucciones específicas para compilar y ejecutar los programas.

### `report`

Contiene el mini-informe desarrollado en LaTeX y el archivo `report.pdf`.

Para compilar el informe:

```bash
cd report
pdflatex report.tex
biber report
pdflatex report.tex
pdflatex report.tex
```

El documento generado corresponde a `report/report.pdf`.

## Requisitos

Para ejecutar los experimentos y generar los gráficos se requiere:

- g++ con soporte para C++17
- make
- Python 3
- NumPy
- Matplotlib
- LaTeX para compilar el informe

Para compilar el informe se requiere una instalación de LaTeX con los paquetes utilizados por la plantilla.

## Ejecución

### Requisitos

Para ejecutar el proyecto se requiere:

- g++ con soporte para C++17
- make
- Python 3
- NumPy
- Matplotlib
- Una distribución de LaTeX para compilar el informe

### Ordenamiento

Para generar los datos de entrada, compilar y ejecutar los experimentos:

```bash
cd code/sorting
make generate
make
make run
make plots
```

Los resultados de las mediciones quedan almacenados en:

```text
code/sorting/data/measurements/
```

y los gráficos generados en:

```text
code/sorting/data/plots/
```

### Multiplicación de matrices

Para generar los datos de entrada, compilar y ejecutar los experimentos:

```bash
cd code/matrix_multiplication
make generate
make
make run
make plots
```

Los resultados de las mediciones quedan almacenados en:

```text
code/matrix_multiplication/data/measurements/
```

y los gráficos generados en:

```text
code/matrix_multiplication/data/plots/
```

### Informe

Una vez generados los resultados y gráficos, el informe puede compilarse mediante:

```bash
cd report
pdflatex report.tex
biber report
pdflatex report.tex
pdflatex report.tex
```

El documento generado corresponde a:

```text
report/report.pdf
```

Para obtener información más detallada sobre la ejecución de los programas y sus argumentos, revisar `code/README.md`.


