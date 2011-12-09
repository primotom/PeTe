#ifndef MONOBESTFIRSTREACHABILITYSEARCH_H
#define MONOBESTFIRSTREACHABILITYSEARCH_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/DistanceMatrix.h"

namespace PetriEngine { namespace Reachability {

/** Best first reachability strategy including monotic ordering */
class MonoBestFirstReachabilitySearch : public ReachabilitySearchStrategy
{
public:
	/** Create instance of MonoBestFirstReachabilitySearch */
	MonoBestFirstReachabilitySearch(PQL::DistanceContext::DistanceStrategy distanceStrategy){
		_distanceStrategy = distanceStrategy;
	}
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal *initialMarking,
								 const VarVal *initialAssignment,
								 const BoolVal *initialBooleanAssignment,
								 PQL::Condition *query);
private:
	PQL::DistanceContext::DistanceStrategy _distanceStrategy;
};


} // Reachability
} // PetriEngine

#endif // MONOBESTFIRSTREACHABILITYSEARCH_H
