#include "SimulationControl.h"

int main(int argc, const char * argv[])
{
	SimulationControl* controller = new SimulationControl();
	
	//run the simulation
	controller->setFlag((argc > 1) ? argv[1] : "");
	controller->run();
	
	delete controller;
	
	return 0;
}


