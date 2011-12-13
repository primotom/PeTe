#ifndef INDEXEDBESTFS_H
#define INDEXEDBESTFS_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/DistanceMatrix.h"

namespace PetriEngine { namespace Reachability {

/** Best first reachability strategy including monotic indexed- ordering */
class IndexedBestFS : public ReachabilitySearchStrategy {
public:
	IndexedBestFS(PQL::DistanceContext::DistanceStrategy distanceStrategy, bool varianceFirst = true){
		_distanceStrategy = distanceStrategy;
		_varianceFirst = varianceFirst;
	}

	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal *initialMarking,
								 const VarVal *initialAssignment,
								 const BoolVal *initialBooleanAssignment,
								 PQL::Condition *query);

private:
	bool _varianceFirst;
	PQL::DistanceContext::DistanceStrategy _distanceStrategy;
};

} // Reachability
} // PetriEngine

#endif // INDEXEDBESTFS_H
