//Joe Riemersma

#include <iostream>
#include <stdlib.h> 
#include "LifeGrid.h"


Grid::Grid(int width,int height): x_width (width), y_height (height){
	grid.resize(x_width);			//Resizes grid to x width and y hight
	for(int i(0); i < x_width; i++) {
		grid[i].resize(y_height);
	}
	

	InitializeGrid();
}

Grid::~Grid(){	//deconstructor
	for(int i(0); i < y_height; i++) {
		for(int j(0); j < x_width; j++) {
			delete grid[j][i];
			grid[j][i] = 0;
		}
	}
}

void Grid::InitializeGrid(){ //Initialises grid with new cell objects
	for(int i = 0; i < y_height; i++){
		for(int j = 0; j < x_width; j++){
			grid[j][i] = new Cell;
		}
	}
}

void Grid::DisplayGrid(){ //Displays grid
	std::cout << "\033c" << std::endl;
	for(int i = 0; i < y_height; i++){
		for(int j = 0; j < x_width; j++){
			grid[j][i]->Update();
			if(grid[j][i]->getStatus() == 0){
				std::cout << " ";
			}else{
				std::cout << "*";
			}
			 
		}
		std::cout << std::endl;
	}
}

void Grid::InitialGeneration(){ //Randomly generates cells to be dead/alive
	srand(time(NULL));
	for(int i = 0; i < y_height; i++){
		for(int j = 0; j < x_width; j++){
			if(rand() % 2 == 0){
				grid[j][i] -> Awake();
			}else{
				grid[j][i] -> Kill();
			}

		}
	}
}

void Grid::GenerateGlider(){ //Used for preset grid
	//First box
	int x = 80;
	int y = 20;
	grid[x][y]->Awake();
	grid[x+1][y]->Awake();
	grid[x][y+1]->Awake();
	grid[x+1][y+1]->Awake();

	//Second box
	x = 114;
	y = 18;
	grid[x][y]->Awake();
	grid[x+1][y]->Awake();
	grid[x][y+1]->Awake();
	grid[x+1][y+1]->Awake();

	//Shape 1
	x = 88;
	y = 21;
	grid[x][y]->Awake();
	grid[x][y+1]->Awake();
	grid[x+1][y+1]->Awake();

	grid[x+1][y-1]->Awake();
	grid[x+2][y-1]->Awake();
	grid[x+2][y]->Awake();

	//Shape 2
	x = 102;
	y = 19;
	grid[x][y]->Awake();
	grid[x][y+1]->Awake();
	grid[x+1][y+1]->Awake();

	grid[x+1][y-1]->Awake();
	grid[x+2][y-1]->Awake();
	grid[x+2][y]->Awake();

	//Glider 1
	x = 96;
	y = 22;
	grid[x][y]->Awake();
	grid[x+1][y]->Awake();
	grid[x+2][y+1]->Awake();
	grid[x][y+1]->Awake();
	grid[x][y+2]->Awake();
	

	//Glider 2
	x = 115;
	y = 25;
	grid[x][y]->Awake();
	grid[x+1][y]->Awake();
	grid[x+2][y+1]->Awake();
	grid[x][y+1]->Awake();
	grid[x][y+2]->Awake();

	//Glider 3
	x = 104;
	y = 30;
	grid[x][y]->Awake();
	grid[x+1][y]->Awake();
	grid[x+2][y]->Awake();
	grid[x][y+1]->Awake();
	grid[x+1][y+2]->Awake();
	
}


void Grid::Generation(){ //Iterates through each item in the grid, logic to decide if cell is dead or not.
	for(int i = 0; i < y_height; i++){
		for(int j = 0; j < x_width; j++){
			if(grid[j][i]->getStatus() == 0){
				int alive = NeighbourAlive(j,i);
				if(alive == 3){
					grid[j][i]->Awake();
				}
			}else if(grid[j][i]->getStatus() == 1){
				int alive = NeighbourAlive(j,i);
				if(alive < 2 || alive > 3){
					grid[j][i]->Kill();
				}
			}
		}
	}
}

int Grid::Alive(int x, int y){ //Checks if element at x,y is alive
	
	if(x < 0 && y < 0){ //top left out of bounds
		return Alive(x_width-1,y_height-1);
	} 
	else if(x > x_width -1 && y < 0){ //top right out of bounds
		return Alive(0,y_height-1);
	} 
	else if(x < 0 && y > y_height-1){ //bottom left out of bounds
		return Alive(x_width-1,0);
	} 
	else if(x > x_width-1 && y > y_height-1){ //bottom right out of bounds
		return Alive(0,0);
	} 
	else if(y < 0){return Alive(x,y_height-1);} //top out of bounds
	else if(y > y_height-1){return Alive(x,0);} //bottom out of bounds
	else if(x < 0){return Alive(x_width-1,y);} //left out of bounds
	else if(x > x_width-1){return Alive(0,y);} //right out of bounds
	
	if(grid[x][y]->getStatus() == 1 || grid[x][y]->getStatus() == 2){
		return 1;
	}else{
		return 0;
	}
}

int Grid::NeighbourAlive(int j, int i){ //Gets number of alive 
	int alive = 0;

	alive += Alive(j + 1, i - 1); //top right
	alive += Alive(j - 1, i - 1); //top left
	alive += Alive(j, i - 1); //top
	alive += Alive(j - 1, i); //left
	alive += Alive(j + 1, i); //right
	alive += Alive(j - 1, i + 1); //bottom left
	alive += Alive(j + 1, i + 1); //bottom right
	alive += Alive(j, i + 1); //bottom

	return alive;

}

