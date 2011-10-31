#ifndef ORDERABLESTATESET_H
#define ORDERABLESTATESET_H
#include "StateSet.h"
#include "PQL/Contexts.h"

namespace PetriEngine { namespace Structures {
class OrderableStateSet : public StateSet{
public:
    OrderableStateSet(const PetriNet& net, PQL::MonotonicityContext& context):StateSet(net){
	_context = &context;
	_net = &net;
    }
    bool add(State* state){
	for(const_iter it = this->begin() ; it != this->end();it++){
	    if (leq(state,*it))
		return false;
	}
	std::pair<iter, bool> result = this->insert(state);
	return result.second;
    }

private:
    bool leq(State* s1, State* s2){

	for(size_t i = 0; i <  _net->numberOfPlaces(); i++){
	    if(_context->goodPlaces()[i])
		if(s1->marking()[i] > s2->marking()[i])
		    return false;
	    else
		if(s1->marking()[i] != s2->marking()[i])
		    return false;

	}

	for(size_t i = 0; i <  _net->numberOfVariables(); i++){
	    if(_context->goodVariables()[i])
		if(s1->valuation()[i] > s2->valuation()[i])
		    return false;
	    else
		if(s1->valuation()[i] != s2->valuation()[i])
		    return false;
	}

	return true;


    }
    const PetriNet* _net;
    PQL::MonotonicityContext* _context;
};

}}
#endif // ORDERABLESTATESET_H
