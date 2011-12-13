#ifndef INDEXEDBFS_H
#define INDEXEDBFS_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"

namespace PetriEngine { namespace Reachability {

/** Implements reachability check in a BFS manner using the indexed state set */
class IndexedBFS : public ReachabilitySearchStrategy {
public:
	IndexedBFS(bool varianceFirst = true) : ReachabilitySearchStrategy() {
		_varianceFirst = varianceFirst;
	}

	/** Perform reachability check using BFS */
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal *initialMarking,
								 const VarVal *initialAssignment,
								 const BoolVal *initialBooleanAssignment,
								 PQL::Condition *query);
private:
	bool _varianceFirst;
};

} // Structures
} // PetriEngine

#endif // INDEXEDBFS_H
