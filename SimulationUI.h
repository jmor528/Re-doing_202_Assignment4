#ifndef _SIMULATION_UI_H
#define _SIMULATION_UI_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

enum COLOR {RED=0, BLUE, GREEN, WHITE, YELLOW, MAGENTA};

class SimulationUI
{
	private: 
		string filename;

	public:
		SimulationUI(string filename);
		~SimulationUI();

		int getMaxSimulationTime(); // 
		vector<string> getInitInfo(); //
		vector<string> getCommands(int tick); 
		void writeResult(int tick, vector<string> state);
		void displayResult(int tick, vector<vector<vector<int>>> cars);
		void printCar(int line, vector<int> carData);
		void printSpace(int line);
		string ColorText(string s, COLOR color);
		string FixedLength(string str, int length);
		vector<string> split(string line, char c);
};

#endif 

