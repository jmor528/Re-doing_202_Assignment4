#ifndef _Platoon_H_
#define _Platoon_H_

#include <vector>
#include <string>
#include <sstream>
#include "Car.h"

using namespace std;

class Platoon
{
    private:
        Car* head = NULL;
        Car* tail = NULL;

    public:
        Platoon(string init);
        Car* get_tail() const;
        Car* get_head() const;

        // helper functions
        int size() const;
        void remove(Car* c); 
        void append(Car* c); 
        void prepend(Car* c); 
        void insert(Car* c);
        Car* getCar(int numList) const;
        

};
#endif
