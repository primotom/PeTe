#Programs for processing
LEX				= flex
YACC			= bison
CC				= g++
RM				= rm
FIND			= find
GREP			= grep

#Compiler and linker flags
CFLAGS			= -O0 -I. -g
LDFLAGS			= -g -static -O3 -llpsolve55 -lcolamd -ldl

#Target
TARGET			= PeTAPAAL

#Source files
FLEX_SOURCES	= $(shell $(FIND) * -name "*.l")
BISON_SOURCES	= $(shell $(FIND) * -name "*.y")
SOURCES			= $(shell $(FIND) * -name "*.cpp" | $(GREP) -v ".\\(parser\\|lexer\\).cpp")	\
				  $(BISON_SOURCES:.y=.parser.cpp)												\
				  $(FLEX_SOURCES:.l=.lexer.cpp)

#Intermediate files
OBJECTS			= $(SOURCES:.cpp=.o)															\
				  $(FLEX_SOURCES:.l=.lexer.cpp)													\
				  $(BISON_SOURCES:.y=.parser.cpp)												\
				  $(BISON_SOURCES:.y=.parser.hpp)

#Build rules
%.lexer.cpp: %.l
	$(LEX) -o $@ $<
%.parser.cpp: %.y
	$(YACC) -d -o $@ $<
%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<
$(TARGET): $(SOURCES:.cpp=.o)
	$(CC) $^ $(LDFLAGS) -o $@

#Phony targets
all: $(TARGET)
clean:
	$(RM) -f $(OBJECTS) $(TARGET)
.PHONY: all clean
