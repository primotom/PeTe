#ifndef BESTFSORDERABLESTATESET_H
#define BESTFSORDERABLESTATESET_H

#include "OrderableStateSet.h"
#include "PriorityList.h"
#include "PQL/Contexts.h"
#include "DistanceMatrix.h"
#include <list>

namespace PetriEngine { namespace Structures {

/** Orderable State Set class which will feed states in a heuristic Best first manner */
class BestFSOrderableStateSet : public OrderableStateSet {
public:
	/** Create the state set */
	BestFSOrderableStateSet(const PetriNet& net, PQL::MonotonicityContext* context, PQL::Condition* query)
		: OrderableStateSet(net, context) {
		_dm = new DistanceMatrix(net);
		_query = query;
	}

	/** Add a state to the state set */
	bool add(State *state);

	/** Get the next state to explore */
	State* getNextState();

	/** Get the size of the waiting list */
	size_t waitingSize();
private:
	typedef PriorityList<State*>::StateIterator iter;
	typedef std::list<State*>::iterator liter;

	PriorityList<State*> _waiting;
	std::list<State*> _visited;
	PQL::DistanceContext::DistanceStrategy _distanceStrategy;
	DistanceMatrix* _dm;
	PQL::Condition* _query;
};

} // Structures
} // PetriEngine

#endif // BESTFSORDERABLESTATESET_H
