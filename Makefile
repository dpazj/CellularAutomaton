CC = g++

CA: CellularAutomaton.cpp Cell.o LifeGrid.o
	${CC} CellularAutomaton.cpp LifeGrid.o Cell.o -o CA

LifeGrid.o: LifeGrid.cpp LifeGrid.h 
	${CC} -c LifeGrid.cpp 

Cell.o: Cell.cpp Cell.h
	${CC} -c Cell.cpp 

run:
	./CA
clean:
	rm -f CA LifeGrid.o  Cell.o	