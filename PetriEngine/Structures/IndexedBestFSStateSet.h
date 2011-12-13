#ifndef INDEXEDBESTFSSTATESET_H
#define INDEXEDBESTFSSTATESET_H

#include "OrderableStateSet.h"
#include "PQL/Contexts.h"
#include "PriorityList.h"
#include "DistanceMatrix.h"
#include <list>

namespace PetriEngine { namespace Structures {

class IndexedBestFSStateSet : public OrderableStateSet {
public:
	IndexedBestFSStateSet(const PetriNet& net,
						  PQL::MonotonicityContext* context,
						  PQL::DistanceContext::DistanceStrategy distanceStrategy,
						  PQL::Condition* query,
						  bool variance)
		: OrderableStateSet(net, context){
		_distanceStrategy = distanceStrategy;
		_dm = new DistanceMatrix(net);
		_query = query;
		_variance = variance;
	}

	bool add(State *state);
	State* getNextState();
	size_t waitingSize();

private:
	// Internal declarations
	typedef std::pair<int, State*> IndexedState;
	typedef PriorityList<State*>::StateIterator waitingIter;
	typedef std::list<IndexedState>::iterator visitIter;

	// Internal lists
	PriorityList<State*> _waiting;
	std::list<IndexedState> _visited;

	// Heuristic elements
	PQL::DistanceContext::DistanceStrategy _distanceStrategy;
	DistanceMatrix* _dm;
	PQL::Condition* _query;

	bool _variance;
};

} // Structures
} // PetriEngine

#endif // INDEXEDBESTFSSTATESET_H
