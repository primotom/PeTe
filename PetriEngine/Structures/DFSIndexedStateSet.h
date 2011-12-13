#ifndef DFSINDEXEDSTATESET_H
#define DFSINDEXEDSTATESET_H


#include "PQL/Contexts.h"
#include "Structures/OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

enum Mode{
	ModeNormal,
	ModeGraterBool,
	ModeGraterState
};
/** A step in the reachability search */
struct Step{
	Step(Structures::State* s, unsigned int t){
		state = s;
		this->t = t;
	}

	Structures::State* state;
	bool operator==(const Step& rhs) {return state==rhs.state;}
	unsigned int t;
};

class DFSIndexedStateSet : public OrderableStateSet {
public:
	DFSIndexedStateSet(const PetriNet& net, PQL::MonotonicityContext* context, bool variance = true, Mode mode = ModeNormal)
		: OrderableStateSet(net, context){
		_variance = variance;
		_mode = mode;
	}

	bool add(State* state){}
	bool add(State* state, unsigned int t);
	Step getWating(){return _stack.back();}
	Step popWating();

	virtual State* getNextState(){return NULL;}
	Step getNextStep();
	int waitingSize();
	void printWaiting();
	void printVisited();

private:
	Mode _mode;

	typedef std::pair<int, State*> IndexedState;
	typedef std::list<IndexedState>::iterator iter;

	std::list<Step> _stack;

	std::list<IndexedState> _visited;

	bool equal(State* s1, State* s2);

	bool _variance;
};
}}

#endif // DFSINDEXEDSTATESET_H
