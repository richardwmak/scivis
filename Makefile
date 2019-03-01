CXX		  := g++
CXX_FLAGS := -Wall -Wextra -Wpedantic -std=c++17 -ggdb

BIN		:= bin
SRC		:= src src/controller src/model src/view ui
INCLUDE	:= /usr/local/include /usr/include $(SRC)
LIB		:= /usr/local/lib

LIBRARIES	:= -lrfftw -lfftw $(shell fltk-config --use-gl --ldstaticflags)
EXECUTABLE	:= smoke


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(foreach d, $(SRC), $d/*.cpp)
	$(CXX) $(CXX_FLAGS) $(foreach d, $(INCLUDE), -I$d) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	clear
	-rm $(BIN)/*

$(shell mkdir -p bin)