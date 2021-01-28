#include "SimulationUI.h"

SimulationUI::SimulationUI(string filename)
{
	this->filename = filename;

	ifstream infile (filename.c_str());
	if (!infile.good()){
		cout << "The input.txt file cannot be found" << endl;
		exit(0);
	}

	cout << "Simulation has started" << endl;
}

int SimulationUI::getMaxSimulationTime()
{
	string line;
	ifstream infile (filename.c_str());
	getline(infile, line);
	infile.close();

	return stoi(line);
}

vector<string> SimulationUI::getInitInfo()
{
	vector<string> output;
	string line;
	ifstream infile (filename.c_str());
	getline(infile, line); // ignore the first line

	while (getline(infile, line))
	{
		std::size_t found = line.find("!");
		if (found!=std::string::npos) break;
		output.push_back(line);
	}

	infile.close();

	return output;
}

vector<string> SimulationUI::getCommands(int tick)
{
	string line;
	ifstream infile (filename.c_str());
	// skip these lines, until "!" is found
	while (getline(infile, line))
	{
		std::size_t found = line.find("!");
		if (found!=std::string::npos) break;
	}

	// now, we read the command lines
	vector<string> commands;

	while(getline(infile, line)){
		string command_time = split(line, ',')[0];
		stringstream ss(command_time);
		int cmd_time;
		ss >> cmd_time;
		if (cmd_time == tick) {
			size_t found = line.find(',');
			string sub = line.substr(found+1, line.size()-1);
			commands.push_back(sub);
		}
	}

	return commands;
}

void SimulationUI::writeResult(int tick, vector<string> state)
{
	// create the final string
	int count = state.size();
	ostringstream out;
	out << tick;
	for (int i = 0; i < count; i++){
		out << state[i];
	}

	if (tick == 0){
		ofstream result;
		result.open("result.txt", ios::out | ios::trunc );
		result << out.str() << endl;
		result.close();
	}
	else{
		ofstream result;
		result.open("result.txt", ios::out | ios::app );
		result << out.str() << endl;
		result.close();
	}
	cout << out.str() << endl;
}

void SimulationUI::displayResult(int tick, vector<vector<vector<int>>> cars)

{
	//Find max number of columns (pos)
	int maxPos = 0;
	for(unsigned int i = 0; i < cars.size(); i++){
		for(unsigned int j = 0; j < cars[i].size(); j++){
			vector<int> carData = cars[i][j];
			int carPos = carData[2];
			if (carPos>= maxPos){
				maxPos = carPos;
			}
		}
	}

	//Cap draw-size to 11 columns
	if(maxPos > 10) {
		return;
	}

	//For the total number of rows
	//Draw the ascii line-by-line (7 high)
	//For each column (pos), check if there is a car there and draw car/space line.
	for(unsigned int row = 0; row < cars.size(); row++) {
		for(int line = 0; line < 7; line++){
			for(int col = 0; col <= maxPos; col++){
				bool carFound = false;
				for(unsigned int i = 0; i<cars[row].size();i++){
					vector<int> carData = cars[row][i];
					int carPos = carData[2];
					if(carPos==col){
						printCar(line,carData);
						carFound = true;
						break;
					} 
				}
				if(!carFound){
					printSpace(line);
				}
			}
			cout << "\n";
		}
	}
	cout << "\n";
}

void SimulationUI::printCar(int line,vector<int> carData){

	//Assign left/right indicator style on/off
	string left = (carData[3] == 1) ? "*" : "0";
	string right = (carData[3] == 2) ? "*" : "0";

	//print space with car inside
	switch(line){
	case 0:
		cout << "|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|" << flush;
		break;

	case 1:
		cout << "| " << ColorText("|‾‾‾‾‾‾‾‾‾‾‾"+left+"|", YELLOW) << "  |"<< flush;
		break;

	case 2:
		cout << "| " << ColorText("|"+FixedLength("id: "+to_string(carData[0]),12)+"|",YELLOW) << "  |" << flush;
		break;

	case 3:
		cout << "| " << ColorText("|"+FixedLength("lane: "+to_string(carData[1]),12)+"|",YELLOW) << "  |" << flush;
		break;

	case 4:
		cout << "| " << ColorText("|"+FixedLength("pos: "+to_string(carData[2]),12)+"|",YELLOW) << "  |" << flush;
		break;

	case 5:
		cout << "| " << ColorText("|___________"+right+"|", YELLOW) << "  |" << flush;
		break;

	case 6:
		cout << "|_________________|"<< flush;
		break;

	default:
		break;
	}
}

void SimulationUI::printSpace(int line){

	//print space with no car
	switch(line){
	case 0:
		cout << "|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|" << flush;
		break;

	case 1:
		cout << "|                 |" << flush;
		break;

	case 2:
		cout << "|                 |" << flush;
		break;

	case 3:
		cout << "|                 |" << flush;
		break;

	case 4:
		cout << "|                 |" << flush;
		break;

	case 5:
		cout << "|                 |" << flush;
		break;

	case 6:
		cout << "|_________________|" << flush;
		break;

	default:
		break;
	}
}

string SimulationUI::ColorText(string s, COLOR color)
{
	string temp;
	switch (color) {
	case RED: temp = "\033[1;31m" + s + "\033[0m"; break;
	case BLUE: temp = "\033[1;34m" + s + "\033[0m"; break;
	case YELLOW: temp = "\033[1;33m" + s + "\033[0m"; break;
	case GREEN: temp = "\033[1;32m" + s + "\033[0m"; break;
	case MAGENTA: temp = "\033[1;35m" + s + "\033[0m"; break;
	case WHITE: temp = "\033[1;37m" + s + "\033[0m"; break;
	default: temp = "\033[0m" + s; break;
	}
	return temp;
}

//Return string followed by empty spaces to fit required length
string SimulationUI::FixedLength(string str, int length){
	int numSpaces = length - str.length();
	return str + string(numSpaces,' ');
}

vector<string> SimulationUI::split(string line, char delimeter){
	stringstream ss(line);
	string segment;
	vector<string> list;
	while(getline(ss, segment, delimeter))
	{
		list.push_back(segment);
	}
	return list;
}

