
#include "MonoBestFirstReachabilitySearch.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/BestFSOrderableStateSet.h"
#include "../Structures/StateAllocator.h"

#include <string.h>
#include <iostream>

using namespace PetriEngine::Structures;
using namespace PetriEngine::PQL;

namespace PetriEngine{
namespace Reachability{

ReachabilityResult MonoBestFirstReachabilitySearch::reachable(const PetriNet &net,
															const MarkVal *m0,
															const VarVal *v0,
															const BoolVal *ba,
															PQL::Condition *query){
	StateAllocator<> allocator(net);

	State* s0 = allocator.createState();
	memcpy(s0->marking(), m0, sizeof(MarkVal) * net.numberOfPlaces());
	memcpy(s0->intValuation(), v0, sizeof(VarVal) * net.numberOfIntVariables());
	memcpy(s0->boolValuation(), ba, sizeof(BoolVal) * net.numberOfBoolVariables());

	if(query->evaluate(*s0))
		return ReachabilityResult(ReachabilityResult::Satisfied, "Satisfied initially", 0, 0);

	//Initialize subclasses
	MonotonicityContext context(&net, query);
	context.analyze();

	BestFSOrderableStateSet states(net, &context, _distanceStrategy, query);
	states.add(s0);

	//Allocate new state
	State* ns = allocator.createState();
	int count = 0;
	BigInt expandedStates = 0;
	BigInt exploredStates = 0;
	size_t max = 1;
	State* s = states.getNextState();
	while(s){
		if(count++ & 1<<16){
			count = 0;
			if(states.waitingSize() > max)
				max = states.waitingSize();
			this->reportProgress((double)(max - states.waitingSize()) / ((double)(max)));
			if(this->abortRequested())
				return ReachabilityResult(ReachabilityResult::Unknown,
										  "Query aborted!");
		}



		// Attempt to fire each transition
		for(unsigned int t = 0; t < net.numberOfTransitions(); t++){
			if(net.fire(t, s->marking(), s->intValuation(), s->boolValuation(), ns->marking(), ns->intValuation(), ns->boolValuation())){
				//If it's new
				if(states.add(ns)){
					exploredStates++;
					//Set parent and transition for the state
					ns->setParent(s);
					ns->setTransition(t);

					//Test query
					if(query->evaluate(*ns)){
						//ns->dumpTrace(net);
						return ReachabilityResult(ReachabilityResult::Satisfied,
												  "Query was satified!", expandedStates, exploredStates, ns->pathLength(), ns->trace());
					}

					//Allocate new state, as states take ownership
					ns = allocator.createState();
				}
			}
		}
		//Take something out of the queue
		s = states.getNextState();
		expandedStates++;
	}

	return ReachabilityResult(ReachabilityResult::NotSatisfied,
							  "Query cannot be satisfied!", expandedStates, exploredStates);
}

} // Reachability
} // PetriEngine
