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
#include <PetriParse/PNMLParser.h>

#include <stdio.h>
#include <PetriEngine/PetriNetBuilder.h>
#include <PetriEngine/PQL/PQL.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <PetriEngine/PQL/PQLParser.h>
#include <PetriEngine/Reachability/LinearOverApprox.h>
#include <PetriEngine/Reachability/BestFirstReachabilitySearch.h>
#include <PetriEngine/PQL/Contexts.h>
#include <PetriEngine/Reachability/RandomDFS.h>
#include <PetriEngine/Reachability/RandomQueryGenerator.h>

using namespace std;
using namespace PetriEngine;
using namespace PetriEngine::PQL;
using namespace PetriEngine::Reachability;

/* Command line interface:
	PeTAPAAL [options] model-file query-file
	Options:
		-k	--k-bound <number of tokens>			Max-number of tokens, 0 to ignore (default)
		-t	--trace									Provide XML-trace to stderr
		-s	--search-strategy <strategy>			Search strategy:
													 - BestFS	Heuristic search (default)
													 - BFS		Best first search
													 - DFS		Depth first search
													 - RDFS		Random depth first search
		-m	--memory-limit <megabyte>				Memory limit for state space in MB, 0 for unlimited (1 GB default)

	Return Values:
		0		Successful, query satisfiable
		1		Unsuccesful, query not satisfiable
		2		Unknown, algorithm was unable to answer the question
		3		Error, see stderr for error message
*/

/** Enumeration of return values from PeTAPAAL */
enum ReturnValues{
	SuccessCode	= 0,
	FailedCode	= 1,
	UnknownCode	= 2,
	ErrorCode	= 3
};

/** Enumeration of search-strategies in PeTAPAAL */
enum SearchStrategies{
	BestFS,		//LinearOverAproxx + UltimateSearch					Memory, k-bound
	BFS,		//LinearOverAproxx + BreadthFirstReachabilitySearch	Memory, k-bound
	DFS,		//LinearOverAproxx + DepthFirstReachabilitySearch	Memory, k-bound
	RDFS		//LinearOverAproxx + RandomDFS						Memory, k-bound
};

int main(int argc, char* argv[]){
	// Commandline arguments
	bool trace = false;
	int kbound = 0;
	SearchStrategies strategy = BestFS;
	int memorylimit = 1024*1024*1024;
	char* modelfile = NULL;
	char* queryfile = NULL;

	//----------------------- Parse Arguments -----------------------//

	// Parse command line arguments
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--k-bound") == 0){
			if(sscanf(argv[++i], "%d", &kbound) != 1 || kbound < 0){
				fprintf(stderr, "Argument Error: Invalid number of tokens \"%s\"\n", argv[i]);
				return ErrorCode;
			}
		}else if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--trace") == 0){
			trace = true;
		}else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--search-strategy") == 0){
			char* s = argv[++i];
			if(strcmp(s, "BestFS") == 0)
				strategy = BestFS;
			else if(strcmp(s, "BFS") == 0)
				strategy = BFS;
			else if(strcmp(s, "DFS") == 0)
				strategy = DFS;
			else if(strcmp(s, "RDFS") == 0)
				strategy = RDFS;
			else{
				fprintf(stderr, "Argument Error: Unrecognized search strategy \"%s\"\n", s);
				return ErrorCode;
			}
		}else if(strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--memory-limit") == 0){
			if(sscanf(argv[++i], "%d", &memorylimit) != 1 || memorylimit < 0){
				fprintf(stderr, "Argument Error: Invalid memory limit \"%s\"\n", argv[i]);
				return ErrorCode;
			}else
				memorylimit *= 1024;
		}else if(modelfile == NULL){
			modelfile = argv[i];
		}else if(queryfile == NULL){
			queryfile = argv[i];
		}else{
			fprintf(stderr, "Argument Error: Unrecognized option \"%s\"\n", modelfile);
			return ErrorCode;
		}
	}

	//----------------------- Validate Arguments -----------------------//

	//Check for model file
	if(!modelfile){
		fprintf(stderr, "Argument Error: No model-file provided\n");
		return ErrorCode;
	}

	//Check for query file
	if(!modelfile){
		fprintf(stderr, "Argument Error: No query-file provided\n");
		return ErrorCode;
	}

	//----------------------- Open Model -----------------------//

	//Load the model, begin scope to release memory from the stack
	PetriNet* net = NULL;
	MarkVal* m0 = NULL;
	VarVal* v0 = NULL;
	{
		//Load the model
		ifstream mfile(modelfile, ifstream::in);
		if(!mfile){
			fprintf(stderr, "Error: Model file \"%s\" couldn't be opened\n", modelfile);
			return ErrorCode;
		}

		//Read everything
		stringstream buffer;
		buffer << mfile.rdbuf();

		//Parse and build the petri net
		PetriNetBuilder builder(false);
		PNMLParser parser;
		parser.parse(buffer.str(), &builder);
		parser.makePetriNet();

		//Build the petri net
		net = builder.makePetriNet();
		m0 = builder.makeInitialMarking();
		v0 = builder.makeInitialAssignment();

		// Close the file
		mfile.close();
	}

	//----------------------- Parse Query -----------------------//

	//Condition to check
	Condition* query = NULL;
	bool isInvariant = false;

	//Read query file, begin scope to release memory
	{
		//Open query file
		ifstream qfile(queryfile, ifstream::in);
		if(!qfile){
			fprintf(stderr, "Error: Query file \"%s\" couldn't be opened\n", queryfile);
			return ErrorCode;
		}

		//Read everything
		stringstream buffer;
		buffer << qfile.rdbuf();
		string querystr = buffer.str();

		//Validate query type
		if(querystr.substr(0, 2) != "EF" && querystr.substr(0, 2) != "AG"){
			fprintf(stderr, "Error: Query type \"%s\" not supported, only (EF and AG is supported)\n", querystr.substr(0, 2).c_str());
			return ErrorCode;
		}

		//Check if is invariant
		isInvariant = querystr.substr(0, 2) == "AG";

		//Parse query
		query = ParseQuery(querystr.substr(2));
		if(!query){
			fprintf(stderr, "Error: Failed to parse query \"%s\"\n", querystr.substr(2).c_str());
			return ErrorCode;
		}

		//Close query file
		qfile.close();
	}

	//----------------------- Context Analysis -----------------------//

	//Create scope for AnalysisContext
	{
		//Context analysis
		AnalysisContext context(*net);
		query->analyze(context);

		//Print errors if any
		if(context.errors().size() > 0){
			for(size_t i = 0; i < context.errors().size(); i++){
				fprintf(stderr, "Query Context Analysis Error: %s\n", context.errors()[i].toString().c_str());
			}
			return ErrorCode;
		}
	}

	//----------------------- Reachability -----------------------//



	//----------------------- Output trace -----------------------//

	return UnknownCode;
}
























