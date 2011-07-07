#Target
TARGET			= PeTAPAAL

#Source files
FLEX_SOURCES	= $(shell find -name "*.l")
BISON_SOURCES	= $(shell find -name "*.y")
SOURCES			= $(shell find -name "*.cpp" | grep -v ".lexer.cpp")	\
				  $(BISON_SOURCES:.y=.parser.cpp)						\
				  $(FLEX_SOURCES:.l=.lexer.cpp)

#Platform dependent settings
LEX				= flex
YACC			= bison
CC				= g++
RM				= rm
CFLAGS			= -O3 -I.
LDFLAGS			= -static -O3 -llpsolve55 -lcolamd -ldl

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
	$(RM) -f $(SOURCES:.cpp=.o)					\
			 $(FLEX_SOURCES:.l=.lexer.cpp)		\
			 $(BISON_SOURCES:.y=.parser.cpp)	\
			 $(BISON_SOURCES:.y=.parser.hpp)	\
			 $(TARGET)
.PHONY: all clean
