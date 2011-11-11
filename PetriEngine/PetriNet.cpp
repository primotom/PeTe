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
#include "PetriNet.h"
#include "PQL/PQL.h"
#include "PQL/Contexts.h"
#include "Structures/State.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

namespace PetriEngine{

PetriNet::PetriNet(int places, int transitions, int variables, int booleans)
	: _places(places), _transitions(transitions), _intVariables(variables), _boolVariables(booleans){
	//Store size for later
	_nPlaces = places;
	_nTransitions = transitions;
	_nIntVariables = variables;
	_nBoolVariables = booleans;
	//Allocate space for ranges
	_ranges = new VarVal[variables];

	//Allocate space for conditions and assignments
	size_t s = (sizeof(PQL::Condition*) + sizeof(PQL::AssignmentExpression*)) * transitions;
	char* d = new char[s];
	memset(d, 0, s);
	_conditions = (PQL::Condition**)d;
	_assignments = (PQL::AssignmentExpression**)(d + sizeof(PQL::Condition*)*transitions);

	//Allocate transition matrix
	_tm = new MarkVal[places * transitions * 2];
	for(int i = 0; i < places * transitions * 2; i++)
		_tm[i] = 0;
}

PetriNet::~PetriNet(){
	if(_ranges)
		delete[] _ranges;
	_ranges = NULL;
	if(_tm)
		delete[] _tm;
	_tm = NULL;
	//Conditions and assignments is allocated in the same block
	if(_conditions)
		delete[] (char*)_conditions;
	_conditions = NULL;
	_assignments = NULL;
}

bool PetriNet::fire(unsigned int t,
					const MarkVal* m,
					const VarVal* a,
					MarkVal* result_m,
					VarVal* result_int) const{
	//Check the condition
	if(_conditions[t] &&
	   !_conditions[t]->evaluate(PQL::EvaluationContext(m, a)))
		return false;

	const MarkVal* tv = _tv(t);
	//Check that we can take from the marking
	for(size_t i = 0; i < _nPlaces; i++){
		result_m[i] = m[i] - tv[i];
		if(result_m[i] < 0)
			return false;
	}
	//Add stuff that the marking gives us
	for(size_t i = 0; i < _nPlaces; i++)
		result_m[i] += tv[i+_nPlaces];


	if(_assignments[t])
		_assignments[t]->evaluate(m, a, NULL, result_int, NULL, _ranges, _nIntVariables, 0);
	else
		memcpy(result_int, a, sizeof(VarVal) * _nIntVariables);
	return true;
}

bool PetriNet::fire(unsigned int t,
					const MarkVal* m,
					const VarVal* a,
					const BoolVal* b,
					MarkVal* result_m,
					VarVal* result_int,
					BoolVal* result_bool) const{
	//Check the condition

	if(_conditions[t] &&
	   !_conditions[t]->evaluate(PQL::EvaluationContext(m, a, b))){
		std::cerr<<"condition breaks"<<std::endl;
		return false;
	}

	const MarkVal* tv = _tv(t);
	//Check that we can take from the marking
	for(size_t i = 0; i < _nPlaces; i++){
		result_m[i] = m[i] - tv[i];
		if(result_m[i] < 0)
			return false;
	}
	//Add stuff that the marking gives us
	for(size_t i = 0; i < _nPlaces; i++)
		result_m[i] += tv[i+_nPlaces];


	if(_assignments[t]){
		_assignments[t]->evaluate(m, a, b, result_int, result_bool, _ranges, _nIntVariables, _nBoolVariables);
	}else{
		memcpy(result_int, a, sizeof(VarVal) * _nIntVariables);
		memcpy(result_bool, b, sizeof(BoolVal) * _nBoolVariables);
	}

	return true;
}

bool PetriNet::fire(unsigned int t,
					const Structures::State* s,
					Structures::State* ns,
					int multiplicity) const{
	//Check the condition

	if(_conditions[t] &&
	   !_conditions[t]->evaluate(PQL::EvaluationContext(s->marking(), s->intValuation(),s->boolValuation())))
		return false;

	// We can handle multiplicity if there's conditions or assignments on the transition
	if((_conditions[t] && multiplicity != 1) || (_assignments[t] && multiplicity != 1))
		return false;

	const MarkVal* tv = _tv(t);
	//Check that we can take from the marking
	for(size_t i = 0; i < _nPlaces; i++){
		ns->marking()[i] = s->marking()[i] - tv[i] * multiplicity;
		if(ns->marking()[i] < 0)
			return false;
	}
	//Add stuff that the marking gives us
	for(size_t i = 0; i < _nPlaces; i++)
		ns->marking()[i] += tv[i+_nPlaces] * multiplicity;


	if(_assignments[t])
		_assignments[t]->evaluate(s->marking(), s->intValuation(),s->boolValuation(), ns->intValuation(),ns->boolValuation(), _ranges, _nIntVariables,_nBoolVariables);
	else{
		memcpy(ns->intValuation(), s->intValuation(), sizeof(VarVal) * _nIntVariables);
		memcpy(ns->boolValuation(), s->boolValuation(), sizeof(BoolVal)* _nBoolVariables);
	}

	return true;
}

int PetriNet::inArc(unsigned int place, unsigned int transition) const{
	return _tv(transition)[place];
}

int PetriNet::outArc(unsigned int transition, unsigned int place) const{
	return _tv(transition)[place + _nPlaces];
}

} // PetriEngine
