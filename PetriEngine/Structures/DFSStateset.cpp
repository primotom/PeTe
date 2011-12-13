

#include "DFSStateset.h"
using namespace std;
namespace PetriEngine { namespace Structures {


bool DFSStateSet::add(State* state, unsigned int t){
	_countAdd++;

	bool skipVisited = false;
	for(std::list<Step>::iterator it = _stack.begin() ; it != _stack.end();){
		if (less((*it).state, state)){
			_stack.erase(it++);
			skipVisited = true;
		} else {
			if(this->leq(state, (*it).state))
				return false;
			it++;
		}
	}
	// Ensure we haven't been visited, or are smaller than or
	//  equal any states in visited
	if(!skipVisited){
		for(std::list<State*>::iterator it = _states.begin() ; it != _states.end();){
			if (less(*it, state)){
				_states.erase(it++);
				this->_countRemove++;
			}else{
				if(leq(state,*it)){
					_countSkip++;
					return false;
				}
				it++;
			}
		}
	}



	_stack.back().t = t + 1;

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

	return true;
}

Step DFSStateSet::getNextStep(){
	Step retState = _stack.back();

	return retState;
}

size_t DFSStateSet::waitingSize(){
	return _stack.size();
}

Step DFSStateSet::popWating(){
	Step temp = _stack.back();
	_stack.pop_back();
	return temp;
}

void writeStatistics(){
	//std::cout<<"Number of call to add: "<<_countAdd<<std::endl;
	//std::cout<<"Number of states skipped: "<<_countSkip<<std::endl;
	//std::cout<<"Number of states not skipped: "<<_countAdd-_countSkip<<std::endl;
	//std::cout<<"Final size of visited and waiting: "<<_stack.size()+ _visited.size()<<std::endl;
}

}}
