//Joe Riemersma

#include <iostream>
#include "Cell.h"
#include <vector>

class Grid{
	public:
		Grid(int x, int y);
		~Grid();
		void InitialGeneration();
		void GenerateGlider();
		void Generation();
		void DisplayGrid();



	private:
		void InitializeGrid();
		int Alive(int x, int y);
		int NeighbourAlive(int x, int y);

		int x_width;
		int y_height;

		std::vector<std::vector<Cell*> > grid; //2d Array of Cell Objects

};