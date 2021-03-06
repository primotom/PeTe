#GNU Bison qmake configuration by Thomas Capricelli, see:
#http://www.freehackers.org/thomas/2009/11/22/how-to-use-flex-and-bison-with-qmake-my-own-way/
bison.name = Bison ${QMAKE_FILE_IN}
bison.input = BISONSOURCES
bison.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.parser.cpp
bison.commands = bison -d -o ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.parser.cpp ${QMAKE_FILE_IN}
bison.CONFIG += target_predeps
bison.variable_out = GENERATED_SOURCES
silent:bison.commands = @echo Bison ${QMAKE_FILE_IN} && $$bison.commands
QMAKE_EXTRA_COMPILERS += bison
bison_header.input = BISONSOURCES
bison_header.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.parser.hpp
bison_header.commands = bison -d -o ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.parser.cpp ${QMAKE_FILE_IN}
bison_header.CONFIG += target_predeps no_link
silent:bison_header.commands = @echo Bison ${QMAKE_FILE_IN} && $$bison.commands
QMAKE_EXTRA_COMPILERS += bison_header

OTHER_FILES += BISONSOURCES
