#include "Platoon.h"
#include <iostream>

using namespace std;

Platoon::Platoon(string init)
{
    // the string 'init' looks like:
    // a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
    stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

            if (head == NULL)
            {
                head = car;
                tail = car;
            }
            else
            {
                tail->set_next(car);
                car->set_prev(tail);
                tail = car;
                car->set_next(NULL);
            }
		}
	}
}

Car* Platoon::get_tail() const
{
	return tail;
}

Car* Platoon::get_head() const
{
	return head;
}

int Platoon::size() const{
	int i=0;
	Car *t;
	t=head;
	//Iterate until the end of the list to
	//determine the size
	while(t!=0){
		i++;
		t = t->get_next();
	}
	return i;
}

void Platoon::remove(Car* c) 
{
  // need to search platoon for car to remove
  int ID = c->get_id();
  Car* probe = head;
  
  if (this->size() == 1 && ID == probe->get_id()) {
    delete c;
    c = NULL;
    cout << "removing" << endl;
    return;
  }

  while(ID != probe->get_id() && tail->get_id() != probe->get_id()) {
   probe = probe->get_next();
  }
  
  if (ID == probe->get_id()) {

    if (ID == head->get_id()) {
      // removing head
      probe = probe->get_next();
      probe->set_prev(0);
      head = probe;

      c->set_next(0);
      delete c;
      c = NULL;

    } else if (ID == tail->get_id()) {
      // removing tail
      probe = probe->get_prev();
      probe->set_next(0);
      tail = probe;

      c->set_prev(0);
      delete c;
      c = NULL;

    } else {
      probe = probe->get_prev();

      probe->set_next(c->get_next());
      probe = probe->get_next();
      probe->set_prev(c->get_prev());
      
      c->set_next(0);
      c->set_prev(0);
      delete c;
      c = NULL;

    }
   
    cout << "removing" << endl;
  }

}

void Platoon::append(Car* c) 
{
  Car* newCar = new Car(c->get_id(),c->get_position());
  tail->set_next(newCar);
  newCar->set_prev(tail);
  tail = newCar;

}

void Platoon::prepend(Car* c) 
{
  Car* newCar = new Car(c->get_id(),c->get_position());
  newCar->set_next(head);
  head->set_prev(newCar);
  head = newCar;

}

void Platoon::insert(Car* c) 
{
  int position = c->get_position();
  int headPos = head->get_position();
  int tailPos = tail->get_position();

  if (position > headPos) {
    prepend(c);
  } else if (position < tailPos) {
    append(c);
  } else if (position < headPos && position > tailPos) {
    // inserting between two cars
    Car* probe = head;
    while (probe->get_position() > position) {
      // moving down the list to insertion site
      probe = probe->get_next();
    }
    probe = probe->get_prev();

    Car* newCar = new Car(c->get_id(),position);
    newCar->set_next(probe->get_next());
    probe->set_next(newCar);

    newCar->set_prev(probe);
    probe = newCar->get_next();
    probe->set_prev(newCar);
  }

}