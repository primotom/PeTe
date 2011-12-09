
/* PeTe - Petri Engine exTremE
 * Copyright (C) 2011  Jonas Finnemann Jensen <jopsen@gmail.com>,
 *                     Thomas Søndersø Nielsen <primogens@gmail.com>,
 *                     Lars Kærlund Østergaard <larsko@gmail.com>,
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MonoDFS.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/StateSet.h"
#include "../Structures/DFSStateset.h"
#include "../Structures/NaiveListStateSet.h"
#include "../Structures/StateAllocator.h"
#include <list>
#include <string.h>

using namespace PetriEngine::PQL;
using namespace PetriEngine::Structures;

namespace PetriEngine{ namespace Reachability {

ReachabilityResult MonoDFS::reachable(const PetriNet &net,
														   const MarkVal *m0,
														   const VarVal *v0,
														   const BoolVal *b0,
														   PQL::Condition *query){
	//Do we initially satisfy query?
	if(query->evaluate(PQL::EvaluationContext(m0, v0, b0)))
		return ReachabilityResult(ReachabilityResult::Satisfied,
								  "A state satisfying the query was found");
	//Create StateSet
	MonotonicityContext context(&net);
	context.analyze();

	DFSStateSet states(net,&context, ModeNormal);

	StateAllocator<1000000> allocator(net);

	State* s0 = allocator.createState();
	memcpy(s0->marking(), m0, sizeof(MarkVal)*net.numberOfPlaces());
	memcpy(s0->intValuation(), v0, sizeof(VarVal)*net.numberOfIntVariables());
	memcpy(s0->boolValuation(), b0, sizeof(BoolVal)*net.numberOfBoolVariables());

	states.add(s0, 0);

	unsigned int max = 0;
	int count = 0;
	BigInt exploredStates = 0;
	BigInt expandedStates = 0;
	State* ns = allocator.createState();
	while(!states.Wating().empty()){
		if(count++ & 1<<18){
			if(states.Wating().size() > max)
				max = states.Wating().size();
			count = 0;
			//report progress
			reportProgress((double)(max-states.Wating().size())/(double)max);
			//check abort
			if(abortRequested())
				return ReachabilityResult(ReachabilityResult::Unknown,
										"Search was aborted.");
		}

		//Take first step of the stack
		State* s = states.getWating().state;
		//Mark as visited
		//states.visit(s);
		ns->setParent(s);
		bool foundSomething = false;
		for(unsigned int t = states.getWating().t; t < net.numberOfTransitions(); t++){
			if(net.fire(t, s->marking(), s->intValuation(),s->boolValuation(), ns->marking(), ns->intValuation(), ns->boolValuation())){
				if(states.add(ns,t)){
					ns->setTransition(t);
					if(query->evaluate(PQL::EvaluationContext(ns->marking(), ns->intValuation(), ns->boolValuation())))
						return ReachabilityResult(ReachabilityResult::Satisfied,
									  "A state satisfying the query was found", expandedStates, exploredStates, ns->pathLength(), ns->trace());
					//stack.back().t = t + 1;
					//stack.push_back(Step(ns,0));
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
							"No state satisfying the query exists.", expandedStates, count);
}

} // Reachability
} // PetriEngine
