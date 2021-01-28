#ifndef _SIMULATION_CONTROL_H_
#define _SIMULATION_CONTROL_H_

#include <stdlib.h>
#include <string>
#include <vector>
#include "TrafficModel.h"
#include "SimulationUI.h"

class SimulationControl
{
	private:
		TrafficModel* model;
		SimulationUI* view;
		string flag;

		void simulationStep(int tick);

	public:
		SimulationControl();
		~SimulationControl();

		bool run(); // run the simulation
		void setFlag(string flag);
		void initialization();
};
#endif

