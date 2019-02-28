CXX		  := g++
CXX_FLAGS := -Wall -Wextra -Wpedantic -std=c++17 -ggdb

BIN		:= bin
SRC		:= src ui
INCLUDE	:= /usr/local/include /usr/include include ui
LIB		:= /usr/local/lib

LIBRARIES	:= -lrfftw -lfftw $(shell fltk-config --use-gl --ldstaticflags)
EXECUTABLE	:= smoke

# use clang-tidy to check code
# source: https://gist.github.com/dtoma/61468552bbc7c0114b2e700f9247a310
style:
	@for src in $(SOURCES) ; do \
		echo "Formatting $$src..." ; \
		clang-format -i "$(SRC)/$$src" ; \
		clang-tidy -checks='-*,readability-identifier-naming' \
		    -config="{CheckOptions: [ \
		    { key: readability-identifier-naming.NamespaceCase, value: lower_case },\
		    { key: readability-identifier-naming.ClassCase, value: CamelCase  },\
		    { key: readability-identifier-naming.StructCase, value: CamelCase  },\
		    { key: readability-identifier-naming.FunctionCase, value: camelBack },\
		    { key: readability-identifier-naming.VariableCase, value: lower_case },\
		    { key: readability-identifier-naming.GlobalConstantCase, value: UPPER_CASE }\
		    ]}" "$(SRC)/$$src" ; \
	done
	@echo "Done"

check-style:
	@for src in $(SOURCES) ; do \
		var=`clang-format "$(SRC)/$$src" | diff "$(SRC)/$$src" - | wc -l` ; \
		if [ $$var -ne 0 ] ; then \
			echo "$$src does not respect the coding style (diff: $$var lines)" ; \
			exit 1 ; \
		fi ; \
	done
	@echo "Style check passed"
  
tidy:
	@for src in $(SOURCES) ; do \
		echo "Running tidy on $$src..." ; \
		clang-tidy -checks="-*,modernize-use-auto,modernize-use-nullptr,\
			readability-else-after-return,readability-simplify-boolean-expr,\
			readability-redundant-member-init,modernize-use-default-member-init,\
			modernize-use-equals-default,modernize-use-equals-delete,\
			modernize-use-using,modernize-loop-convert,\
			cppcoreguidelines-no-malloc,misc-redundant-expression" \
			-header-filter=".*" \
			"$(SRC)/$$src" ; \
	done
	@echo "Done"

all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(foreach d, $(SRC), $d/*.cpp)
	$(CXX) $(CXX_FLAGS) $(foreach d, $(INCLUDE), -I$d) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	clear
	-rm $(BIN)/*

$(shell mkdir -p bin)