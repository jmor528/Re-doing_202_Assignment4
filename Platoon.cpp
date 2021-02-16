#include "Platoon.h"

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

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

void Platoon::remove(Car* c) 
{
  // need to search platoon for car to remove
  ID = c->get_id();
  Car probe = head;

  while(ID != probe->get_id() && tail->get_id() != probe->get_id()) {
   probe = probe->get_next();
  }
  if (ID == probe->get_id()) {
    
    probe = probe->get_prev();
    probe->set_next(c->get_next());
    probe = probe->get_next();
    probe->set_prev(c->get_prev());

    c->set_next(0);
    c->set_prev(0);
    delete c;
    c = NULL;

  }


}

void Platoon::append(Car* c) 
{
  newCar = new Car(c->get_id(),c->get_position());
  tail->next = newCar;
  newCar->set_prev(tail);
  tail = newCar;

}

void Platoon::prepend(Car* c) 
{
  newCar = new Car(c->get_id(),c->get_position());
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
    Car probe = head;
    while (probe->get_position() > position) {
      // moving down the list to insertion site
      probe = probe->get_next();
    }
    probe = probe->get_prev();

    newCar = new Car(c->get_id(),position);
    newCar->set_next(probe->get_next());
    probe->set_next(newCar);

    newCar->set_prev(probe);
    probe = newCar->get_next();
    probe->set_prev(newCar);
  }

}