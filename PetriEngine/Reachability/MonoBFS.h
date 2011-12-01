#ifndef MONOBFS_H
#define MONOBFS_H
#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"

namespace PetriEngine { namespace Reachability {

/** Implements reachability check in a BFS manner using a hash table */
class MonoBFS : public ReachabilitySearchStrategy {
public:
	MonoBFS() : ReachabilitySearchStrategy() {}

	/** Perform reachability check using BFS with hasing */
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal *m0,
								 const VarVal *v0,
								 const BoolVal *b0,
								 PQL::Condition *query);
};

}} // Namespaces

#endif // MONOBFS_H
