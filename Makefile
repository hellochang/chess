CXX = clang++
CXXFLAGS = -std=c++14 -Wall -Werror=vla -MMD -g
EXEC = chess
OBJECTS = main.o GLOBALS.o Game.o Board.o Player.o Human.o Computer.o Render.o window.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
