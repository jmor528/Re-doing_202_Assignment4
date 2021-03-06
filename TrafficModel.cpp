#include "TrafficModel.h"
#include <iostream>

using namespace std;

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){

  for (unsigned int i = 0; i < platoons.size(); i++) {
    // remove all the cars
    Platoon currentLane = platoons[i];
    int length = currentLane.size();

    for (int j = 0; j < length; j++) {
      currentLane.remove(currentLane.get_head());
    }

  }

 }

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}

/*
 * A helper function that will check in platoons for a position in a particular lane being vacant or not.
*/ 
bool TrafficModel::positionVacant(int lane, int pos)
{

  if (lane < 0 || lane >= (int)platoons.size()) {
    // illegal lane indexing
    return false;

  } else {
    //cout << pos;
    Platoon checkLane = platoons[lane];

    for (int i = 0; i < checkLane.size(); i++) {
      Car* currentCar = checkLane.getCar(i);

      if (currentCar->get_position() == pos) {
        // spot taken
        // cout << " false, position is " << currentCar->get_position() << endl;
        return false;
      }

    }

    //cout << " true" << endl;
    return true;
  }

}

/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{
	/*
   * Loop through the lanes (0 to n), each having a 
   * platoon of cars. For each car, get the lane change
   * command and update the car's position accordingly.
  */

  for (unsigned int i = 0; i < platoons.size(); i++) {
    Platoon Lane = platoons[i];
    Car* car = Lane.get_head();
    for (int j = 0; j < Lane.size(); j++) {
      // resetting each car
      car->set_hasMoved(false);
      car = car->get_next();
    }
  }

  for (unsigned int i = 0; i < platoons.size(); i++) {
    Platoon currentLane = platoons[i];
    int length = currentLane.size();
    
    for (int j = 0; j < length; j++) {
      // cars already removed from this lane
      int removed = length - currentLane.size();

      // current car to be checked
      Car* car = currentLane.getCar(j - removed);

      int id = car->get_id();
      int pos = car->get_position();

      if (!car->get_hasMoved()) {
        int command;
        command = this->get_lane_change_command(id);
        /*
        * 0: move forward
        * 1: turn left
        * 2: turn right
        */
    
        if (command == 2) {
          if (this->positionVacant(i+1, pos) ) {
            // can perform the lane change
            Platoon nextLane = platoons[i+1];
            Car* newCar = new Car(id,pos);
            cout << "check 1" << endl;

            nextLane.insert(newCar);
            currentLane.remove(car);

            newCar->set_hasMoved(true);

          } else {
            command = 0;
           
          }

        } else if (command == 1) {
          if (this->positionVacant(i-1, pos)) {
            // can perform the lane change
            Platoon nextLane = platoons[i-1];
            Car* newCar = new Car(id,pos);
            cout << "check 1" << endl;
            
            nextLane.insert(newCar);
            currentLane.remove(car);

            newCar->set_hasMoved(true);

          } else {
            command = 0;
          }

        }

        if (command == 0) {
          if (this->positionVacant(i, pos+1)) {
            // move forward
            car->set_position(pos+1);

            car->set_hasMoved(true);
        
          }

        }

      } 

    }

  }

}


/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon p = Platoon(info[i]);
		platoons.push_back(p);
	}
}

//
// IMPORTANT: DO NOT CHANGE THE FUNCTIONS BELOW THIS LINE
//

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i].get_tail();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					 "," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_prev();
		}

		output.push_back(out.str());
	}
	return output;
}

//Get the state of cars as a 3D vector representing car data in lane/pos
vector<vector<vector<int>>> TrafficModel::get_car_state(){

	vector<string> state = get_system_state();
	vector<vector<vector<int>>> cars;
	string remainingCars;
	string newCar;

	//Parse state into vector of car data
	for (unsigned int i = 0; i < state.size(); i++){
		vector<vector<int>> carRow;
		remainingCars = state[i];
		remainingCars.push_back(';');

		//Parse string of entire lane into individual car data
		while (remainingCars.size() > 1) {
			remainingCars = remainingCars.substr(1);
			size_t pos = remainingCars.find(";");
			newCar = remainingCars.substr(1,pos-2);

			carRow.push_back(parse_car_data(newCar));

			if(pos!=string::npos){
				remainingCars = remainingCars.substr(pos);
			} else {
				remainingCars = "";
			}
		}
		cars.push_back(carRow);
	}
	return cars;
}

//Parse string in form (id,lane,pos,turn) into vector of ints
vector<int> TrafficModel::parse_car_data (string& car){
	vector<int> carData;
	string delimiter = ",";
	size_t last = 0;
	size_t next = 0;
	int index = 0;

	while ((next = car.find(delimiter, last)) != string::npos) {
		carData.push_back(stoi(car.substr(last, next-last)));
		last = next + 1;
		index++;
	}
	carData.push_back(stoi(car.substr(last)));
	return carData;
}
