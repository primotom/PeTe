#ifndef INDEXEDRFS_H
#define INDEXEDRFS_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"

namespace PetriEngine { namespace Reachability {

/** Implements a reachability search strategy in a random dfs manner, using indexed state set */
class IndexedRFS : public ReachabilitySearchStrategy {
public:
	IndexedRFS(bool varianceFirst = true) : ReachabilitySearchStrategy() {
		_varianceFirst = varianceFirst;
	}

		/** Perform reachability check using random DFS with a indexed state set */
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal *initialMarking,
								 const VarVal *initialAssignment,
								 const BoolVal *initialBooleanAssignment,
								 PQL::Condition *query);

private:
	bool _varianceFirst;
};

} // Reachability
} // PetriEngine

#endif // INDEXEDRFS_H
