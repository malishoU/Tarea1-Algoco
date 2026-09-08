#IMPORTANTE: mi implementacion no utiliza este archivo, solo correr mediante Makefile para
#compilar el archivo y make run para ejecutar.

#!/usr/bin/env bash

# Automatización de mediciones para los algoritmos de ordenamiento.
#
# Uso:
#   ./scripts/run_measurements.sh [tamaño|all] [repeticiones]
#
# Ejemplos:
#   ./scripts/run_measurements.sh 10 1
#   ./scripts/run_measurements.sh 1000 3
#   ./scripts/run_measurements.sh all 3
#
# El tiempo corresponde exclusivamente al algoritmo de ordenamiento,
# porque sorting.cpp inicia y detiene el reloj alrededor del algoritmo.
#
# La memoria corresponde al máximo de memoria residente del proceso
# completo, informado por /usr/bin/time en KiB.

set -u


# --------------------------------------------------
# Configuración
# --------------------------------------------------

SIZE_FILTER="${1:-10}"
REPETITIONS="${2:-1}"

EXECUTABLE="./sorting"
INPUT_DIRECTORY="data/array_input"
MEASUREMENTS_DIRECTORY="data/measurements"
CSV_FILE="${MEASUREMENTS_DIRECTORY}/sorting_measurements.csv"

ALGORITHMS=(
    "merge"
    "quick"
    "patience"
    "std"
)


# --------------------------------------------------
# Validaciones
# --------------------------------------------------

if [[ ! -x "$EXECUTABLE" ]]; then
    echo "Error: no se encontró el ejecutable $EXECUTABLE."
    echo "Ejecuta primero: make"
    exit 1
fi


if [[ ! -d "$INPUT_DIRECTORY" ]]; then
    echo "Error: no existe el directorio $INPUT_DIRECTORY."
    exit 1
fi


if [[ ! -x "/usr/bin/time" ]]; then
    echo "Error: no se encontró /usr/bin/time."
    echo "Puedes instalarlo con: sudo apt install time"
    exit 1
fi


if ! [[ "$REPETITIONS" =~ ^[1-9][0-9]*$ ]]; then
    echo "Error: la cantidad de repeticiones debe ser un entero positivo."
    exit 1
fi


if [[ "$SIZE_FILTER" != "all" ]] &&
   ! [[ "$SIZE_FILTER" =~ ^[1-9][0-9]*$ ]]; then

    echo "Error: el tamaño debe ser un entero positivo o la palabra all."
    exit 1
fi


# --------------------------------------------------
# Preparar directorio y archivo CSV
# --------------------------------------------------

mkdir -p "$MEASUREMENTS_DIRECTORY"

printf '%s\n' \
"algorithm,n,type,domain,sample,repetition,time_seconds,max_rss_kb,input_file" \
> "$CSV_FILE"


# --------------------------------------------------
# Buscar archivos de entrada
# --------------------------------------------------

if [[ "$SIZE_FILTER" == "all" ]]; then
    FILE_PATTERN="*.txt"
else
    FILE_PATTERN="${SIZE_FILTER}_*.txt"
fi


mapfile -d '' INPUT_FILES < <(
    find "$INPUT_DIRECTORY" \
        -maxdepth 1 \
        -type f \
        -name "$FILE_PATTERN" \
        -print0 |
    sort -z
)


if [[ "${#INPUT_FILES[@]}" -eq 0 ]]; then
    echo "Error: no se encontraron archivos con el patrón:"
    echo "$INPUT_DIRECTORY/$FILE_PATTERN"
    exit 1
fi


# --------------------------------------------------
# Archivo temporal para recibir la memoria
# --------------------------------------------------

MEMORY_FILE="$(mktemp)"

cleanup() {
    rm -f "$MEMORY_FILE"
}

trap cleanup EXIT


# --------------------------------------------------
# Ejecutar experimentos
# --------------------------------------------------

TOTAL_RUNS=$(
    (
        ${#INPUT_FILES[@]} *
        ${#ALGORITHMS[@]} *
        REPETITIONS
    )
)

CURRENT_RUN=0
FAILED_RUNS=0


echo "Inicio de las mediciones"
echo "Tamaño seleccionado: $SIZE_FILTER"
echo "Repeticiones: $REPETITIONS"
echo "Archivos de entrada: ${#INPUT_FILES[@]}"
echo "Ejecuciones totales: $TOTAL_RUNS"
echo


for INPUT_FILE in "${INPUT_FILES[@]}"; do

    FILE_NAME="$(basename "$INPUT_FILE")"
    FILE_STEM="${FILE_NAME%.txt}"

    # El nombre oficial tiene la estructura:
    # n_tipo_dominio_muestra.txt
    IFS="_" read -r \
        N_VALUE \
        TYPE_VALUE \
        DOMAIN_VALUE \
        SAMPLE_VALUE \
        EXTRA_VALUE \
        <<< "$FILE_STEM"


    if [[ -n "${EXTRA_VALUE:-}" ]] ||
       [[ -z "${N_VALUE:-}" ]] ||
       [[ -z "${TYPE_VALUE:-}" ]] ||
       [[ -z "${DOMAIN_VALUE:-}" ]] ||
       [[ -z "${SAMPLE_VALUE:-}" ]]; then

        echo "Error: nombre de archivo no reconocido: $FILE_NAME"
        exit 1
    fi


    for ALGORITHM in "${ALGORITHMS[@]}"; do

        for ((REPETITION = 1; REPETITION <= REPETITIONS; REPETITION++)); do

            CURRENT_RUN=$((CURRENT_RUN + 1))

            echo "[$CURRENT_RUN/$TOTAL_RUNS]" \
                 "$ALGORITHM | $FILE_NAME | repetición $REPETITION"


            # /usr/bin/time guarda en MEMORY_FILE el máximo de memoria
            # residente del proceso completo, expresado en KiB.
            if ! PROGRAM_OUTPUT=$(
                /usr/bin/time \
                    -f "%M" \
                    -o "$MEMORY_FILE" \
                    "$EXECUTABLE" \
                    "$ALGORITHM" \
                    "$INPUT_FILE"
            ); then

                echo "  ERROR: la ejecución terminó con un código de error."
                FAILED_RUNS=$((FAILED_RUNS + 1))
                continue
            fi


            MEASUREMENT_LINE=$(
                printf '%s\n' "$PROGRAM_OUTPUT" |
                grep '^MEASUREMENT ' |
                tail -n 1
            )


            if [[ -z "$MEASUREMENT_LINE" ]]; then
                echo "  ERROR: no se encontró la línea MEASUREMENT."
                FAILED_RUNS=$((FAILED_RUNS + 1))
                continue
            fi


            TIME_SECONDS=$(
                printf '%s\n' "$MEASUREMENT_LINE" |
                awk '{
                    for (i = 1; i <= NF; i++) {
                        if ($i ~ /^time_seconds=/) {
                            split($i, value, "=");
                            print value[2];
                        }
                    }
                }'
            )


            STATUS=$(
                printf '%s\n' "$MEASUREMENT_LINE" |
                awk '{
                    for (i = 1; i <= NF; i++) {
                        if ($i ~ /^status=/) {
                            split($i, value, "=");
                            print value[2];
                        }
                    }
                }'
            )


            MAX_RSS_KB="$(tr -d '[:space:]' < "$MEMORY_FILE")"


            if [[ -z "$TIME_SECONDS" ]]; then
                echo "  ERROR: no se pudo obtener el tiempo."
                FAILED_RUNS=$((FAILED_RUNS + 1))
                continue
            fi


            if [[ "$STATUS" != "CORRECT" ]]; then
                echo "  ERROR: el resultado no fue validado como correcto."
                FAILED_RUNS=$((FAILED_RUNS + 1))
                continue
            fi


            if ! [[ "$MAX_RSS_KB" =~ ^[0-9]+$ ]]; then
                echo "  ERROR: la medición de memoria no es válida."
                FAILED_RUNS=$((FAILED_RUNS + 1))
                continue
            fi


            printf '%s,%s,%s,%s,%s,%s,%s,%s,%s\n' \
                "$ALGORITHM" \
                "$N_VALUE" \
                "$TYPE_VALUE" \
                "$DOMAIN_VALUE" \
                "$SAMPLE_VALUE" \
                "$REPETITION" \
                "$TIME_SECONDS" \
                "$MAX_RSS_KB" \
                "$INPUT_FILE" \
                >> "$CSV_FILE"

        done
    done
done


# --------------------------------------------------
# Resumen
# --------------------------------------------------

echo
echo "Mediciones terminadas."
echo "Archivo generado: $CSV_FILE"
echo "Ejecuciones correctas: $((TOTAL_RUNS - FAILED_RUNS))"
echo "Ejecuciones fallidas: $FAILED_RUNS"


if [[ "$FAILED_RUNS" -ne 0 ]]; then
    exit 1
fi

exit 0
