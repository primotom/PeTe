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
#include "Structures/State.h"
#include "Structures/OrderableStateSet.h"
#include "Structures/StateAllocator.h"

using namespace PetriEngine;
using namespace PetriEngine::Structures;

SUITE(OrderableStateSetTest){
	TEST(SmallerStateTest){
		StateAllocator<6> allocator(2,2,2);

		State* s1 = allocator.createState();
		State* s2 = allocator.createState();

		for(int i = 0; i<2; i++){
			s1->marking()[i] = i;
			s1->boolValuation()[i] = i;

			s2->marking()[i] = i+1;
			s2->boolValuation()[i] = i+1;
		}

		PetriNetBuilder* b;
		b->addPace("p1", 0, 0,0);



		//OrderableStateSet set();
	}
}

