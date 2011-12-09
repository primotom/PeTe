#ifndef DFSSTATESET_H
#define DFSSTATESET_H


#include "PQL/Contexts.h"
#include <list>

namespace PetriEngine { namespace Structures {

class DFSStateSet {
public:
	DFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context){
	_context = context;
	_net = &net;
	_countSkip =0;
	_countAdd =0;
	}

	bool add(State* state);

	//Is some good boolean variable or marking in s1 greater than the one in s2, where true > false
	bool greater(State* s1, State* s2);

	//Is some good boolean variable in s1 greater than the one in s2, where true > false
	bool greaterBool(State* s1, State* s2);

	std::list<State*> States() {return _states;}

	void writeStatistics();

protected:
	//Is S1 less or equal to S2
	bool leq(State* s1, State* s2);

	//Is S1 less than S2
	bool less(State* s1, State* s2);

	const PetriNet* _net;
	PQL::MonotonicityContext* _context;
	std::list<State*> _states;

	int _countSkip;
	int _countAdd;
};

}}

#endif // DFSSTATESET_H
