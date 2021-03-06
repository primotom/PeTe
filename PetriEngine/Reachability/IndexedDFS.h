#ifndef INDEXEDDFS_H
#define INDEXEDDFS_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"
#include "../Structures/DFSIndexedStateSet.h"
namespace PetriEngine { namespace Reachability {

/** Implements reachability check by trying to select larger states first, using a hash table */
class IndexedDFS : public ReachabilitySearchStrategy
{
public:
	IndexedDFS(bool varianceFirst = true, PetriEngine::Structures::Mode mode = PetriEngine::Structures::ModeNormal) : ReachabilitySearchStrategy(){
	_varianceFirst = varianceFirst;
	_mode = mode;
	}

	/** Perform reachability check using DFS with hash table */
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal* initialMarking,
								 const VarVal* initialAssignment,
								 const BoolVal* initialBooleanAssignment,
								 PQL::Condition* query);
private:
	bool _varianceFirst;
	PetriEngine::Structures::Mode _mode;
};
} // Reachability
} // PetriEngine

#endif // INDEXEDDFS_H
