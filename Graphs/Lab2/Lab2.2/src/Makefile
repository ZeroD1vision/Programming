# Makefile для сборки проекта проверки планарности графа

# Переменные
LIB_NAME 	 := lib
MAIN_NAME 	 := main
BUILD_DIR 	 := ./build

# Пути к файлам (выходные в ./build)
LIB_SRC 	 := ./lib/$(LIB_NAME).cpp
LIB_OBJ 	 := $(BUILD_DIR)/$(LIB_NAME).o
LIB_STATIC	 := $(BUILD_DIR)/$(LIB_NAME).a
MAIN_SRC     := ./$(MAIN_NAME).cpp
MAIN_OBJ     := $(BUILD_DIR)/$(MAIN_NAME).o
EXECUTABLE   := $(BUILD_DIR)/$(MAIN_NAME).exe

# Компилятор и флаги
CXX			 := g++
CXXFLAGS     := -fdiagnostics-color=always -g -I. -std=c++17 -pthread

# Команды
MKDIR        := @mkdir -p "$(BUILD_DIR)"
RM           := @rm -rf "$(BUILD_DIR)"

# --- Цели ---

.PHONY: all clean build run test help

# Справка
help:
	@echo "Доступные цели:"
	@echo "  make all     - Собрать проект"
	@echo "  make build   - Собрать проект"
	@echo "  make run     - Запустить с тестом 1"
	@echo "  make test N=1 - запустить с тестом N (1-15)"
	@echo "  make clean   - Очистить билд"
	@echo "  make help    - Показать справку"

# Главная цель: по умолчанию собирает и запускает
all: build test

# Сборка: создать библиотеку и исполняемый файл
build: $(EXECUTABLE) $(LIB_STATIC)

# Компиляция объектного файла библиотеки
$(LIB_OBJ): $(LIB_SRC)
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -c $(LIB_SRC) -o $@

# Создание статической библиотеки
$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $@ $<

# Компиляция основного файла
$(MAIN_OBJ): $(MAIN_SRC)
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $@

# Сборка исполняемого файла (линковка с библиотекой)
$(EXECUTABLE): $(MAIN_OBJ) $(LIB_STATIC)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) $(LIB_STATIC) -o $@

###################################
#			Т Е С Т Ы	  		  #
###################################

# Номер теста по умолчанию
N = 1

# Функция для генерации имени тестового файла
TEST_FILE = ../tests/epg_t2_$(shell printf "%03d" $(N)).txt
OUTPUT_FILE = ./output/result_$(shell printf "%03d" $(N)).txt

# Создание выходной директории
create-output-dir:
	@mkdir -p "./output"

# Запуск с тестом 1 по умолчанию
run: $(EXECUTABLE) create-output-dir
	./$(EXECUTABLE) $(TEST_FILE) -o $(OUTPUT_FILE)

# Запуск с конкретным тестом (N=1..15)
test: $(EXECUTABLE) create-output-dir
	./$(EXECUTABLE) $(TEST_FILE) -o $(OUTPUT_FILE)

# Запуск всех тестов (1-15)
test-all: $(EXECUTABLE) create-output-dir
	@echo "Запуск всех тестов (1-15)..."
	@for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do \
		echo "=== Тест $$i ==="; \
		./$(EXECUTABLE) ../tests/epg_t2_$$(printf "%03d" $$i).txt -o ./output/result_$$(printf "%03d" $$i).txt; \
	done
	@echo "Все тесты завершены"

# Очистка
clean:
	$(RM)
	@rm -rf ./output