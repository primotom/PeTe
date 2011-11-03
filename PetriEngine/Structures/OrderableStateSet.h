#ifndef ORDERABLESTATESET_H
#define ORDERABLESTATESET_H

#include "PQL/Contexts.h"
#include <list>

namespace PetriEngine { namespace Structures {

class OrderableStateSet {
public:
    OrderableStateSet(const PetriNet& net, PQL::MonotonicityContext& context){
	_context = &context;
	_net = &net;
    }
    bool add(State* state){

	for(std::list<std::pair<bool,State*> >::iterator it = _states.begin() ; it != _states.end();){
	    if (leq(state,(*it).second)){
		_states.remove(*it++);
	    }else{
		if(leq(state,(*it).second)){
		    return false;
		}
		it++;
	    }
	}
	;
	_states.push_back(std::make_pair(false,state));;
	return true;

    }

    void visit(State* state){
	for(std::list<std::pair<bool,State*> >::iterator it = _states.begin() ; it != _states.end();it++){
	    if ((*it).second == state){
		(*it).first = true;
		return;
	    }
	}
    }

private:
    bool leq(State* s1, State* s2){

	for(size_t i = 0; i <  _net->numberOfPlaces(); i++){
		if(_context->goodPlaces()[i]) {
			if(s1->marking()[i] > s2->marking()[i]){
				return false;
			}else{
				if(s1->marking()[i] != s2->marking()[i])
					return false;
			}
		}else{
			if(s1->marking()[i] != s2->marking()[i])
				return false;
		}

	}

	for(size_t i = 0; i <  _net->numberOfIntVariables(); i++){
		if(_context->goodVariables()[i]){
			if(s1->intValuation()[i] > s2->intValuation()[i]){
				return false;
			}else{
				if(s1->intValuation()[i] != s2->intValuation()[i])
					return false;
			}
		}else{
			if(s1->intValuation()[i] != s2->intValuation()[i])
				return false;
		}
	}

	for(size_t i = 0; i <  _net->numberOfBoolVariables(); i++){
		if(_context->goodBoolVariables()[i]) {
			if(s1->boolValuation()[i] && !s2->boolValuation()[i]){
				return false;
			}
		}else{
			if(s1->boolValuation()[i] != s2->boolValuation()[i])
				return false;
		}
	}

	return true;


    }
    const PetriNet* _net;
    PQL::MonotonicityContext* _context;
    std::list<std::pair<bool,State*> > _states; //true = visited //false = waiting
};

}}
#endif // ORDERABLESTATESET_H
