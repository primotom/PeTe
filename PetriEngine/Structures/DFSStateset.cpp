#include "DFSStateset.h"

bool DFSStateSet::add(State* state){
	_countAdd++;
	for(std::list<State*>::iterator it = _states.begin() ; it != _states.end();){
		if (less(*it, state)){
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
}


bool DFSStateSet::greater(State* s1, State* s2){
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


bool DFSStateSet::greaterBool(State* s1, State* s2){
	for(size_t i = 0; i <  _net->numberOfBoolVariables(); i++){
		if(_context->goodBoolVariables()[i]) {
			if(s1->boolValuation()[i] && (!s2->boolValuation()[i]))
				return true;
		}
	}
	return false;
}

bool DFSStateSet::leq(State* s1, State* s2){
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
bool DFSStateSet::less(State* s1, State* s2){
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


void writeStatistics(){
	std::cout<<"Number of call to add: "<<_countAdd<<std::endl;
	std::cout<<"Number of states skipped: "<<_countSkip<<std::endl;
	std::cout<<"Number of states not skipped: "<<_countAdd-_countSkip<<std::endl;
	std::cout<<"Final size of visited and waiting: "<<_states.size()<<std::endl;
}
