SHELL := /bin/bash

CXX = g++
CXXFLAGS = -std=c++17 -O2

TARGET = sorting

SOURCES = sorting.cpp \
algorithms/mergesort.cpp \
algorithms/quicksort.cpp \
algorithms/patiencesort.cpp \
algorithms/sort.cpp

OBJECTS = $(SOURCES:.cpp=.o)

ALGORITHMS = merge quick patience std

INPUT_DIR = data/array_input
OUTPUT_DIR = data/array_output
MEASUREMENTS_DIR = data/measurements
MEASUREMENTS = $(MEASUREMENTS_DIR)/sorting_measurements.csv

TIMEOUT_SECONDS = 30

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --------------------------------------------------
# Small test
# Runs every algorithm over all n = 10 datasets
# --------------------------------------------------

run-small: $(TARGET)
	@mkdir -p $(MEASUREMENTS_DIR)
	@mkdir -p $(OUTPUT_DIR)
	@echo "algorithm,n,type,domain,sample,status,time_seconds,max_rss_kb,error" > $(MEASUREMENTS)
	@for file in $(INPUT_DIR)/10_*.txt; do \
		echo "Procesando $$file"; \
		base=$$(basename "$$file" .txt); \
		IFS="_" read -r n type domain sample <<< "$$base"; \
		for alg in $(ALGORITHMS); do \
			echo "  Algoritmo: $$alg"; \
			timeout $(TIMEOUT_SECONDS)s ./$(TARGET) $$alg $$file; \
			status=$$?; \
			if [ $$status -eq 124 ]; then \
				echo "$$alg,$$n,$$type,$$domain,$$sample,TIMEOUT,,,supero_limite_de_tiempo" >> $(MEASUREMENTS); \
				echo "  TIMEOUT: $$alg superó $(TIMEOUT_SECONDS) segundos"; \
			elif [ $$status -ne 0 ]; then \
				echo "$$alg,$$n,$$type,$$domain,$$sample,ERROR,,,ejecucion_fallida" >> $(MEASUREMENTS); \
				echo "  ERROR: $$alg terminó con código $$status"; \
			fi; \
		done; \
	done

# --------------------------------------------------
# Full experiment
# Runs every algorithm over every official input
# Maximum execution time: 30 seconds per case
# --------------------------------------------------

run: $(TARGET)
	@mkdir -p $(MEASUREMENTS_DIR)
	@mkdir -p $(OUTPUT_DIR)
	@echo "algorithm,n,type,domain,sample,status,time_seconds,max_rss_kb,error" > $(MEASUREMENTS)
	@total=$$(find $(INPUT_DIR) -maxdepth 1 -type f -name "*.txt" | wc -l); \
	total=$$((total * 4)); \
	current=0; \
	for file in $(INPUT_DIR)/*.txt; do \
		base=$$(basename "$$file" .txt); \
		IFS="_" read -r n type domain sample <<< "$$base"; \
		for alg in $(ALGORITHMS); do \
			current=$$((current + 1)); \
			echo "[$$current/$$total] $$alg | $$file"; \
			timeout $(TIMEOUT_SECONDS)s ./$(TARGET) $$alg $$file; \
			status=$$?; \
			if [ $$status -eq 124 ]; then \
				echo "$$alg,$$n,$$type,$$domain,$$sample,TIMEOUT,,,supero_limite_de_tiempo" >> $(MEASUREMENTS); \
				echo "  TIMEOUT: $$alg superó $(TIMEOUT_SECONDS) segundos"; \
			elif [ $$status -ne 0 ]; then \
				echo "$$alg,$$n,$$type,$$domain,$$sample,ERROR,,,ejecucion_fallida" >> $(MEASUREMENTS); \
				echo "  ERROR: $$alg terminó con código $$status"; \
			fi; \
		done; \
	done

# --------------------------------------------------
# Generate official input datasets
# --------------------------------------------------

generate:
	cd scripts && python3 array_generator.py

# --------------------------------------------------
# Generate plots
# --------------------------------------------------

plots:
	cd scripts && python3 plot_generator.py

# --------------------------------------------------
# Remove compiled files
# --------------------------------------------------

clean:
	rm -f $(OBJECTS) $(TARGET)

# --------------------------------------------------
# Remove experimental results
# --------------------------------------------------

clean-results:
	rm -f $(MEASUREMENTS)
	rm -f $(OUTPUT_DIR)/*

.PHONY: all run-small run generate plots clean clean-results