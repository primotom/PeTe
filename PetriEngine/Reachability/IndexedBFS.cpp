#include "IndexedBFS.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/IndexedBFSStateSet.h"
#include "../Structures/StateAllocator.h"

#include <list>
#include <string.h>

using namespace PetriEngine::PQL;
using namespace PetriEngine::Structures;

namespace PetriEngine{ namespace Reachability {
ReachabilityResult IndexedBFS::reachable(const PetriNet &net,
										 const MarkVal *m0,
										 const VarVal *v0,
										 const BoolVal *ba,
										 PQL::Condition *query){
	//Do we initially satisfy query?
	if(query->evaluate(PQL::EvaluationContext(m0, v0,ba)))
		return ReachabilityResult(ReachabilityResult::Satisfied,
								  "A state satisfying the query was found");
	//Create StateSet
	MonotonicityContext context(&net, query);
	context.analyze();

	IndexedBFSStateSet states(net, &context, _varianceFirst);

	StateAllocator<1000000> allocator(net);

	State* s0 = allocator.createState();
	memcpy(s0->marking(), m0, sizeof(MarkVal)*net.numberOfPlaces());
	memcpy(s0->intValuation(), v0, sizeof(VarVal)*net.numberOfIntVariables());
	memcpy(s0->boolValuation(), ba, sizeof(BoolVal)*net.numberOfBoolVariables());

	states.add(s0);

	unsigned int max = 0;
	int count = 0;
	BigInt expandedStates = 0;
	BigInt exploredStates = 0;
	BigInt transitionFired  = 0;
	State* ns = allocator.createState();
	State* s = states.getNextState();
	while(s){
		// Progress reporting and abort checking
		if(count++ & 1<<15){
			if(states.waitingSize() > max)
				max = states.waitingSize();
			count = 0;
			// Report progress
			reportProgress((double)(max - states.waitingSize())/(double)max);
			// Check abort
			if(abortRequested())
				return ReachabilityResult(ReachabilityResult::Unknown,
										"Search was aborted.");
		}

		for(unsigned int t = 0; t < net.numberOfTransitions(); t++){
			if(net.fire(t, s, ns)){
				transitionFired++;
				if(states.add(ns)){
					exploredStates++;
					ns->setParent(s);
					ns->setTransition(t);
					if(query->evaluate(PQL::EvaluationContext(ns->marking(), ns->intValuation(), ns->boolValuation()))){
						return ReachabilityResult(ReachabilityResult::Satisfied,
												"A state satisfying the query was found", expandedStates, exploredStates, transitionFired, states.getCountRemove(),  ns->pathLength(), ns->trace());
					}

					ns = allocator.createState();
				}
			}
		}
		s = states.getNextState();
		expandedStates++;
	}

	return ReachabilityResult(ReachabilityResult::NotSatisfied,
						"No state satisfying the query exists.", expandedStates, exploredStates, transitionFired, states.getCountRemove());
}

} // Reachability
} // PetriEngine
