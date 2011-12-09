#ifndef DFSSTATESET_H
#define DFSSTATESET_H


#include "PQL/Contexts.h"
#include <list>
#include "OrderableStateSet.h"
#include "State.h"
namespace PetriEngine { namespace Structures {


		enum Mode{
			ModeNormal,
			ModeGraterBool,
			ModeGraterState
		};

class DFSStateSet : public OrderableStateSet {
public:
	DFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context, Mode mode):
	OrderableStateSet(net,context){
		_mode = mode;
	}

	bool add(State* state){}

	bool add(State* state, unsigned int t);



	/** A step in the reachability search */
	struct Step{
		Step(Structures::State* s, unsigned int t){
			state = s;
			this->t = t;
		}
		Structures::State* state;
		unsigned int t;
	};


	Step popWating(){Step temp = _stack.back();
					_stack.pop_back();
					return temp;}
	Step getWating(){return _stack.back();}

	std::list<Step> Wating() {return _stack;}

	virtual State* getNextState(){return NULL;}

	void writeStatistics();


protected:
	Mode _mode;
	//wating list
	std::list<Step> _stack;

	int _countSkip;
	int _countAdd;

};

}}

#endif // DFSSTATESET_H
