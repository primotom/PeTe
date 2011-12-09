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
#include "Structures/IndexedStateSet.h"
#include "Structures/StateAllocator.h"
#include "PQL/Contexts.h"

using namespace PetriEngine;
using namespace PetriEngine::Structures;

SUITE(OrderableStateSetTest){
	TEST(IndexedTest){
		StateAllocator<> allocator(3,0,1);

		State* s1 = allocator.createState();
		State* s2 = allocator.createState();

		s1->marking()[0] = 1;
		s1->marking()[1] = 1;
		s1->marking()[2] = 1;

		s2->marking()[0] = 1;
		s2->marking()[1] = 1;
		s2->marking()[2] = 0;

		s1->boolValuation()[0]=true;
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

		IndexedStateSet set1(*net, &context);

		CHECK(set1.add(s1));
		CHECK(!set1.add(s2));

		IndexedStateSet set2(*net, &context);
		CHECK(set2.add(s2));
		CHECK(set2.add(s1));
		CHECK((*set2.States().begin())->marking()[0]== 1);
		CHECK((*set2.States().begin())->marking()[1]== 1);
		CHECK((*set2.States().begin())->marking()[2]== 1);
	}

	TEST(EqualIndexedTest){
		StateAllocator<> allocator(3,0,1);

		State* s1 = allocator.createState();
		State* s2 = allocator.createState();

		s1->marking()[0] = 0;
		s1->marking()[1] = 1;
		s1->marking()[2] = 1;

		s2->marking()[0] = 1;
		s2->marking()[1] = 1;
		s2->marking()[2] = 0;

		s1->boolValuation()[0]=true;
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

		IndexedStateSet set1(*net, &context);

		CHECK(set1.add(s1));
		CHECK(set1.add(s2));

		CHECK((*set1.States().begin())->marking()[0]== 0);
		CHECK((*set1.States().begin())->marking()[1]== 1);
		CHECK((*set1.States().begin())->marking()[2]== 1);
		CHECK((*(++set1.States().begin()))->marking()[0]== 1);
		CHECK((*(++set1.States().begin()))->marking()[1]== 1);
		CHECK((*(++set1.States().begin()))->marking()[2]== 0);
	}
}
