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
#include "MonoBFS.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/StateSet.h"
#include "../Structures/BFSOrderableStateSet.h"
#include "../Structures/StateAllocator.h"

#include <list>
#include <string.h>
#include <iostream>

using namespace PetriEngine::PQL;
using namespace PetriEngine::Structures;

namespace PetriEngine{ namespace Reachability {

ReachabilityResult MonoBFS::reachable(const PetriNet &net,
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

	BFSOrderableStateSet states(net,&context);

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
				if(states.add(ns)){
					exploredStates++;
					ns->setParent(s);
					ns->setTransition(t);
					if(query->evaluate(PQL::EvaluationContext(ns->marking(), ns->intValuation(), ns->boolValuation()))){
						//ns->dumpTrace(net);
						return ReachabilityResult(ReachabilityResult::Satisfied,
												"A state satisfying the query was found", expandedStates, exploredStates, ns->pathLength(), ns->trace());
					}

					ns = allocator.createState();
				}
			}
		}
		s = states.getNextState();
		expandedStates++;
	}

	return ReachabilityResult(ReachabilityResult::NotSatisfied,
						"No state satisfying the query exists.", expandedStates, exploredStates);
}

}}
