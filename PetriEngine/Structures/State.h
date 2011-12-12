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
#ifndef GENERALSTATE_H
#define GENERALSTATE_H

#include "../PetriNet.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <vector>



namespace PetriEngine { namespace Structures {

template<size_t blocksize> class StateAllocator;
template<size_t blocksize> class BoundedStateAllocator;

/** GeneralState class for reachability searches.
  * Used in most reachability search cases */
class State {
public:
	MarkVal* marking(){return _marking;}
	const MarkVal* marking() const{return _marking;}

	VarVal* intValuation(){return _intValuation; }
	const VarVal* intValuation() const{return _intValuation; }

	BoolVal* boolValuation(){return _boolValuation; }
	const BoolVal* boolValuation() const{return _boolValuation; }


	void setMarking(MarkVal* m){ _marking = m;}
	void setIntValuation(VarVal* v){ _intValuation = v;}
	void setBoolValuation(BoolVal* v){ _boolValuation = v;}

	/** Getter for the parent */
	State* parent(){ return _parent; }
	/** Setter for the parent*/
	void setParent(State* parent){ _parent = parent; }

	/** Getter for the transition the parent took to get here */
	unsigned int transition(){ return _parentTransition; }
	/** Setter for the transition the parent took to get here */
	void setTransition(unsigned int t, unsigned int multiplicity = 1){ _parentTransition = t; _transitionMultiplicity = multiplicity; }
	/** Get transition multiplicity */
	unsigned int transitionMultiplicity() const { return _transitionMultiplicity; }

	/** Gets the length of the trace to this state */
	int pathLength(){
		if(_parent)
			return transitionMultiplicity() + _parent->pathLength();
		else
			return 0;
	}

	/** Get trace from initial state */
	std::vector<unsigned int> trace() {
		State* current = this;
		std::vector<unsigned int> trace;
		while(current->parent()){
			for(unsigned int i = 0; i < transitionMultiplicity(); i++)
				trace.push_back(current->transition());
			current = current->parent();
		}
		std::reverse(trace.begin(), trace.end());
		return trace;
	}

	/** Dump trace to stderr */
	void dumpTrace(const PetriNet& net){
		fprintf(stderr, "trace: (Latest first)");
		State* c = this;
		while(c->parent()){
			fprintf(stderr, "%s\n", net.transitionNames()[c->transition()].c_str());
			c = c->parent();
		}
	}

	/** Index of state */
	int stateIndex(const PetriNet& net){
		int temp = 0;
		for(uint i = 0; i < net.numberOfPlaces(); i++){
			temp += marking()[i];
		}
		for(uint i = 0; i < net.numberOfBoolVariables(); i++){
			temp += boolValuation()[i];
		}
		return temp;
	}

	/** Variation of state */
	int stateVariation(const PetriNet& net, int index){
		int avg = index / net.numberOfPlaces();

		int temp = 0;
		for(uint i = 0; i < net.numberOfPlaces(); i++){
			temp += std::abs(avg - marking()[i]);
		}
		for(uint i = 0; i < net.numberOfBoolVariables(); i++){
			if(!boolValuation()[i])
				temp += 1;
		}
		return temp;
	}

	/** State specialisation of std::hash */
	class hash : public std::unary_function<State*, size_t>{
	public:
		size_t operator()(const State* state) const{
			//TODO: Rotate bits during hashing
			size_t hash = 0;
			for(unsigned int i = 0; i < nPlaces; i++)
				hash ^=	(state->_marking[i] << (i*4 % (sizeof(MarkVal)*8))) | (state->_marking[i] >> (32 - (i*4 % (sizeof(MarkVal)*8))));
			for(unsigned int i = 0; i < nIntVariables; i++)
				hash ^= (state->_intValuation[i] << (i*4 % (sizeof(VarVal)*8))) | (state->_intValuation[i] >> (32 - (i*4 % (sizeof(VarVal)*8))));
			for(unsigned int i = 0; i < nBoolVariables; i++)
				hash ^= (state->_boolValuation[i] << (i*4 % (sizeof(BoolVal)*8))) | (state->_boolValuation[i] >> (32 - (i*4 % (sizeof(BoolVal)*8))));
			return hash; //TODO need implementation for bools if we are going to use it
		}
		hash(unsigned int places, unsigned int variables)
			: nPlaces(places), nIntVariables(variables){}
	private:
		unsigned int nPlaces;
		unsigned int nIntVariables;
		unsigned int nBoolVariables;
	};

	/** State specialisation of std::equal_to */
	class equal_to : public std::binary_function<State*, State*, bool>{
	public:
		bool operator()(const State* state1, const State* state2) const{
			bool equal = true;
			//Check int valuations
			for(unsigned int i = 0; i < nIntVariables; i++){
				equal &= state1->_intValuation[i] == state2->_intValuation[i];
				if(!equal)
					return false;
			}
			//Check bool valuations
			for(unsigned int i = 0; i < nBoolVariables; i++){
				equal &= state1->_boolValuation[i] == state2->_boolValuation[i];
				if(!equal)
					return false;
			}

			//Check marking
			for(unsigned int i = 0; i < nPlaces; i++){
				equal &= state1->_marking[i] == state2->_marking[i];
				if(!equal)
					return false;
			}
			return true;
		}
		equal_to(unsigned int places, unsigned int intVariables, unsigned int boolVariables)
			: nPlaces(places), nIntVariables(intVariables), nBoolVariables(boolVariables) {}
	private:
		unsigned int nPlaces;
		unsigned int nIntVariables;
		unsigned int nBoolVariables;
	};
	template<size_t blocksize> friend class StateAllocator;
	template<size_t blocksize> friend class BoundedStateAllocator;
private:
	State* _parent;
	unsigned int _parentTransition;
	unsigned int _transitionMultiplicity;
	MarkVal* _marking;
	VarVal* _intValuation;
	BoolVal* _boolValuation;
};

}}

#endif //GENERALSTATE_H
