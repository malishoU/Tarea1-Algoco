SHELL := /bin/bash

CXX = g++
CXXFLAGS = -std=c++17 -O2

TARGET = matrix_multiplication

SOURCES = matrix_multiplication.cpp \
          algorithms/naive.cpp \
          algorithms/strassen.cpp

OBJECTS = $(SOURCES:.cpp=.o)

INPUT_DIR = data/matrix_input
OUTPUT_DIR = data/matrix_output
MEASUREMENTS = data/measurements/matrix_measurements.csv

ALGORITHMS = naive strassen
TIMEOUT_SECONDS = 60

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --------------------------------------------------
# Ejecuta todos los casos de prueba.
# 72 pares de matrices x 2 algoritmos = 144 pruebas.
#
# El programa C++ registra las ejecuciones exitosas.
# Este makefile registra los TIMEOUT y los errores.
# --------------------------------------------------

run: $(TARGET)
	@mkdir -p "$(dir $(MEASUREMENTS))" "$(OUTPUT_DIR)"
	@printf '%s\n' 'algorithm,n,type,domain,sample,status,time_seconds,max_rss_kb,error' > "$(MEASUREMENTS)"
	@rm -f $(OUTPUT_DIR)/*_out.txt
	@count=0; total=144; \
	for file1 in $(INPUT_DIR)/*_1.txt; do \
		[ -f "$$file1" ] || continue; \
		file2=$${file1%_1.txt}_2.txt; \
		base=$${file1##*/}; \
		base=$${base%_1.txt}; \
		IFS='_' read -r n type domain sample <<< "$$base"; \
		for alg in $(ALGORITHMS); do \
			count=$$((count + 1)); \
			echo "[$$count/$$total] $$alg | $$file1"; \
			timeout $(TIMEOUT_SECONDS)s ./$(TARGET) "$$alg" "$$file1" "$$file2"; \
			status=$$?; \
			if [ $$status -eq 124 ]; then \
				printf '%s,%s,%s,%s,%s,TIMEOUT,,,supero_limite_de_tiempo\n' "$$alg" "$$n" "$$type" "$$domain" "$$sample" >> "$(MEASUREMENTS)"; \
				echo "TIMEOUT: $$alg | $$file1"; \
			elif [ $$status -ne 0 ]; then \
				printf '%s,%s,%s,%s,%s,ERROR,,,codigo_salida_%s\n' "$$alg" "$$n" "$$type" "$$domain" "$$sample" "$$status" >> "$(MEASUREMENTS)"; \
				echo "ERROR: $$alg | $$file1"; \
			fi; \
		done; \
	done

# --------------------------------------------------
# Genera todos los archivos de entrada.
# --------------------------------------------------

generate:
	cd scripts && python3 matrix_generator.py

# --------------------------------------------------
# Genera los gráficos a partir de las mediciones.
# --------------------------------------------------

plots:
	cd scripts && python3 plot_generator.py

# --------------------------------------------------
# Elimina archivos de compilación.
# --------------------------------------------------

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all run generate plots clean