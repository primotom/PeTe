#ifndef INDEXEDRFSSTATESET_H
#define INDEXEDRFSSTATESET_H

#include "PQL/Contexts.h"
#include "OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

/** Indexed State Set class which will feed states in a random depth first manner */
class IndexedRFSStateSet : public OrderableStateSet {
public:
	IndexedRFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context, bool variance = true)
		: OrderableStateSet(net, context) {
		_variance = variance;
	}

	/** Add a state to the state set */
	bool add(State* state);

	/** Get the next state to explore */
	State* getNextState();

	/** Get size of waiting */
	size_t waitingSize();

	/** Push a state to waiting without checking */
	void pushToWaiting(State* state);

private:
	typedef std::pair<int, State*> IndexedState;
	typedef std::list<IndexedState>::iterator visitedIter;
	typedef std::list<State*>::iterator waitingIter;

	std::list<State*> _waiting;
	std::list<IndexedState> _visited;

	bool _variance;
};

} // Structures
} // PetriEngine

#endif // INDEXEDRFSSTATESET_H
