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
#include "IndexedSearch.h"
#include "IndexedBFS.h"
#include "IndexedDFS.h"
#include "IndexedBestFS.h"
#include "IndexedRFS.h"
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
#define NAME_MonoDFS							"DFS with Monotonicity"
#define NAME_MonoDFSState						"DFS States with Monotonicity"
#define NAME_MonoDFSBool						"DFS Bools with Monotonicity"
#define NAME_MonoBFS							"BFS with Monotonicity"
#define NAME_MonoRandomDFS						"Random DFS with Monotonicity"
#define NAME_MonoNewBestFS						"Ordered new Best First Seach"

//Indexed
#define NAME_Indexed							"Ordered Largest Index Search with Monotonicity"
#define NAME_IndexedVariance					"Ordered Largest Index Search with Variance and Monotonicity"
#define NAME_IndexedBFS							"Ordered BFS with Monotonicity"
#define NAME_IndexedBFSVariance					"Ordered BFS with Variance and Monotonicity"
#define NAME_IndexedBestFS						"Ordered BestFS with Monotonicity"
#define NAME_IndexedBestFSVariance				"Ordered BestFS with Variance and Monotonicity"
#define NAME_IndexedRFS							"Ordered Random DFS with Monotonicity"
#define NAME_IndexedRFSVariance					"Ordered Random DFS with Variance and Monotonicity"

#define NAME_IndexedDFS							"Ordered DFS with Monotonicity"
#define NAME_IndexedDFSVariance					"Ordered DFS with Variance and Monotonicity"

#define NAME_IndexedDFSState					"Ordered DFS State with Monotonicity"
#define NAME_IndexedDFSStateVariance			"Ordered DFS State with Variance and Monotonicity"

#define NAME_IndexedDFSBool						"Ordered DFS Bool with Monotonicity"
#define NAME_IndexedDFSBoolVariance				"Ordered DFS Bool with Variance and Monotonicity"

//Heuristics
#define NAME_BestFSOld							"BestFS with Hash"
#define NAME_MonoBestFS							"BestFS with Monotonicity"

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
		//NAME_MonoNewBestFS,
		NAME_Indexed,
		//NAME_IndexedVariance,
		NAME_IndexedBFS,
		//NAME_IndexedBFSVariance,
		NAME_IndexedDFS,
		//NAME_IndexedDFSVariance,
		NAME_IndexedDFSState,
		//NAME_IndexedDFSStateVariance,
		NAME_IndexedDFSBool,
		//NAME_IndexedDFSBoolVariance,
		NAME_IndexedBestFS,
		//NAME_IndexedBestFSVariance,
		NAME_IndexedRFS,
		//NAME_IndexedRFSVariance
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

	// Indexed Search Strategies
	if(strategy == NAME_Indexed)
		return new IndexedSearch(false);
	if(strategy == NAME_IndexedVariance)
		return new IndexedSearch();
	if(strategy == NAME_IndexedBFS)
		return new IndexedBFS(false);
	if(strategy == NAME_IndexedBFSVariance)
		return new IndexedBFS();
	if(strategy == NAME_IndexedBestFS){
		int flags = PQL::DistanceContext::AndSum | PQL::DistanceContext::OrExtreme;
		return new IndexedBestFS((PQL::DistanceContext::DistanceStrategy)flags, false);
	}
	if(strategy == NAME_IndexedBestFSVariance){
		int flags = PQL::DistanceContext::AndSum | PQL::DistanceContext::OrExtreme;
		return new IndexedBestFS((PQL::DistanceContext::DistanceStrategy)flags);
	}
	if(strategy == NAME_IndexedRFS)
		return new IndexedRFS(false);
	if(strategy == NAME_IndexedRFSVariance)
		return new IndexedRFS();

	if(strategy == NAME_IndexedDFS)
		return new IndexedDFS(false);
	if(strategy == NAME_IndexedDFSVariance)
		return new IndexedDFS(true);
	if(strategy == NAME_IndexedDFSBool)
		return new IndexedDFS(false,ModeGraterBool);
	if(strategy == NAME_IndexedDFSBoolVariance)
		return new IndexedDFS(true,ModeGraterBool);
	if(strategy == NAME_IndexedDFSState)
		return new IndexedDFS(false,ModeGraterState);
	if(strategy == NAME_IndexedDFSStateVariance)
		return new IndexedDFS(true,ModeGraterState);



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

