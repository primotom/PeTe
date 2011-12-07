#ifndef INDEXEDSTATESET_H
#define INDEXEDSTATESET_H

#include "PQL/Contexts.h"
#include "Structures/OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

class IndexedStateSet : OrderableStateSet {
public:
	IndexedStateSet(const PetriNet& net, PQL::MonotonicityContext* context)
		: OrderableStateSet(net, context){}

	bool add(State* state){
		_countAdd++;

		bool insert = true;
		if(_states.empty())
			_states.push_back(state);

		for(std::list<State*>::reverse_iterator it = _states.rbegin() ; it != _states.rend();){
			std::cerr<<"start for"<<std::endl;
			if(state->stateIndex(*_net) <= (*it)->stateIndex(*_net)){
				std::cerr<<"index leq"<<std::endl;
				if(leq(state, *it))
					return false;
				else{
					it--;
					std::cerr<<"continue"<<std::endl;
					continue;
				}
			}
			else{
				if (insert){
					++it;
					_states.insert(it,state);
					--(--it);
					insert = false;
					std::cerr<<"done insert"<<std::endl;
				}
				if(leq(*it,state)){
					std::cerr<<"remove"<<std::endl;
					_states.remove((*it++));
					std::cerr<<"done remove"<<std::endl;
				}
				else{
					std::cerr<<"--"<<std::endl;
					it++;
					std::cerr<<"done --"<<std::endl;
				}
			}
			std::cerr<<"hest"<<std::endl;

		}
		return true;

		/*
		for(std::list<State*>::iterator it = _states.begin() ; it != _states.end();){
			if (less((*it), state)){
				_states.remove(*it++);
				_countSkip++;
			}else{
				if(leq(state,*it)){
					_countSkip++;
					return false;
				}
				it++;
			}
		}
		_states.push_back(state);
		return true;
		*/

	}

};
}
}

#endif // INDEXEDSTATESET_H
