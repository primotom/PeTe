#ifndef INDEXEDSTATESET_H
#define INDEXEDSTATESET_H

#include "PQL/Contexts.h"
#include "Structures/OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

class IndexedStateSet : public OrderableStateSet {
public:
	IndexedStateSet(const PetriNet& net, PQL::MonotonicityContext* context, bool variance = true)
		: OrderableStateSet(net, context){
		_variance = variance;
	}

	bool add(State* state);
	State* getNextState();
	int waitingSize();
	void printWaiting();
	void printVisited();

private:
	typedef std::pair<int, State*> IndexedState;
	typedef std::list<IndexedState>::iterator iter;

	std::list<IndexedState> _waiting;
	std::list<IndexedState> _visited;

	bool equal(State* s1, State* s2);

	bool _variance;
};
}}

#endif // INDEXEDSTATESET_H
