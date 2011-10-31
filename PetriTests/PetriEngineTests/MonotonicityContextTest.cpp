#include <UnitTest++.h>
#include <string>
#include <vector>
#include <iostream>
#include "PQL/Contexts.h"
#include "PetriNetBuilder.h"

using namespace PetriEngine;
using namespace PetriEngine::PQL;

SUITE(MonotonicityContextTest){
	TEST(BadPlaceTest){
		PetriNetBuilder builder;
		builder.addPlace("P1", 1, 0, 0);
		builder.addPlace("P2", 0, 0, 0);
		builder.addTransition("T1", "P1 < 3 and P2 >= 0", "", 0, 0);
		builder.addInputArc("P1", "T1", 1);
		builder.addOutputArc("T1", "P2", 1);

		PetriNet* net = builder.makePetriNet();
		MarkVal* marking = builder.makeInitialMarking();
		VarVal* valuation = builder.makeInitialAssignment();

		int index1 = 0;
		int index2 = 0;

		for(int i = 0; i < net->numberOfPlaces(); i++){
			if(net->placeNames()[i] == "P1")
				index1 = i;
			if(net->placeNames()[i] == "P2")
				index2 = i;
		}

		MonotonicityContext context(net);
		CHECK(context.goodPlaces()[index1] == false);
		CHECK(context.goodPlaces()[index2] == true);
	}

	TEST(BadVariableTest){
		PetriNetBuilder builder;
		builder.addPlace("P1", 1, 0, 0);
		builder.addPlace("P2", 0, 0, 0);
		builder.addVariable("V1", 1, 5);
		builder.addVariable("V2", 0, 2);
		builder.addTransition("T1", "V1 < 3 or V2 > 0", "", 0, 0);
		builder.addInputArc("P1", "T1", 1);
		builder.addOutputArc("T1", "P2", 1);

		PetriNet* net = builder.makePetriNet();
		MarkVal* marking = builder.makeInitialMarking();
		VarVal* valuation = builder.makeInitialAssignment();

		int indexBad = 0;
		int indexGood = 0;

		for(int i = 0; i < net->numberOfVariables(); i++){
			if(net->variableNames()[i] == "V1")
				indexBad = i;
			if(net->variableNames()[i] == "V2")
				indexGood = i;
		}

		MonotonicityContext context(net);
		CHECK(context.goodVariables()[indexBad] == false);
		CHECK(context.goodVariables()[indexGood] == true);
	}
}
