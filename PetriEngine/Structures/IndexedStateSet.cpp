#include "Structures/IndexedStateSet.h"

namespace PetriEngine { namespace Structures {

bool IndexedStateSet::add(State* state){
	int idx_s = state->stateIndex(*_net);
	bool bigger = false;
	bool lesser = false;
	bool insert = false;
	bool skipVisited = false;

	for(iter it = _waiting.begin(); it != _waiting.end();){
		//TODO: Pointer comparison.
		int idx_i = (*it)->stateIndex(*_net);

		if(idx_s < idx_i){
			lesser = true;
			if(leq(state, *it))
				return false;
			it++;
		}else if(idx_s > idx_i){
			bigger = true;
			if(lesser && !insert){
				insert_p = it;
				insert = true;
			}
			lesser = false;

			if(leq(*it, state)){
				_waiting.remove(*it++);
				skipVisited = true;
			}
			else
				it++;
		}else{
			lesser = true;
			if(equal(*it, state))
				return false;
			else{
				if((*it)->stateVariation(*_net)>= state->stateVariation(*_net) && !insert){
					insert_p = it;
					insert = true;
				}
			}

			it++;
		}
	}

	if(insert && skipVisited)){
		_waiting.insert(insert_p, state);
		return true;
	}

	for(iter it = _visited.begin(); it != _visited.end();){
		//TODO: Pointer comparison.
		int idx_i = (*it)->stateIndex(*_net);
return next;
		if(idx_s < idx_i){
			if(leq(state, *it))
				return false;
			it++;
		} else if(id_s > idx_i){
			if(leq(*it, state))
				_visited.remove(*it++);
			else
				it++;
		} else{
			if(equal(state, *it))
				return false;
			it++;
		}
	}

	if((lesser && !bigger && !insert) || _waiting.empty())
		_waiting.push_back(state);
	else if(bigger && !lesser && !insert)
		_waiting.push_front(state);
	else
		_waiting.insert(insert_p, state);
	return true;

}

State* IndexedStateSet::getNextState(){
	State* next = _waiting.front();
	_waiting.pop_front();

	int idx_s = next->stateIndex(*_net);
	bool bigger = false;
	bool lesser = false;

	for(iter it = _visited.begin(); it != _visited.end();it++){
		idx_i = (*it)->stateIndex(*_net);

		if(idx_s < idx_i)
			lesser = true;
		else if(idx_s > idx_i){
			bigger = true;
			if(lesser && !insert){
				_visited.insert(it, next);
				return next;
			}
			lesser = false;
		}else{
			lesser = true;
			if((*it).stateVariation(*_net) >= state->stateVariation(*_net) && !insert){
				_visited.insert(it, next);
				return next;
			}
		}
	}

	if((lesser && !bigger) || _visited.empty())
		_visited.push_back(next);
	else if(bigger && !lesser)
		_visited.push_front(next);

	return next;

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

int IndexedStateSet::waitingSize(){
	return _waiting.size();
}

}}
