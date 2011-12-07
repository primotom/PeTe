#ifndef INDEXEDSTATESET_H
#define INDEXEDSTATESET_H

#include "PQL/Contexts.h"
#include "Structures/OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

class IndexedStateSet : OrderableStateSet {
public:
	IndexedStateSet(const PetriNet& net, PQL::MonotonicityContext* context)
		: OrderableStateSet(net, context){}

	bool add(State* state);
	bool equal(State* s1, State* s2);

};
}
}

#endif // INDEXEDSTATESET_H
