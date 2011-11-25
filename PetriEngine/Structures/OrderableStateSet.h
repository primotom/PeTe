#ifndef ORDERABLESTATESET_H
#define ORDERABLESTATESET_H

#include "PQL/Contexts.h"
#include <list>

namespace PetriEngine { namespace Structures {

class OrderableStateSet {
public:
	OrderableStateSet(const PetriNet& net, PQL::MonotonicityContext* context){
	_context = context;
	_net = &net;
	_countSkip =0;
	_countAdd =0;
    }

	bool add(State* state){
		_countAdd++;
		for(std::list<std::pair<bool,State*> >::iterator it = _states.begin() ; it != _states.end();){
			//if (less((*it).second, state)){//TODO: Think about using this again
			//	_states.remove(*it++);
				//_countSkip++;
			//}else{
				if(leq(state,(*it).second)){
					_countSkip++;
					return false;
				}
				it++;
			//}
		}
		_states.push_back(std::make_pair(false,state));
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

	//Is some good boolean variable or marking in s1 greater than the one in s2, where true > false
	bool greater(State* s1, State* s2){
		for(size_t i = 0; i <  _net->numberOfPlaces(); i++){
			if(_context->goodPlaces()[i]) {
				if(s1->marking()[i] > s2->marking()[i]){
					return true;
				}
			}
		}

		for(size_t i = 0; i <  _net->numberOfBoolVariables(); i++){
			if(_context->goodBoolVariables()[i]) {
				if(s1->boolValuation()[i] && !s2->boolValuation()[i]){
					return true;
				}
			}
		}
		return false;
	}

	//Is some good boolean variable in s1 greater than the one in s2, where true > false
	bool greaterBool(State* s1, State* s2){
		for(size_t i = 0; i <  _net->numberOfBoolVariables(); i++){
			if(_context->goodBoolVariables()[i]) {
				if(s1->boolValuation()[i] && (!s2->boolValuation()[i]))
					return false;
			}
		}
		return false;
	}

	std::list<std::pair<bool,State*> > States() {return _states;}

	void writeStatistics(){
		std::cout<<"Number of states skipped: "<<_countSkip<<std::endl;
		std::cout<<"Number of states not skipped: "<<_countAdd-_countSkip<<std::endl;
		std::cout<<"Final size of visited and waiting: "<<_states.size()<<std::endl;
	}

private:
	//Is S1 less or equal to S2
    bool leq(State* s1, State* s2){
		for(size_t i = 0; i <  _net->numberOfPlaces(); i++){
			if(_context->goodPlaces()[i]) {
				if(s1->marking()[i] > s2->marking()[i])
					return false;
			}else if(s1->marking()[i] != s2->marking()[i])
				return false;
		}

		for(size_t i = 0; i <  _net->numberOfBoolVariables(); i++){
			if(_context->goodBoolVariables()[i]) {
				if(s1->boolValuation()[i] && (!s2->boolValuation()[i]))
					return false;
			}else if(s1->boolValuation()[i] != s2->boolValuation()[i])
				return false;
		}
		return true;
    }

	//Is S1 less than S2
	bool less(State* s1, State* s2){
		bool _less = false;
		for(size_t i = 0; i <  _net->numberOfPlaces(); i++){
			if(_context->goodPlaces()[i]) {
				if(s1->marking()[i] > s2->marking()[i])
					return false;
				else if(s1->marking()[i] < s2->marking()[i])
					_less = true;
			}else if(s1->marking()[i] != s2->marking()[i])
				return false;
		}

		for(size_t i = 0; i <  _net->numberOfBoolVariables(); i++){
			if(_context->goodBoolVariables()[i]) {
				if((s1->boolValuation()[i] && !s2->boolValuation()[i]))
					return false;
				else if(!s1->boolValuation()[i] && s2->boolValuation()[i])
					_less = true;
			}else if(s1->boolValuation()[i] != s2->boolValuation()[i])
				return false;
		}

		if(!_less)
			return false;
		return true;
	}

    const PetriNet* _net;
	PQL::MonotonicityContext* _context;
    std::list<std::pair<bool,State*> > _states; //true = visited //false = waiting

	int _countSkip;
	int _countAdd;
};

}}
#endif // ORDERABLESTATESET_H
