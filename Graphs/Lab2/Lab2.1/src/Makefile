# Makefile для сборки проекта с сортировочными алгоритмами на Windows (MSYS2/Git Bash)

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

.PHONY: all clean build run

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

N = 4
# Запуск с тестом 1 по умолчанию
run: $(EXECUTABLE)
	./$(EXECUTABLE) 1

# Запуск с конкретным тестом (N=...)
test: $(EXECUTABLE)
	./$(EXECUTABLE) $(N)

# Запуск с 8 потоками
test-threads: $(EXECUTABLE)
	./$(EXECUTABLE) 1 -n 8

clean:
	$(RM)