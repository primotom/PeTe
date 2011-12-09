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
#include <UnitTest++.h>
#include "PetriNet.h"
#include "PetriNetBuilder.h"
#include "Structures/State.h"
#include "Structures/OrderableStateSet.h"
#include "Structures/StateAllocator.h"
#include "PQL/Contexts.h"

using namespace PetriEngine;
using namespace PetriEngine::Structures;
	/*
SUITE(OrderableStateSetTest){
	TEST(AddSmallerStateBooleanTest){
		StateAllocator<> allocator(3,0,1);

		State* s1 = allocator.createState();
		State* s2 = allocator.createState();

		for(int i = 0; i<3; i++){
			s1->marking()[i] = i;
			s2->marking()[i] = i+1;
		}

		s1->boolValuation()[0]=false;
		s2->boolValuation()[0]=true;

		PetriNetBuilder builder;

		builder.addBoolVariable("b",false);
		builder.addPlace("P1", 1, 0, 0);
		builder.addPlace("P2", 0, 0, 0);
		builder.addPlace("P3", 0, 0, 0);
		builder.addTransition("T1","","b:=true",0,0);
		builder.addTransition("T2","b","",0,0);
		builder.addInputArc("P1", "T1",1);
		builder.addOutputArc("T1","P2",1);
		builder.addInputArc("P2","T2",1);
		builder.addOutputArc("T2","P3",1);

		PetriNet* net = builder.makePetriNet();
		PQL::MonotonicityContext context(net);
		context.analyze();

		OrderableStateSet set1(*net, &context);
		OrderableStateSet set2(*net, &context);
		OrderableStateSet set3(*net, &context);

		CHECK(set1.add(s2));
		CHECK(!set1.add(s1));

		CHECK(set2.add(s1));
		CHECK(set2.add(s2));

		CHECK(set3.add(s1));
		CHECK(set3.add(s2));
		CHECK(!set3.add(s1));


		CHECK(set1.States().size() == set2.States().size());
		CHECK(set1.States().size() == set3.States().size());
	}

	TEST(AddSmallerStateMarkingTest){
		StateAllocator<> allocator(3,0,0);

		State* s1 = allocator.createState();
		State* s2 = allocator.createState();

		s1->marking()[0] = 1;
		s1->marking()[1] = 0;
		s1->marking()[2] = 0;

		s2->marking()[0] = 2;
		s2->marking()[1] = 0;
		s2->marking()[2] = 0;

		PetriNetBuilder builder;
		builder.addPlace("P1", 1,0,0);
		builder.addPlace("P2", 0,0,0);
		builder.addPlace("P3", 0,0,0);
		builder.addTransition("T1","P1>2","",0,0);
		builder.addTransition("T2","P2<2","",0,0);
		builder.addInputArc("P1", "T1",1);
		builder.addOutputArc("T1","P2",1);
		builder.addInputArc("P2","T2",1);
		builder.addOutputArc("T2","P3",1);

		PetriNet* net = builder.makePetriNet();
		PQL::MonotonicityContext context(net);
		context.analyze();

		CHECK(context.goodPlaces()[0]); //GOOD
		CHECK(!context.goodPlaces()[1]);//BAD
		CHECK(context.goodPlaces()[2]); //GOOD

		OrderableStateSet set1(*net, &context);
		OrderableStateSet set2(*net, &context);
		OrderableStateSet set3(*net, &context);

		CHECK(set1.add(s1));
		CHECK(set1.add(s2));

		CHECK(set2.add(s2));
		CHECK(!set2.add(s1));

		CHECK(set3.add(s1));
		CHECK(set3.add(s2));
		CHECK(!set3.add(s1));

		CHECK(set1.States().size() == set2.States().size());
		CHECK(set1.States().size() == set3.States().size());

	}
	TEST(AddSmallerStateCombinedTest){
		StateAllocator<> allocator(3,0,2);

		State* s1 = allocator.createState();
		State* s2 = allocator.createState();

		s1->marking()[0] = 1;
		s1->marking()[1] = 0;
		s1->marking()[2] = 0;
		s1->boolValuation()[0] = true;
		s1->boolValuation()[1] = false;

		s2->marking()[0] = 2;
		s2->marking()[1] = 0;
		s2->marking()[2] = 0;
		s2->boolValuation()[0] = true;
		s2->boolValuation()[1] = true;

		PetriNetBuilder builder;
		builder.addBoolVariable("B1",false);
		builder.addBoolVariable("B2",false);
		builder.addBoolVariable("B3",false);
		builder.addPlace("P1", 1,0,0);
		builder.addPlace("P2", 0,0,0);
		builder.addPlace("P3", 0,0,0);
		builder.addTransition("T1","","B1:=true;B2:=true;B3:=true;",0,0);
		builder.addTransition("T2","!B1 or (P2<1 and P1>1 and B2)","",0,0);
		builder.addTransition("T3","B1","B3:=false",0,0);
		builder.addInputArc("P1", "T1",1);
		builder.addOutputArc("T1","P2",1);
		builder.addInputArc("P2","T2",1);
		builder.addOutputArc("T2","P3",1);
		builder.addInputArc("P2","T3",1);

		PetriNet* net = builder.makePetriNet();
		PQL::MonotonicityContext context(net);
		context.analyze();

		CHECK(context.goodPlaces()[0]); //GOOD
		CHECK(!context.goodPlaces()[1]);//BAD
		CHECK(context.goodPlaces()[2]); //GOOD
		CHECK(!context.goodBoolVariables()[0]);//BAD
		CHECK(context.goodBoolVariables()[1]);//GOOD
		CHECK(!context.goodBoolVariables()[2]);//BAD //TODO: Consider making this a good variable

		/*OrderableStateSet set1(*net, &context);
		OrderableStateSet set2(*net, &context);
		OrderableStateSet set3(*net, &context);

		CHECK(set1.add(s1));
		CHECK(set1.add(s2));

		CHECK(set2.add(s2));
		CHECK(!set2.add(s1));

		CHECK(set3.add(s1));
		CHECK(set3.add(s2));
		CHECK(!set3.add(s1));

		CHECK(set1.States().size() == set2.States().size());
		CHECK(set1.States().size() == set3.States().size());


	}
}
*/
