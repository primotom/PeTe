#ifndef RANDOMDFSSTATESET_H
#define RANDOMDFSSTATESET_H

#include "PQL/Contexts.h"
#include "OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

/** Orderable State Set class which will feed states in a Random Depth First manner */
class RandomDFSStateSet : public OrderableStateSet {
public:
	/** Create the state set */
	RandomDFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context)
		: OrderableStateSet(net, context) {}

	/** Add a state to the state set */
	bool add(State* state);

	/** Get the next state to explore */
	State* getNextState();

	/** Get size of waiting */
	size_t waitingSize();


	void pushToWating(State* state);

private:
	typedef std::list<State*>::iterator iter;

	std::list<State*> _waiting;
	std::list<State*> _visited;


};

} // Structures
} // PetriEngine

#endif // RANDOMDFSSTATESET_H
