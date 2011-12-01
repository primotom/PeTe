#ifndef MONORANDOMDFS_H
#define MONORANDOMDFS_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"

namespace PetriEngine { namespace Reachability {

/** Implements reachability check in a random DFS manner, using a hash table */
class MonoRandomDFS : public ReachabilitySearchStrategy {
public:
	MonoRandomDFS() : ReachabilitySearchStrategy(){}

	/** Perform reachability check using random DFS with a hash table */
	ReachabilityResult reachable(const PetriNet& net,
								 const MarkVal* m0,
								 const VarVal* v0,
								 const BoolVal* b0,
								 PQL::Condition* query);
};

} // Reachability
} // PetriEngine

#endif // MONORANDOMDFS_H
