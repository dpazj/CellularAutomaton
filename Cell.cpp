//Joe Riemersma
#include "Cell.h"

using namespace std;

/* status:
	0 - dead
	1 - alive
	2 - alivedead
	3 - deadalive
*/

Cell::Cell(){
	status = 0;
}

int Cell::getStatus(){ //returns cell status
	return status;
}

void Cell::Awake(){
	status = 3;
}

void Cell::Kill(){
	status = 2;
}

void Cell::Update(){ //Updates cell.
	if(status == 2){
		status = 0;
	}else if(status == 3){
		status = 1;
	}
}