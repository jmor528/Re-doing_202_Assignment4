#include "SimulationControl.h"

SimulationControl::SimulationControl()
{
	model = new TrafficModel();
	view = new SimulationUI("input.txt");
}

SimulationControl::~SimulationControl()
{
	// TODO: destructor
}

bool SimulationControl::run()
{
	//End execution if no command line flag exists
	if (flag.compare("-f")!=0 && flag.compare("-v")!=0) {
		cout << "Please use a command line argument: -f or -v" << endl;
		exit(-1);
	}

	// initialization
	int time = 0;
	int max_time = view->getMaxSimulationTime();
	initialization();

	while(time < max_time)
	{
		time++;
		if(flag.compare("-v")==0) do
		 {
		   cout << '\n' << "Press a key to view next tick...";
		 } while (cin.get() != '\n');

		simulationStep(time);
	}
	return true;
}

/*
 * A single step (tick) execution
 */
void SimulationControl::simulationStep(int tick)
{
	// read the commands, and update the system state
	model->update();

	// read the commands
	vector<string> commands = view->getCommands(tick);
	model->set_commands(commands);

	// get output and write to the result.txt file or also show visualisation
	vector<string> output = model->get_system_state();
	if (flag.compare("-f")==0) {
		view->writeResult(tick, output);
	}
	else if (flag.compare("-v")==0) {
		view->writeResult(tick, output);
		view->displayResult(tick, model->get_car_state());
	}

}

/*
 * Initialization
 */
void SimulationControl::initialization()
{
	// get the initial position
	vector<string> init = view->getInitInfo();
	model->initialize(init);
	// get the commands
	vector<string> commands = view->getCommands(0);
	model->set_commands(commands);
	// get the current state of the system
	vector<string> output = model->get_system_state();

	if (flag.compare("-f")==0) {
		view->writeResult(0, output); //Zero tick
	}
	else if (flag.compare("-v")==0) {
		view->writeResult(0, output);
		view->displayResult(0, model->get_car_state());
	}
}


void SimulationControl::setFlag(string flag){
	this->flag = flag;
}


