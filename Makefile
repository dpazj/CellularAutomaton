CC = g++

CA: CellularAutomaton.cpp
	${CC} CellularAutomaton.cpp -o CA
run:
	./CA
clean:
	rm -f CA	