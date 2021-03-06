#ifndef MONODFS_H
#define MONODFS_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"

namespace PetriEngine { namespace Reachability {

/** Implements reachability check by trying to select larger states first, using a hash table */
class MonoDFS : public ReachabilitySearchStrategy
{
public:
	MonoDFS() : ReachabilitySearchStrategy(){}

	/** Perform reachability check using DFS with hash table */
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal* initialMarking,
								 const VarVal* initialAssignment,
								 const BoolVal* initialBooleanAssignment,
								 PQL::Condition* query);
private:

};
} // Reachability
} // PetriEngine

#endif // MONODFS_H
