CXX		  := g++
CXX_FLAGS := -Wall -Wextra -Wpedantic -std=c++17 -ggdb

BIN		:= bin
SRC_DIR	:= src src/controller src/model src/view ui
SRC     := $(foreach d, $(SRC_DIR), $d/*.cpp)
INCLUDE	:= /usr/local/include /usr/include $(SRC_DIR)
LIB		:= /usr/local/lib

LIBRARIES	:= -lrfftw -lfftw $(shell fltk-config --use-gl --ldstaticflags)
EXECUTABLE	:= smoke

style:
	@for src in $(SRC) ; do \
		echo "Formatting $$src..." ; \
		clang-format -i "$(SRC_DIR)/$$src" ; \
		clang-tidy -checks='-*,readability-identifier-naming' \
		    -config="{CheckOptions: [ \
		    { key: readability-identifier-naming.NamespaceCase, value: lower_case },\
		    { key: readability-identifier-naming.ClassCase, value: CamelCase  },\
		    { key: readability-identifier-naming.StructCase, value: CamelCase  },\
		    { key: readability-identifier-naming.FunctionCase, value: camelBack },\
		    { key: readability-identifier-naming.VariableCase, value: lower_case },\
		    { key: readability-identifier-naming.GlobalConstantCase, value: UPPER_CASE }\
		    ]}" "$(SRC_DIR)/$$src" ; \
	done
	@echo "Done"

check-style:
	@for src in $(SRC) ; do \
		var=`clang-format "$(SRC_DIR)/$$src" | diff "$(SRC_DIR)/$$src" - | wc -l` ; \
		if [ $$var -ne 0 ] ; then \
			echo "$$src does not respect the coding style (diff: $$var lines)" ; \
			exit 1 ; \
		fi ; \
	done
	@echo "Style check passed"
  
tidy:
	@for src in $(SRC) ; do \
		echo "Running tidy on $$src..." ; \
		clang-tidy -checks="-*,modernize-use-auto,modernize-use-nullptr,\
			readability-else-after-return,readability-simplify-boolean-expr,\
			readability-redundant-member-init,modernize-use-default-member-init,\
			modernize-use-equals-default,modernize-use-equals-delete,\
			modernize-use-using,modernize-loop-convert,\
			cppcoreguidelines-no-malloc,misc-redundant-expression" \
			-header-filter=".*" \
			"$(SRC_DIR)/$$src" ; \
	done
	@echo "Done"
  
.PHONY: style check-style tidySOURCES


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)
	$(CXX) $(CXX_FLAGS) $(foreach d, $(INCLUDE), -I$d) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	clear
	-rm $(BIN)/*

$(shell mkdir -p bin)