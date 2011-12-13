#ifndef INDEXEDBFSSTATESET_H
#define INDEXEDBFSSTATESET_H

#include "PQL/Contexts.h"
#include "Structures/OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

class IndexedBFSStateSet : public OrderableStateSet {
public:
	IndexedBFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context, bool variance = true)
		: OrderableStateSet(net, context){
		_variance = variance;
	}

	bool add(State *state);
	State* getNextState();
	size_t waitingSize();

private:
	typedef std::pair<int, State*> IndexedState;
	typedef std::list<IndexedState>::iterator iter;

	std::list<IndexedState> _waiting;
	std::list<IndexedState> _visited;

	bool _variance;
};


} // Structures
} // PetriEngine

#endif // INDEXEDBFSSTATESET_H
