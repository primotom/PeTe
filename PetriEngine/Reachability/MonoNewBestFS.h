#ifndef MONONEWBESTFS_H
#define MONONEWBESTFS_H


#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/DistanceMatrix.h"

namespace PetriEngine { namespace Reachability {

/** Best first reachability strategy including monotic ordering */
class MonoNewBestFS : public ReachabilitySearchStrategy
{
public:
	/** Create instance of MonoNewBFS */
	MonoNewBestFS(){
	}
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal *initialMarking,
								 const VarVal *initialAssignment,
								 const BoolVal *initialBooleanAssignment,
								 PQL::Condition *query);
private:
	void initialize(const PQL::Condition* query, const PetriNet& net);
	/** Method for prioritizing different states, lower priority is better */
	double priority(const Structures::State* state,
					PQL::MonotonicityContext* context,
					const PetriNet& net);
};


} // Reachability
} // PetriEngine

#endif // MONONEWBESTFS_H
