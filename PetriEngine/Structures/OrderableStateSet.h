#ifndef ORDERABLESTATESET_H
#define ORDERABLESTATESET_H
#include "StateSet.h"


namespace PetriEngine { namespace Structures {
class OrderableStateSet : public StateSet{
public:
    OrderableStateSet(const PetriNet& net):StateSet(net){}
    OrderableStateSet(unsigned int places, unsigned int variables):StateSet(places,variables){}

    bool add(State* state){

        std::cerr<<"debug test"<<std::endl;
    }
private:
        bool less(State* s1, State* s2);
};

}}
#endif // ORDERABLESTATESET_H
