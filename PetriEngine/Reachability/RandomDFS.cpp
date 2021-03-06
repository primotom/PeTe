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
#include "RandomDFS.h"
#include "../PQL/PQL.h"
#include "../PQL/Contexts.h"
#include "../Structures/StateSet.h"
#include "../Structures/StateAllocator.h"

#include <list>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace PetriEngine::PQL;
using namespace PetriEngine::Structures;

namespace PetriEngine{ namespace Reachability {

ReachabilityResult RandomDFS::reachable(const PetriNet &net,
										const MarkVal *m0,
										const VarVal *v0,
										const BoolVal *ba,
										PQL::Condition *query){
	//Do we initially satisfy query?
	if(query && query->evaluate(PQL::EvaluationContext(m0, v0, ba)))
		return ReachabilityResult(ReachabilityResult::Satisfied,
								  "A state satisfying the query was found");

	StateSet states(net);
	StateAllocator<> allocator(net);
	std::list<State*> stack;
	srand(time(0));	// Chosen by fair dice roll

	State* s0 = allocator.createState();
	memcpy(s0->marking(), m0, sizeof(MarkVal)*net.numberOfPlaces());
	memcpy(s0->intValuation(), v0, sizeof(VarVal)*net.numberOfIntVariables());
	memcpy(s0->boolValuation(), ba, sizeof(BoolVal)*net.numberOfBoolVariables());

	stack.push_back(s0);
	states.add(s0);
	State* ns = allocator.createState();

	int countdown = rand() % 800000;
	unsigned int max = 0;
	int count = 0;
	BigInt exploredStates = 0;
	BigInt expandedStates = 0;
	BigInt transitionFired  = 0;
	State* s = s0;
	while(!stack.empty()){
		// Progress reporting and abort checking
		if(count++ & 1<<17){
			if(stack.size() > max)
				max = stack.size();
			count = 0;
			// Report progress
			reportProgress((double)(max - stack.size())/(double)max);
			// Check abort
			if(abortRequested())
				return ReachabilityResult(ReachabilityResult::Unknown,
										"Search was aborted.");
		}

		s = stack.back();
		stack.pop_back();

		//Hack for when query is null and we're look to print a random state
		if(!query && countdown-- <= 0){
			//TODO: Remove this hack or copy the code for random state generation, if we need that feature...
			printf("%s == %i ", net.placeNames()[0].c_str(), s->marking()[0]);
			for(unsigned int p = 1; p < net.numberOfPlaces(); p++)
				printf(" and %s == %i ", net.placeNames()[p].c_str(), s->marking()[p]);
			for(unsigned int x = 0; x < net.numberOfIntVariables(); x++)
				printf(" and %s == %i ", net.intVariableNames()[x].c_str(), s->intValuation()[x]);
			for(unsigned int x = 0; x < net.numberOfBoolVariables(); x++)
				printf(" and %s == %i ", net.boolVariableNames()[x].c_str(), s->boolValuation()[x]);
			return ReachabilityResult();
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
					if(query && query->evaluate(*ns)) {
						return ReachabilityResult(ReachabilityResult::Satisfied,
												  "A state satisfying the query was found", expandedStates, exploredStates, transitionFired, 0, ns->pathLength(), ns->trace());
					}
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
					stack.push_back(succ[t]);
					succ[t] = NULL;
					t++;
					break;
				}
				t = (t+1) % net.numberOfTransitions();
			} while(t != random);
		} while(t != random);
	}

	//Hack for when query is null and we're look to print a random state
	if(!query){
		printf("%s == %i ", net.placeNames()[0].c_str(), s->marking()[0]);
			for(unsigned int p = 1; p < net.numberOfPlaces(); p++)
				printf(" and %s == %i ", net.placeNames()[p].c_str(), s->marking()[p]);
			for(unsigned int x = 0; x < net.numberOfIntVariables(); x++)
				printf(" and %s == %i ", net.intVariableNames()[x].c_str(), s->intValuation()[x]);
			for(unsigned int x = 0; x < net.numberOfBoolVariables(); x++)
				printf(" and %s == %i ", net.boolVariableNames()[x].c_str(), s->boolValuation()[x]);

	}


	return ReachabilityResult(ReachabilityResult::NotSatisfied,
						"No state satisfying the query exists.", expandedStates, exploredStates, transitionFired, 0);
}

}} // Namespaces
