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
#include "ReachabilitySearchStrategy.h"

//#include "KarpMillerL1SearchStrategy.h"
#include "DepthFirstReachabilitySearch.h"
#include "BreadthFirstReachabilitySearch.h"
#include "MonoDFS.h"
#include "MonoDFSState.h"
#include "MonoDFSBool.h"
#include "MonoBFS.h"
#include "MonoRandomDFS.h"
#include "RandomDFS.h"
#include "MonoNewBestFS.h"
#include "BestFirstReachabilitySearch.h"
#include "MonoBestFirstReachabilitySearch.h"
#include "LinearOverApprox.h"
#include "RandomDFS.h"

#include "UltimateSearch.h"
#include "HeuristicDFS.h"
#include "StateSearch.h"
#include "../PQL/Contexts.h"
#include <stdio.h>

//Basic
#define NAME_DFS								"Naive DFS with Hash"
#define NAME_RandomDFS							"Random DFS with Hash"
#define NAME_BFS								"Naive BFS with Hash"

//Ordered
#define NAME_MonoDFS							"Ordered DFS"
#define NAME_MonoDFSState						"Ordered DFS States"
#define NAME_MonoDFSBool						"Ordered DFS Bools"
#define NAME_MonoBFS							"Ordered BFS"
#define NAME_MonoRandomDFS						"Ordered Random DFS"
#define NAME_MonoNewBestFS						"Ordered new Best First Seach"

//Heuristics
#define NAME_BestFSOld							"Old BestFS (Heuristic distance)"
#define NAME_MonoBestFS							"Old BestFS (Monotonicity Included)"

//Over-approximation by linear programming
#define NAME_LinearOverApprox					"Linear over-approximation"


#define NAME_UltimateSearchDFS					"BestFS Ultimate Edition DFS"
#define NAME_UltimateSearchBFS					"BestFS Ultimate Edition BFS"
#define NAME_StateSearchBFS						"State Search BFS"
#define NAME_StateSearchDFS						"State Search DFS"
#define NAME_GeneralSearch						"Linear Overapprox. and Heuristic Combo"

#include <stdio.h>

using namespace PetriEngine::Structures;

namespace PetriEngine { namespace Reachability {

/** List all reachability strategies, return unqiue display names */
std::vector<std::string> ReachabilitySearchStrategy::listStrategies(){
	std::string strats[] = {
		NAME_DFS,
		NAME_BFS,
		NAME_RandomDFS,
		NAME_MonoDFS,
		NAME_MonoBFS,
		NAME_MonoRandomDFS,
		NAME_MonoDFSState,
		NAME_MonoDFSBool,
		NAME_BestFSOld,
		NAME_MonoBestFS,
		NAME_MonoNewBestFS
	};
	return std::vector<std::string>(strats, strats + sizeof(strats) / sizeof(std::string));
}

/** Create a reachability strategy from string, NULL if not found */
ReachabilitySearchStrategy* ReachabilitySearchStrategy::createStrategy(const std::string& strategy){
	// Basic Search Strategies
	if(strategy == NAME_DFS)
		return new DepthFirstReachabilitySearch();
	if(strategy == NAME_BFS)
		return new BreadthFirstReachabilitySearch();
	if(strategy == NAME_RandomDFS)
		return new RandomDFS();

	// Ordered Search Strategies
	if(strategy == NAME_MonoDFS)
		return new MonoDFS();
	if(strategy == NAME_MonoBFS)
		return new MonoBFS();
	if(strategy == NAME_MonoRandomDFS)
		return new MonoRandomDFS();
	if(strategy == NAME_MonoDFSState)
		return new MonoDFSState();
	if(strategy == NAME_MonoDFSBool)
		return new MonoDFSBool();
	if(strategy == NAME_MonoNewBestFS)
		return new MonoNewBestFS();

	// Heuristic Search Strategies
	if(strategy == NAME_BestFSOld){
		int flags = PQL::DistanceContext::AndSum | PQL::DistanceContext::OrExtreme;
		return new BestFirstReachabilitySearch((PQL::DistanceContext::DistanceStrategy)flags);
	}
	if(strategy == NAME_MonoBestFS){
		int flags = PQL::DistanceContext::AndSum | PQL::DistanceContext::OrExtreme;
		return new MonoBestFirstReachabilitySearch((PQL::DistanceContext::DistanceStrategy)flags);
	}


	//If we didn't find it
	fprintf(stderr, "Reachability strategy: \"%s\" not found!\n", strategy.c_str());
	return new DepthFirstReachabilitySearch();
}

} // Reachability
} // PetriEngine

