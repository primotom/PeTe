#include "DFSStateset.h"




bool DFSStateSet::add(State* state, unsigned int t){
	_countAdd++;

	for(std::list<Step*>::iterator it = _stack.begin() ; it != _stack.end();){
		if (less((*it).state, state)){
			_visited.remove(*it++);
			_countSkip++;
		}else
			it++;
	}

	for(std::list<State*>::iterator it = _visited.begin() ; it != _visited.end();){
		if (less(*it, state)){
			_visited.remove(*it++);
		}else{
			if(leq(state,*it)){
				_countSkip++;
				return false;
			}
			it++;
		}
	}

	_stack.back().t =  t+1;
	if(_mode == PetriEngine::Structures::ModeGraterBool){

		if(greaterBool(state,stack.back().state))
			stack.push_back(Step(ns,0));
		else
			stack.push_front(Step(ns, 0));

	}else if(){

		if(greater(state,stack.back().state))
			stack.push_back(Step(ns,0));
		else
			stack.push_front(Step(ns, 0));

	}else{
		_stack.push_back(Step(state,0));
	}

	_visited.push_back(state);

	//we are adding to wating




	return true;
}


void writeStatistics(){
	std::cout<<"Number of call to add: "<<_countAdd<<std::endl;
	std::cout<<"Number of states skipped: "<<_countSkip<<std::endl;
	std::cout<<"Number of states not skipped: "<<_countAdd-_countSkip<<std::endl;
	std::cout<<"Final size of visited and waiting: "<<_stack.size()+ _visited.size()<<std::endl;
}
