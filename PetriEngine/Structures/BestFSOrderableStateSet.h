#ifndef BESTFSORDERABLESTATESET_H
#define BESTFSORDERABLESTATESET_H

#include "OrderableStateSet.h"
#include "EnhancedPriorityQueue.h"
#include "PQL/Contexts.h"
#include <list>

namespace PetriEngine { namespace Structures {

/** Orderable State Set class which will feed states in a heuristc Best first manner */
class BestFSOrderableStateSet : public OrderableStateSet {
public:
	/** Create the state set */
	BestFSOrderableStateSet(const PetriNet& net, PQL::MonotonicityContext* context)
		: OrderableStateSet(net, context) {}

	/** Add a state to the state set */
	bool add(State *state);

	/** Get the next state to explore */
	State* getNextState;
private:


	EnhancedPriorityQueue<State*> _waiting;
	std::list<State*> _visited;
};

} // Structures
} // PetriEngine

#endif // BESTFSORDERABLESTATESET_H
