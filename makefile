OBJECTS     = fluids.o
CPPFILES    = $(OBJECTS:.o=.c)
EXECFILE    = smoke
INCLUDEDIRS = -I./fftw-2.1.5/include/
LIBDIRS     = -L./usr/local/lib
LIBS        = -lrfftw -lfftw -lglut -lGL -lGLU -lGLEW -lm
#Possible flags for release (ffast-math uses less precision for floating-point numbers, check that your application can handle this)
#CFLAGS      = -O3 -march=x86-64 -mtune=generic -DNDEBUG -mfpmath=sse -ffast-math -Wall -pipe
#Debug flags
CPPFLAGS    = -ggdb -Wall -pipe
LINKFLAGS   =

# list all targets that are not files
.PHONY: style check-style tidy clean depend all

# use clang-tidy to check code
# source: https://gist.github.com/dtoma/61468552bbc7c0114b2e700f9247a310
style:
	@for src in $(SOURCES) ; do \
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
	@for src in $(SOURCES) ; do \
		var=`clang-format "$(SRC_DIR)/$$src" | diff "$(SRC_DIR)/$$src" - | wc -l` ; \
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
			"$(SRC_DIR)/$$src" ; \
	done
	@echo "Done"

	
# compile to executable
all: $(EXECFILE)

$(EXECFILE): $(OBJECTS)
	gcc $(LINKFLAGS) $(OBJECTS) -o $@ $^ $(LIBDIRS) $(LIBS) 

%.o: %.cpp
	gcc $(CPPFLAGS) $(INCLUDEDIRS) -c -o $@ $<

# remove all compiled files
clean:
	-rm -rf $(OBJECTS) $(EXECFILE)

depend:
	gcc -MM $(CPPFILES) > make.dep
  



-include make.dep
