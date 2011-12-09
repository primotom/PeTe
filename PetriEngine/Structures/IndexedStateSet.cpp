#include "Structures/IndexedStateSet.h"

namespace PetriEngine { namespace Structures {

bool IndexedStateSet::add(State* state){
	int idx = state->stateIndex(*_net);
	bool foundSomething = false;
	bool bigger = false;
	bool lesser = false;

	for(std::list<State*>::reverse_iterator it = _states.rbegin(); it != _states.rend();){
		//TODO: Pointer comparison
		int it_idx = (*it)->stateIndex(*_net);

		if(it_idx > idx){
			bigger = true;
			if(leq(state,*it)){
				return false;
			}
		}else if(it_idx < idx){
			lesser = true;
			if(bigger && !foundSomething){
				_states.insert(it.base(), state);
				foundSomething = true;
				bigger = false;
				it++;
			}
			if(less(*it, state)){
				_states.remove(*it++);
			}
		}else if(it_idx == idx){
			bigger = true;
			if(equal(*it, state)){
				return false;
			}
			else{
				if((*it)->stateVariation(*_net) >= state->stateVariation(*_net) && !foundSomething){
					_states.insert(it.base(), state);
					foundSomething = true;
					it++;
				}
			}
		}
		it++;
	}

	if(foundSomething){
		return true;
	}
	if(bigger && !lesser){
		_states.push_front(state);
		return true;
	}
	if(_states.empty() || (lesser && !bigger)){
		_states.push_back(state);
		return true;
	}
	return false;
}

bool IndexedStateSet::equal(State* s1, State* s2){
	for(uint i= 0; i<_net->numberOfPlaces(); i++)
		if(s1->marking()[i]!= s2->marking()[i])
			return false;
	for(uint i= 0; i<_net->numberOfBoolVariables(); i++)
		if(s1->boolValuation()[i]!= s2->boolValuation()[i])
			return false;
	return true;
}

}}
