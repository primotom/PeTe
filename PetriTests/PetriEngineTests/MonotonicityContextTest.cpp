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

		int index1 = 0;
		int index2 = 0;

		for(unsigned int i = 0; i < net->numberOfPlaces(); i++){
			if(net->placeNames()[i] == "P1")
				index1 = i;
			if(net->placeNames()[i] == "P2")
				index2 = i;
		}

		MonotonicityContext context(net);
		context.analyze();
		CHECK(context.goodPlaces()[index1] == false);
		CHECK(context.goodPlaces()[index2] == true);
	}

	TEST(BadVariableTest){
		PetriNetBuilder builder;
		builder.addPlace("P1", 1, 0, 0);
		builder.addPlace("P2", 0, 0, 0);
		builder.addBoolVariable("B1", false);
		builder.addBoolVariable("B2", false);
		builder.addTransition("T1", "B1 or !B2", "B2 := true", 0, 0);
		builder.addInputArc("P1", "T1", 1);
		builder.addOutputArc("T1", "P2", 1);

		PetriNet* net = builder.makePetriNet();

		int indexBad = 0;
		int indexGood = 0;

		for(unsigned int i = 0; i < net->numberOfBoolVariables(); i++){
			if(net->boolVariableNames()[i] == "B2")
				indexBad = i;
			if(net->boolVariableNames()[i] == "B1")
				indexGood = i;
		}

		MonotonicityContext context(net);
		context.analyze();
		CHECK(context.goodBoolVariables()[indexBad] == false);
		CHECK(context.goodBoolVariables()[indexGood] == true);

	}

}
