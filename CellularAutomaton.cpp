//Joe Riemersma and Claire Sommerville
//Cellular Automation 
//18/10/17

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <bitset>
#include <string>

using namespace std;

char *parentGen;
char *childGen;


string ruleset;
int genSize;
int iterations;

bool singleline;
bool writeFile;
bool wrap = false;
string writeFileName;


bool validSeq(string seq){ //checks string to see if it is a binary sequence
	for(int i = 0; i < seq.length(); i++){
		if(seq.at(i) != '0' && seq.at(i) != '1'){
			return true;
		}
	}
	return false;
}

void RandGeneration(){ //Generates a random binary sequence
	srand(time(NULL)); //Used to make more random each time **changes the seed**
	for(int i = 0; i < genSize; i++){	
		int temp = rand() % 2;	// generates the random number
		if(temp == 0){			//casting an int to char was being a pain 
			parentGen[i] = '0'; 
		}else if(temp == 1){
			parentGen[i] = '1';
		} 
	}
}

void InitGenerations(){  //iniitalises arrays
	parentGen = new char[genSize];
	childGen = new char[genSize];
	for(int i = 0; i < genSize; i++){
		parentGen[i] = '0';
	}
}

void OutputCharArray(char arrayToPrint[]){ //Prints binary sequence to terminal and outputs to file if user selcted.
	
	//Print to terminal
	for(int i = 0; i < genSize; i++){
		cout << arrayToPrint[i];
	}
	if(singleline){
		getchar(); //make user enter something before next row is displayed.
	}else{         //getchar() creates a new line
		cout << endl;
	}	

	//Write to file.
	if(writeFile){
		ofstream ofile;
		ofile.open(writeFileName.c_str(),std::ios_base::app);
		for(int i = 0; i < genSize; i++){
			ofile << arrayToPrint[i];
		}
		ofile << endl;
		ofile.close();
	}
	
}

char FindNextCell(int cellIndex){ //Works out the child cell based on the current and two cells next to current cell
	char left;
	char middle;
	char right;

	middle = parentGen[cellIndex];
	if(cellIndex == 0){	//Sets values of right and left when index is either zero or at the size of the array. Prevents out of bounds exception.
		if(wrap){
			left = parentGen[genSize];
		}else{
			left = '0';
		}
		right = parentGen[(cellIndex + 1)];
	}else if(cellIndex == (genSize - 1)){
		if(wrap){
			right = parentGen[0];
		}else{  							
			right = '0';
		}
		left = parentGen[(cellIndex - 1)];
	}else{
		right = parentGen[(cellIndex + 1)];
		left = parentGen[(cellIndex - 1)];
	}

	//Binary operation logic
	if(left == '1' && middle == '1' && right == '1'){
		return ruleset.at(0);
	}
	if(left == '1' && middle == '1' && right == '0'){
		return ruleset.at(1);
	}
	if(left == '1' && middle == '0' && right == '1'){
		return ruleset.at(2);
	}
	if(left == '1' && middle == '0' && right == '0'){
		return ruleset.at(3);
	}
	if(left == '0' && middle == '1' && right == '1'){
		return ruleset.at(4);
	}
	if(left == '0' && middle == '1' && right == '0'){
		return ruleset.at(5);
	}
	if(left == '0' && middle == '0' && right == '1'){
		return ruleset.at(6);
	}
	if(left == '0' && middle == '0' && right == '0'){
		return ruleset.at(7);
	}
}

void StartAutomaton(){ //Iterates through parent sequence to produce child sequences, and does this n times as specified by user.
	for(int j = 0; j < iterations; j++){ 
		OutputCharArray(parentGen);
		for(int i = 0; i < genSize; i++){
		  	childGen[i] = FindNextCell(i); //gets new value for element i from binary operation.
		}	
		parentGen = childGen; //set child to parent
		childGen = new char[genSize]; //creates new child array
	}
}



bool testInt(char* argv){ //Test argument char array for an integer
	int temp;
	istringstream iss(argv);
	if(!(iss >> temp)) {return false;}
	return true;
}

int main(int argc, char* argv[]){ //Processes command line arguments
	string USAGE("Usage: ./CA [-db] [-bd] [[-a] [[-i=<'sequenceSize'>][-d=<'sequenceSize'][-c]] [[-i=<'ruleset'>][-d][-c]] [[-i=<'repetitions'][-sl=<'repetitions'>]] [-s=<'outputfile'>] [-w]] \nTake a look at the help file for more information");

	int argIndex = 1;
	
	bool randGen = false;
	bool defaultGen = false;
	bool customGen = false;
	string customSeq;

	bool customBitset = false;
	

	if(argc == 1){
		cout << USAGE << endl;
		return 1;
	}else{
		if(string(argv[argIndex]) == "-a" || string(argv[argIndex]) == "--automaton"){
			
			//PARENT SEQUENCE ARGUMENTS
			argIndex++;
			if(argIndex >= argc){
				cout << USAGE << endl;
				return 1;
			}else if(string(argv[argIndex]) == "-i" || string(argv[argIndex]) == "--input"){
				argIndex++;
				if(argIndex >= argc){cout << USAGE << endl;return 1;}

				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 0 || atoi(argv[argIndex]) > 200){
						cout << "'-i' should be follwed by an integer between 0 and 200" << endl;
						cout << USAGE << endl;
						return 1;
					}else{
						genSize = atoi(argv[argIndex]);
						randGen = true;
					}
				}else{ 
					cout << "Please follow '-i' with an integer" << endl;
					cout << USAGE << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "-d" || string(argv[argIndex]) == "--default"){
				argIndex++;
				if(argIndex >= argc){cout << USAGE << endl;return 1;}

				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 0 || atoi(argv[argIndex]) > 200){
						cout << "the first '-d' should be follwed by an integer between 0 and 200" << endl;
						cout << USAGE << endl;
						return 1;
					}else{
						genSize = atoi(argv[argIndex]);
						defaultGen = true;
					}
				}else{ 
					cout << "Please follow the first '-d' with an integer" << endl;
					cout << USAGE << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "-c" || string(argv[argIndex]) == "--custom"){
				customGen = true;
			}else{
				cout << USAGE << endl;
				return 1;
			}

			//RULESET ARGUMENTS
			argIndex++;
			if(argIndex >= argc){
				cout << USAGE << endl;
				return 1;
			}else if(string(argv[argIndex]) == "-i" || string(argv[argIndex]) == "--input"){
				argIndex++;
				if(argIndex >= argc){cout << USAGE << endl;return 1;}
				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 0 || atoi(argv[argIndex]) > 255){
						cout << "-i' should be follwed by an integer between 0 and 255" << endl;
						cout << USAGE << endl;
						return 1;
					}else{
						bitset<8> a (atoi(argv[argIndex]));
						ruleset = a.to_string();
					}
				}else{ 
					cout << "Please follow '-i' with an integer";
					cout << USAGE << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "-d" || string(argv[argIndex]) == "--default"){
				bitset<8> a (30);
				ruleset = a.to_string();
			}else if(string(argv[argIndex]) == "-c" || string(argv[argIndex]) == "--custom"){
				customBitset = true;
			}else{
				cout << USAGE << endl;
				return 1;
			}

			//ITERATION ARGUMENTS
			argIndex++;
			if(argIndex >= argc){
				cout << USAGE << endl;
				return 1;
			}else if(string(argv[argIndex]) == "-i" || string(argv[argIndex]) == "--input"){
				argIndex++;
				if(argIndex >= argc){cout << USAGE << endl;return 1;}
				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 1){
						cout << "-i' should be follwed by an integer greater than 0" << endl;
						cout << USAGE << endl;
						return 1;
					}else{
						iterations = atoi(argv[argIndex]);
					}
				}else{ 
					cout << "Please follow '-i' with an integer";
					cout << USAGE << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "-sl" || string(argv[argIndex]) == "--singleline"){
				singleline = true;
				argIndex++;
				if(argIndex >= argc){cout << USAGE << endl;return 1;}
				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 1){
						cout << "-sl' should be follwed by an integer greater than 0" << endl;
						cout << USAGE << endl;
						return 1;
					}else{
						iterations = atoi(argv[argIndex]);
					}
				}else{ 
					cout << "Please follow '-s' with an integer";
					cout << USAGE << endl;
					return 1;
				}
			}else{
				cout << USAGE << endl;
				return 1;
			}

			//Gets rest of arguments
			argIndex++;
			while(argIndex < argc){
				if(string(argv[argIndex]) == "-s" || string(argv[argIndex]) == "--save" ){
					argIndex++;
					if(argIndex >= argc){cout << USAGE << endl;return 1;}
					writeFileName = string(argv[argIndex]);
					writeFile = true;
				}else if(string(argv[argIndex]) == "-w" || string(argv[argIndex]) == "--wrap"){
						wrap = true;
					
				}else{
					cout << "Command '" << string(argv[argIndex]) << "' not recognised\n" <<USAGE << endl;
				}
				argIndex++;
			}

			//Get custom sequence
			if(customGen){
				cout << "Please enter your custom sequence" << endl;
				cin >> customSeq;

				while(validSeq(customSeq)){
					cin.clear();
					cin.ignore();
					cout << "Please enter a valid sequence" << endl;
					cin >> customSeq;
				}
				genSize = customSeq.length();
			}
			//Get custom Bitset;
			if(customBitset){
				cout << "Please enter your custom sequence" << endl;
				cin >> ruleset;

				while(validSeq(ruleset) || ruleset.length() != 8){
					cin.clear();
					cin.ignore();
					cout << "Please enter a valid sequence" << endl;
					cin >> ruleset;
				}
			}

			if(writeFile){
				ofstream ofile;
				ofile.open(writeFileName.c_str());
				ofile << "Rule set: " << ruleset << " | Wrap side: ";
				if(wrap){
					ofile << "true" << endl;
				}else{
					ofile << "false" << endl;
				}
				ofile.close();
			}

			//Initialise Arrays
			InitGenerations();
			//Gen sequences
			if(randGen){
				RandGeneration();
			}else if(defaultGen){
				if((genSize % 2) == 1){
					parentGen[((genSize-1)/2)+1] = '1';
				}else{
					parentGen[genSize/2] = '1';
				}
			}else if(customGen){
				for(int i = 0; i < genSize; i++){
					parentGen[i] = customSeq.at(i);
				}
			}

			StartAutomaton();
			return 0;
		}else if(string(argv[argIndex]) == "-db" || string(argv[argIndex]) == "--decimalbin"){
			//TODO

		}else if(string(argv[argIndex]) == "-bd" || string(argv[argIndex]) == "--bindecimal"){
			//TODO

		}else{
			cout << USAGE << endl;
			return 1;
		}
	}

	return 0;
}

