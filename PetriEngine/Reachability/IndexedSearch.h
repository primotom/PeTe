#ifndef INDEXEDSEARCH_H
#define INDEXEDSEARCH_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"

#include <list>

namespace PetriEngine { namespace Reachability {

/** Implements reachability check by trying to select larger states first, using a hash table */
class IndexedSearch : public ReachabilitySearchStrategy
{
public:
	IndexedSearch(bool varianceFirst = true) : ReachabilitySearchStrategy(){
		_varianceFirst = varianceFirst;
	}

	/** Perform reachability check using DFS with hash table */
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal* initialMarking,
								 const VarVal* initialAssignment,
								 const BoolVal* initialBooleanAssignment,
								 PQL::Condition* query);
private:
	/** A step in the reachability search */
	struct Step{
		Step(Structures::State* s, unsigned int t){
			state = s;
			this->t = t;
		}
		Structures::State* state;
		unsigned int t;
	};

	bool _varianceFirst;
};
} // Reachability
} // PetriEngine


#endif // INDEXEDSEARCH_H
