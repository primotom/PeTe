#include "MonoNewBestFS.h"

#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/PriorityQueue.h"
#include "../Structures/EnhancedPriorityQueue.h"
#include "../Structures/OrderableStateSet.h"
#include "../Structures/StateAllocator.h"

#include <string.h>

using namespace PetriEngine::Structures;
using namespace PetriEngine::PQL;

namespace PetriEngine{
namespace Reachability{

ReachabilityResult MonoNewBestFS::reachable(const PetriNet&,
											const MarkVal*,
											const VarVal*,
											const BoolVal*,
											PQL::Condition*){
	/*StateAllocator<> allocator(net);

	State* s0 = allocator.createState();
	memcpy(s0->marking(), m0, sizeof(MarkVal) * net.numberOfPlaces());
	memcpy(s0->intValuation(), v0, sizeof(VarVal) * net.numberOfIntVariables());
	memcpy(s0->boolValuation(), ba, sizeof(BoolVal) * net.numberOfBoolVariables());

	if(query->evaluate(*s0))
		return ReachabilityResult(ReachabilityResult::Satisfied, "Satisfied initially", 0, 0);

	//Initialize subclasses
	initialize(query, net);
	MonotonicityContext context(&net, query);
	context.analyze();

	OrderableStateSet states(net, &context);
	states.add(s0);
	EnhancedPriorityQueue<State*> queue;
	queue.push(0, s0);

	//Allocate new state
	State* ns = allocator.createState();
	int count = 0;
	BigInt expandedStates = 0;
	BigInt exploredStates = 0;
	size_t max = 1;
	while(!queue.empty()){
		if(count++ & 1<<17){
			count = 0;
			if(queue.size() > max)
				max = queue.size();
			this->reportProgress((double)(max - queue.size()) / ((double)(max)));
			if(this->abortRequested())
				return ReachabilityResult(ReachabilityResult::Unknown,
										  "Query aborted!");
		}

		//Take something out of the queue
		State* s = queue.pop();
		expandedStates++;

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

					// Insert in queue, with given priority
					double bestp = priority(ns, &context, net);
					queue.push(bestp, ns);

					//Allocate new state, as states take ownership
					ns = allocator.createState();
				}
			}
		}
	}
	states.writeStatistics();*/
	//return ReachabilityResult(ReachabilityResult::NotSatisfied,
	//						  "Query cannot be satisfied!", expandedStates, exploredStates);
	return ReachabilityResult(ReachabilityResult::NotSatisfied,
							  "Query cannot be satisfied!", 0, 0,0);
}

double MonoNewBestFS::priority(const Structures::State *state,
												 PQL::MonotonicityContext* context,
												 const PetriNet& net){

	double d = 0;

	for(size_t i = 0; i <  net.numberOfBoolVariables(); i++){
		if(context->goodBoolVariables()[i]) {
			if(state->boolValuation()[i])
				d -= 1;
		}
	}
	std::cerr<<d<<std::endl;


	return d;
}

void MonoNewBestFS::initialize(const PQL::Condition*,
							   const PetriNet&){
}

} // Reachability
} // PetriEngine
