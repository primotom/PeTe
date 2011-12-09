

#include "DFSStateset.h"
namespace PetriEngine { namespace Structures {


bool DFSStateSet::add(State* state, unsigned int t){
	_countAdd++;
	Step* oldBack = &_stack.back();
	for(std::list<Step>::iterator it = _stack.begin() ; it != _stack.end();){
		if (less((*it).state, state)){
			_stack.remove(*it++);
			_countSkip++;
		}else
			it++;
	}

	for(std::list<State*>::iterator it = _states.begin() ; it != _states.end();){
		if (less(*it, state)){
			_states.remove(*it++);
		}else{
			if(leq(state,*it)){
				_countSkip++;
				return false;
			}
			it++;
		}
	}

	_stack.back().t =  t+1;
	if(_mode == PetriEngine::Structures::ModeGraterBool && state->parent()){

		if(greaterBool(state, state->parent()))
			_stack.push_back(Step(state,0));
		else
			_stack.push_front(Step(state, 0));

	}else if(_mode == PetriEngine::Structures::ModeGraterState && state->parent()){

		if(greater(state, state->parent()))
			_stack.push_back(Step(state,0));
		else
			_stack.push_front(Step(state, 0));

	}else{
		_stack.push_back(Step(state,0));
	}

	_states.push_back(state);

	//we are adding to wating




	return true;
}


void writeStatistics(){
	//std::cout<<"Number of call to add: "<<_countAdd<<std::endl;
	//std::cout<<"Number of states skipped: "<<_countSkip<<std::endl;
	//std::cout<<"Number of states not skipped: "<<_countAdd-_countSkip<<std::endl;
	//std::cout<<"Final size of visited and waiting: "<<_stack.size()+ _visited.size()<<std::endl;
}

}}
