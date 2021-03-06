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
#include <string>
#include <iostream>
#include <QFile>
#include "PQL/PQLParser.h"
#include "PQL/PQL.h"
#include "PetriNetBuilder.h"

using namespace PetriEngine;
using namespace PetriEngine::PQL;
using namespace std;

SUITE(PQLParserTest){
	TEST(PQLQueryTreeConstruction){
		string query("(P5==5 && P1 > 4)");

		Condition* expr1 = ParseQuery(query);
		CHECK(expr1 != NULL);
		//cout<<"Expression parsed:"<<endl;
		//cout<<expr1->toString()<<endl;

		Condition* expr2 = ParseQuery(expr1->toString());
		CHECK(expr1->toString() == expr2->toString());
	}

	TEST(PQLAssignmentTreeConstruction){
		//This works
		string assignment("X2:=5;Excaliber:=4+5*8+(55*7)+1;");

		AssignmentExpression* ass1 = ParseAssignment(assignment);
		CHECK(ass1 != NULL);

		AssignmentExpression* ass2 = ParseAssignment(ass1->toString());
		CHECK(ass1->toString() == ass2->toString());

		//This shouldn't
		//string assignment1("X2:=5:Excaliber:=4+5*8+(55*7)+1;");

		//AssignmentExpression* ass3 = ParseAssignment(assignment1);
		//CHECK(ass1 == NULL);

	}

	TEST(SUMoConstruction){
		string query("fOK541016 = reachable (Pout4 equals {3*[.]}) and (Pout2 equals {1*[.]}) and (Pout3 equals { }) and (Pout1 equals {1*[.]}) and (P4 equals { }) and (P1 equals {2*[.]}) and (Pm3 equals { }) and (Pm4 equals {1*[.]}) and (P3 equals { }) and (Pm1 equals {1*[.]}) and (Pm2 equals {1*[.]}) and (P2 equals { }) and (Pback1 equals {1*[.]}) and (Pback2 equals {3*[.]}) and (Pback3 equals {5*[.]}) and (Pback4 equals {1*[.]})");

		Condition* expr1 = ParseSUMoQuery(query).query;
		CHECK(expr1);
	}

/*
	TEST(CTLEvaluation){

		string queryString("PdfsHash1==0&&P2==1");

		PetriNetBuilder builder;
		builder.addPlace("P1",0,0,0);
		builder.addPlace("P2",0,1,1);
		builder.addTransition("T0",0.5,0.5);
		builder.addInputArc("P1","T0",1);
		builder.addOutputArc("T0","P2",1);

		PetriNet net = *(builder.makePetriNet());

		CTLParser parser(&net);
		CTLExpr* query = parser.parse(queryString);

		Mark* m0 = net.makeEmptyMarking();
		CHECK(query->evaluate(m0) == 0);

		Mark* m1 = net.makeEmptyMarking();
		m0[0] = 1;
		net.fire(0,m0,m1);
		CHECK(query->evaluate(m1) != 0);
	}
*/
}
