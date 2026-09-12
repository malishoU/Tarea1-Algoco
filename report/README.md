# Informe de Tarea 1

Esta carpeta contiene el informe de los experimentos de ordenamiento y multiplicación de matrices, elaborado con la plantilla de la asignatura.

- `author.tex` contiene los datos del autor.
- `sections/introduccion.tex`, `sections/experiment_intro.tex`, `sections/experiment_results.tex` y `sections/conclusions.tex` contienen el cuerpo del informe.
- `references.bib` contiene la bibliografía.
- `report.tex` y `preamble.tex` corresponden a la estructura y configuración de la plantilla.
- `report.pdf` es el informe compilado.

Los gráficos utilizados se encuentran en las carpetas `data/plots` de ambos programas, dentro de `code`.

## Compilación

Desde la raiz del proyecto (con LaTeX y Biber instalados):

```bash
cd report
pdflatex report.tex
biber report
pdflatex report.tex
pdflatex report.tex
```

El resultado queda en `report/report.pdf`. Para compilar el informe con los graficos incluidos no es necesario repetir los experimentos, solo compilar los graficos.

## Entrega

La entrega se realiza via **aula.usm.cl** en formato `.zip`, incluyendo el informe y el proyecto.
