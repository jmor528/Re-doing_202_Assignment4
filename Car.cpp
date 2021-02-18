#include "Car.h"

Car::Car(int id, int pos){
	this->id = id;
	this->position = pos;
  this->hasMoved = false;
}

int Car::get_id()
{
  return id;
}

int Car::get_position()
{
	return position;
}

void Car::set_position(int pos)
{
	this->position = pos;
}

bool Car::get_hasMoved()
{
  return hasMoved;
}

void Car::set_hasMoved(bool move) 
{
  this->hasMoved = move;
}

void Car::set_next(Car* v)
{
  next = v;
}

void Car::set_prev(Car* v)
{  
  prev = v;
}

Car* Car::get_next()
{
  return next;
}

Car* Car::get_prev()
{
  return prev;
}
