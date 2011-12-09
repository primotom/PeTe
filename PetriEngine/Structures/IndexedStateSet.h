#ifndef INDEXEDSTATESET_H
#define INDEXEDSTATESET_H

#include "PQL/Contexts.h"
#include "Structures/OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

class IndexedStateSet : public OrderableStateSet {
public:
	IndexedStateSet(const PetriNet& net, PQL::MonotonicityContext* context)
		: OrderableStateSet(net, context){}

	bool add(State* state);
	State* getNextState();
	int waitingSize();

private:
	typedef std::list<State*>::iterator iter;

	std::list<State*> _waiting;
	std::list<State*> _visited;

	bool equal(State* s1, State* s2);


};
}}

#endif // INDEXEDSTATESET_H
