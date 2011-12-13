
#include "IndexedRFS.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/IndexedRFSStateSet.h"
#include "../Structures/StateAllocator.h"

#include <list>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


using namespace PetriEngine::PQL;
using namespace PetriEngine::Structures;

namespace PetriEngine{ namespace Reachability {

ReachabilityResult IndexedRFS::reachable(const PetriNet &net,
										 const MarkVal *m0,
										 const VarVal *v0,
										 const BoolVal *ba,
										 PQL::Condition *query){
	//Do we initially satisfy query?
	if(query && query->evaluate(PQL::EvaluationContext(m0, v0, ba)))
		return ReachabilityResult(ReachabilityResult::Satisfied,
								  "A state satisfying the query was found");

	//Create StateSet
	MonotonicityContext context(&net, query);
	context.analyze();

	IndexedRFSStateSet states(net, &context, _varianceFirst);

	StateAllocator<> allocator(net);
	srand(time(0));	// Chosen by fair dice roll

	State* s0 = allocator.createState();
	memcpy(s0->marking(), m0, sizeof(MarkVal)*net.numberOfPlaces());
	memcpy(s0->intValuation(), v0, sizeof(VarVal)*net.numberOfIntVariables());
	memcpy(s0->boolValuation(), ba, sizeof(BoolVal)*net.numberOfBoolVariables());

	states.pushToWaiting(s0);
	State* ns = allocator.createState();

	unsigned int max = 0;
	int count = 0;
	BigInt exploredStates = 0;
	BigInt expandedStates = 0;
	BigInt transitionFired  = 0;
	State* s = states.getNextState();
	while(s){
		// Progress reporting and abort checking
		if(count++ & 1<<17){
			if(states.waitingSize() > max)
				max = states.waitingSize();
			count = 0;
			// Report progress
			reportProgress((double)(max -states.waitingSize())/(double)max);
			// Check abort
			if(abortRequested())
				return ReachabilityResult(ReachabilityResult::Unknown,
										"Search was aborted.");
		}

		State* succ[net.numberOfTransitions()];
		memset(succ, 0, net.numberOfTransitions()*sizeof(State*));
		for(unsigned int t = 0; t < net.numberOfTransitions(); t++){
			if(net.fire(t, s, ns)){
				transitionFired++;
				if(states.add(ns)){
					exploredStates++;
					ns->setParent(s);
					ns->setTransition(t);
					if(query && query->evaluate(*ns))
						return ReachabilityResult(ReachabilityResult::Satisfied,
												"A state satisfying the query was found", expandedStates, exploredStates, transitionFired, states.getCountRemove(), ns->pathLength(), ns->trace());
					succ[t] = ns;
					ns = allocator.createState();
				}
			}
		}
		// Randomly sorts states into the stack
		expandedStates++;
		int random;
		int t;
		do {
			random = rand() % net.numberOfTransitions();
			t = random;
			do{
				if(succ[t]){
					states.pushToWaiting(succ[t]);
					succ[t] = NULL;
					t++;
					break;
				}
				t = (t+1) % net.numberOfTransitions();
			} while(t != random);
		} while(t != random);
		s = states.getNextState();
	}
	return ReachabilityResult(ReachabilityResult::NotSatisfied,
						"No state satisfying the query exists.", expandedStates, exploredStates, transitionFired, states.getCountRemove());

}

} // Reachability
} // PetriEngine
