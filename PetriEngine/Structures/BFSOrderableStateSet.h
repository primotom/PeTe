#ifndef BFSORDERABLESTATESET_H
#define BFSORDERABLESTATESET_H

#include "PQL/Contexts.h"
#include "OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

/** Orderable State Set class which will feed states in a Breadth First manner */
class BFSOrderableStateSet : public OrderableStateSet {
public:
	/** Create the state set */
	BFSOrderableStateSet(const PetriNet& net, PQL::MonotonicityContext* context)
		: OrderableStateSet(net, context) {}

	/** Add a state to the state set */
	bool add(State* state);

	/** Get the next state to explore */
	State* getNextState;

private:
	typedef typename list<State*>::iterator iter;

	std::list<State*> _waiting;
	std::list<State*> _visited;
};

} // Structures
} // PetriEngine

#endif // BFSORDERABLESTATESET_H
