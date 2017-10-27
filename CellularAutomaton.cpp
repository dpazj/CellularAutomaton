//Joe Riemersma and Claire Sommerville
//Cellular Automation 
//18/10/17

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <bitset>
#include <string>
#include <unistd.h>
#include "LifeGrid.h"

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
		if(rand() % 2 == 0){			//generates random number
			parentGen[i] = '0'; 
		}else{
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
		if(arrayToPrint[i] == '0'){
			cout << char(32);
		}else{
			cout << char(42); 
		}
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
			left = parentGen[genSize - 1];
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
	string s(argv);

	//Taken from first answer https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int 
	//user paercebal
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    char * p ;
    strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}


void RunGameOfLife(int preset){
	unsigned int sleeptime = 50000;
	int evolutions = 2000; //number of evolutions

	Grid g(200,50);

	if(preset == 0){
		g.InitialGeneration();
	}else if(preset == 1){
		g.GenerateGlider();
	}
	g.DisplayGrid();

	usleep(sleeptime);
	for(int i = 0; i < evolutions; i++) {
			g.Generation();
			g.DisplayGrid();
			usleep(sleeptime);	
	}
}

int main(int argc, char* argv[]){ //Processes command line arguments
	string USAGE("Usage: ./CA [-a] [-gameoflife] \nUse command ./CA -help for more information.");
	string AUTOMATON("Usage: ./CA -a [seqinput=<'sequenceSize'> | seqdefault=<'sequenceSize' | seqcustom]	[rulesetinput=<'ruleset'> | rulesetdefault | rulesetcustom] 	[iterationall=<'repetitions' | iterationsingl=<'repetitions'>]	 [-s=<'outputfile'>] [-w] \nUse command ./CA -help for more information.");
	string GOLIFE("Usage: ./CA -gameoflife [random] [glider]");

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
				cout << AUTOMATON << endl;
				return 1;
			}else if(string(argv[argIndex]) == "seqinput"){
				argIndex++;
				if(argIndex >= argc){cout << "Usage: ./CA -a seqinput 'sequenceSize'" << endl;return 1;}

				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 0 || atoi(argv[argIndex]) > 200){
						cout << "'seqinput' should be follwed by an integer between 0 and 200" << endl;
						cout << "Usage: ./CA -a seqinput 'sequenceSize'" << endl;
						return 1;
					}else{
						genSize = atoi(argv[argIndex]);
						randGen = true;
					}
				}else{ 
					cout << "Please follow 'seqinput' with an integer" << endl;
					cout << "Usage: ./CA -a seqinput 'sequenceSize'" << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "seqdefault"){
				argIndex++;
				if(argIndex >= argc){cout << "Usage: ./CA -a seqdefault 'sequenceSize'" << endl;return 1;}

				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 0 || atoi(argv[argIndex]) > 200){
						cout << "'seqdefault' should be follwed by an integer between 0 and 200" << endl;
						cout << "Usage: ./CA -a seqdefault 'sequenceSize'" << endl;
						return 1;
					}else{
						genSize = atoi(argv[argIndex]);
						defaultGen = true;
					}
				}else{ 
					cout << "Please follow 'seqdefault' with an integer" << endl;
					cout << "Usage: ./CA -a seqdefault 'sequenceSize'" << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "seqcustom"){
				customGen = true;
			}else{
				cout << AUTOMATON << endl;
				return 1;
			}

			//RULESET ARGUMENTS
			argIndex++;
			if(argIndex >= argc){
				cout << AUTOMATON << endl;
				return 1;
			}else if(string(argv[argIndex]) == "rulesetinput"){
				argIndex++;
				if(argIndex >= argc){cout << "Usage: ./CA -a 'seqoption' rulesetinput 'ruleset'" << endl;return 1;}
				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 0 || atoi(argv[argIndex]) > 255){
						cout << "rulesetinput' should be follwed by an integer between 0 and 255" << endl;
						cout << "Usage: ./CA -a 'seqoption' rulesetinput 'ruleset'" << endl;
						return 1;
					}else{
						bitset<8> a (atoi(argv[argIndex]));
						ruleset = a.to_string();
					}
				}else{ 
					cout << "Please follow 'rulesetinput' with an integer";
					cout << "Usage: ./CA -a 'seqoption' rulesetinput 'ruleset'" << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "rulesetdefault"){
				bitset<8> a (30);
				ruleset = a.to_string();
			}else if(string(argv[argIndex]) == "rulesetcustom"){
				customBitset = true;
			}else{
				cout << AUTOMATON << endl;
				return 1;
			}

			//ITERATION ARGUMENTS
			argIndex++;
			if(argIndex >= argc){
				cout << AUTOMATON << endl;
				return 1;
			}else if(string(argv[argIndex]) == "iterationall"){
				argIndex++;
				if(argIndex >= argc){cout << "Usage: ./CA -a 'seqoption' 'rulesetoption' iterationall 'iterations'" << endl;return 1;}
				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 1){
						cout << "iterationall' should be follwed by an integer greater than 0" << endl;
						cout << "Usage: ./CA -a 'seqoption' 'rulesetoption' iterationall 'iterations'" << endl;
						return 1;
					}else{
						iterations = atoi(argv[argIndex]);
					}
				}else{ 
					cout << "Please follow 'iterationall' with an integer";
					cout << "Usage: ./CA -a 'seqoption' 'rulesetoption' iterationall 'iterations'" << endl;
					return 1;
				}
			}else if(string(argv[argIndex]) == "iterationsingle"){
				singleline = true;
				argIndex++;
				if(argIndex >= argc){cout << "Usage: ./CA -a 'seqoption' 'rulesetoption' iterationsingle 'iterations'" << endl;return 1;}
				if(testInt(argv[argIndex])){
					if(atoi(argv[argIndex]) < 1){
						cout << "iterationsingle' should be follwed by an integer greater than 0" << endl;
						cout << "Usage: ./CA -a 'seqoption' 'rulesetoption' iterationsingle 'iterations'" << endl;
						return 1;
					}else{
						iterations = atoi(argv[argIndex]);
					}
				}else{ 
					cout << "Please follow 'iterationsingle' with an integer";
					cout << "Usage: ./CA -a 'seqoption' 'rulesetoption' iterationsingle 'iterations'" << endl;
					return 1;
				}
			}else{
				cout << AUTOMATON << endl;
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
					cout << "WORKS";
						wrap = true;
					
				}else{
					cout << "Command '" << string(argv[argIndex]) << "' not recognised\n" << AUTOMATON << endl;
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
		}else if(string(argv[argIndex]) == "-gameoflife"){
			argIndex++;
			if(argIndex >= argc){cout << GOLIFE << endl;return 1;}

			if(string(argv[argIndex]) == "random"){
				RunGameOfLife(0);
			}else if(string(argv[argIndex]) == "glider"){
				RunGameOfLife(1);
			}else{
				cout << GOLIFE << endl;
			}
	
		}else if(string(argv[argIndex]) == "-help"){
			string line;
			ifstream helpfile("helpfile");
			if(helpfile.is_open()){
				while(getline(helpfile,line)){
					cout << line << endl;
				}
				helpfile.close();
			}else{
				cout << "Unable to open helpfile, please make sure it has not been deleted or moved.";
			}
		}else{
			cout << USAGE << endl;
			return 1;
		}
	}

	return 0;
}

