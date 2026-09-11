# INF-221 - Algoritmos y Complejidad
# Tarea 1 - 2026-2
#
# Generador de graficos de tiempo de ejecucion y memoria.
# Lee las mediciones obtenidas experimentalmente y genera
# los graficos correspondientes en formato PNG.
#
# Implementado para la Tarea 1 utilizando Matplotlib.
# Referencia: https://matplotlib.org/


import csv
from pathlib import Path
from collections import defaultdict
import matplotlib.pyplot as plt

CSV_PATH = Path("../data/measurements/sorting_measurements.csv")
OUTPUT_DIR = Path("../data/plots")

OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

nombres = {
    "merge": "MergeSort",
    "quick": "QuickSort",
    "patience": "PatienceSort",
    "std": "std::sort"
}

tiempos = defaultdict(list)
memorias = defaultdict(list)
timeouts = defaultdict(int)

with open(CSV_PATH, newline="", encoding="utf-8") as archivo:
    lector = csv.DictReader(archivo)

    for fila in lector:
        algoritmo = fila["algorithm"]
        n = int(fila["n"])
        tipo = fila["type"]
        dominio = fila["domain"]
        estado = fila["status"]

        clave = (tipo, dominio, algoritmo, n)

        if estado == "OK":
            tiempo = float(fila["time_seconds"])
            memoria = float(fila["max_rss_kb"])

            tiempos[clave].append(tiempo)
            memorias[clave].append(memoria)

        elif estado == "TIMEOUT":
            timeouts[clave] += 1


tipos = ["aleatorio", "ascendente", "descendente"]
dominios = ["D1", "D7"]
algoritmos = ["merge", "quick", "patience", "std"]



def nota_grafico(tipo, dominio):
    detalle = []
    for (t, d, algoritmo, n), cantidad in sorted(timeouts.items()):
        if t == tipo and d == dominio:
            detalle.append(f"{nombres[algoritmo]} n={n}: {cantidad}/3 TIMEOUT")
    nota = "Media y rango min-max de muestras OK; 3 muestras por caso."
    if detalle:
        nota += "\n" + "; ".join(detalle) + ". Sin tiempo final ni memoria para TIMEOUT."
    plt.figtext(0.5, 0.015, nota, ha="center", fontsize=8)

# --------------------------------------------------
# Graficos de tiempo
# --------------------------------------------------

for tipo in tipos:
    for dominio in dominios:

        plt.figure(figsize=(9, 6))

        for algoritmo in algoritmos:
            puntos = []

            for n in sorted({
                clave[3]
                for clave in tiempos
                if clave[0] == tipo
                and clave[1] == dominio
                and clave[2] == algoritmo
            }):
                valores = tiempos[(tipo, dominio, algoritmo, n)]

                if valores:
                    promedio = sum(valores) / len(valores)
                    puntos.append((n, promedio))

            if puntos:
                x = [p[0] for p in puntos]
                y = [p[1] for p in puntos]

                linea, = plt.plot(
                    x,
                    y,
                    marker="o",
                    label=nombres[algoritmo]
                )
                for n, promedio in puntos:
                    valores = tiempos[(tipo, dominio, algoritmo, n)]
                    plt.errorbar(n, promedio,
                                 yerr=[[promedio - min(valores)], [max(valores) - promedio]],
                                 fmt="none", color=linea.get_color(), capsize=3)


        plt.xscale("log")
        plt.yscale("log")

        plt.xlabel("Tamaño del arreglo (n)")
        plt.ylabel("Tiempo promedio de muestras OK (segundos)")
        plt.title(
            f"Tiempo de ejecución - {tipo.capitalize()} - {dominio}"
        )

        plt.grid(True, which="both", linestyle="--", alpha=0.4)
        plt.legend()
        nota_grafico(tipo, dominio)
        plt.tight_layout(rect=(0, 0.10, 1, 1))

        salida = OUTPUT_DIR / f"tiempo_{tipo}_{dominio}.png"

        plt.savefig(salida, dpi=300)
        plt.close()

        print("Grafico generado:", salida)


# --------------------------------------------------
# Graficos de memoria
# --------------------------------------------------

for tipo in tipos:
    for dominio in dominios:

        plt.figure(figsize=(9, 6))

        for algoritmo in algoritmos:
            puntos = []

            for n in sorted({
                clave[3]
                for clave in memorias
                if clave[0] == tipo
                and clave[1] == dominio
                and clave[2] == algoritmo
            }):
                valores = memorias[(tipo, dominio, algoritmo, n)]

                if valores:
                    promedio = sum(valores) / len(valores)
                    puntos.append((n, promedio))

            if puntos:
                x = [p[0] for p in puntos]
                y = [p[1] for p in puntos]

                linea, = plt.plot(
                    x,
                    y,
                    marker="o",
                    label=nombres[algoritmo]
                )
                for n, promedio in puntos:
                    valores = memorias[(tipo, dominio, algoritmo, n)]
                    plt.errorbar(n, promedio,
                                 yerr=[[promedio - min(valores)], [max(valores) - promedio]],
                                 fmt="none", color=linea.get_color(), capsize=3)


        plt.xscale("log")
        plt.yscale("log")

        plt.xlabel("Tamaño del arreglo (n)")
        plt.ylabel("Memoria máxima promedio de muestras OK (KiB)")
        plt.title(
            f"Uso de memoria - {tipo.capitalize()} - {dominio}"
        )

        plt.grid(True, which="both", linestyle="--", alpha=0.4)
        plt.legend()
        nota_grafico(tipo, dominio)
        plt.tight_layout(rect=(0, 0.10, 1, 1))

        salida = OUTPUT_DIR / f"memoria_{tipo}_{dominio}.png"

        plt.savefig(salida, dpi=300)
        plt.close()

        print("Grafico generado:", salida)


print()
print("Generacion terminada.")
print("Casos con TIMEOUT:", len(timeouts))

for tipo, dominio, algoritmo, n in sorted(timeouts):
    print(
        f"TIMEOUT: {nombres[algoritmo]} | "
        f"n={n} | {tipo} | {dominio}"
    )