
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

	states.add(s0,0);
	State* ns = allocator.createState();

	unsigned int max = 0;
	int count = 0;
	BigInt exploredStates = 0;
	BigInt expandedStates = 0;
	BigInt transitionFired  = 0;
	State* s = states.getNextState();
	while(states.waitingSize()){
		if(count++ & 1<<18){
			if(states.waitingSize() > max)
				max = states.waitingSize();
			count = 0;
			//report progress
			reportProgress((double)(max-states.waitingSize())/(double)max);
			//check abort
			if(abortRequested())
				return ReachabilityResult(ReachabilityResult::Unknown,
										"Search was aborted.");
		}

		//Take first step of the stack
		RStep tstep = states.getNextStep();
		State* s = tstep.state;

		ns->setParent(s);
		bool foundSomething = false;
		for(unsigned int t = tstep.t.back(); !tstep.t.empty(); tstep.t.pop_back()){
			t = tstep.t.back();
			if(net.fire(t, s->marking(), s->intValuation(),s->boolValuation(), ns->marking(), ns->intValuation(), ns->boolValuation())){
				transitionFired++;
				if(states.add(ns,t)){
					ns->setTransition(t);
					if(query->evaluate(PQL::EvaluationContext(ns->marking(), ns->intValuation(), ns->boolValuation())))
						return ReachabilityResult(ReachabilityResult::Satisfied,
									  "A state satisfying the query was found", expandedStates, exploredStates, transitionFired, states.getCountRemove(), ns->pathLength(), ns->trace());

					exploredStates++;
					foundSomething = true;
					ns = allocator.createState();
					break;
				}
			}
		}
		if(!foundSomething){
			states.popWating();
			expandedStates++;
		}
	}
	//states.writeStatistics();
	return ReachabilityResult(ReachabilityResult::NotSatisfied,
							"No state satisfying the query exists.", expandedStates, exploredStates, transitionFired, states.getCountRemove());
}

} // Reachability
} // PetriEngine
