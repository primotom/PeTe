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
#ifndef DTAPNTRANSLATOR_H
#define DTAPNTRANSLATOR_H

#include <string>
#include <list>
#include <vector>

#include "../AbstractPetriNetBuilder.h"
#include "AbstractDTAPNBuilder.h"

namespace PetriEngine{
namespace DTAPN {

/** Translator from DTAPN to PNDV */
class DTAPNTranslator : public AbstractDTAPNBuilder
{
	/** Encapsulates a place */
	struct Place{
		std::string name;
		int tokens;
		int maxAge;
		int maxInvariantAge;
	};
	typedef std::list<Place> PlaceList;
	typedef std::list<Place>::iterator PlaceIter;
	/** Encapsulates a transition */
	struct Transition{
		std::string name;
	};
	typedef std::list<Transition> TransitionList;
	typedef std::list<Transition>::iterator TransitionIter;
	/** An input arc */
	struct InArc{
		std::string start;
		std::string end;
		int startInterval;
		int endInterval;
	};
	typedef std::list<InArc> InArcList;
	typedef std::list<InArc>::iterator InArcIter;
	/** An output arc */
	struct OutArc{
		std::string start;
		std::string end;
	};
	typedef std::list<OutArc> OutArcList;
	typedef std::list<OutArc>::iterator OutArcIter;
public:
	DTAPNTranslator(int bound){ this->bound = bound; }
	void addPlace(const std::string& name, int tokens, int maxInvariantAge, double x = 0, double y = 0);
	void addTransition(const std::string& name, double x = 0, double y = 0);
	void addInputArc(const std::string& place, const std::string& transition, int startInterval, int endInterval);
	void addOutputArc(const std::string& transition, const std::string& place);
	void makePNDV(AbstractPetriNetBuilder* builder);
	static std::string translateQuery(const std::string& query);
private:
	/** Maximum number of tokens at a place */
	int bound;
	/** List of DTAPN places */
	PlaceList places;
	/** List of DTAPN transitions */
	TransitionList transitions;
	/** List of DTAPN input arcs */
	InArcList inArcs;
	/** List of DTAPN output arcs */
	OutArcList outArcs;
	/** Returns a DTAPN place with the provided name */
	Place& findPlace(const std::string& name);
	/** Make sure it can't collide with stuff we have */
	std::string escapeIdentifier(std::string identifier);
	/** Gets a list of input arcs for a transition */
	InArcList preset(const std::string& transition);
	/** Gets a list of output arcs for a transition */
	OutArcList postset(const std::string& transition);
	/** True, if place is the target of any output arcs */
	bool isEndPlace(const std::string& place);
	/** Lock state, when in idle */
	int lockStateIdle;
	/** Lock state, when in ageing */
	int lockStateAgeing;
	/** Lock state for a transition */
	std::string lockState(const std::string& transition);
	/** Pre place to a transition */
	std::string prePlace(const std::string& transition, int inArcNr);
	/** Transition from place to pre-place */
	std::string prePlaceTransition(const std::string& transition, int inArcNr, int tokenIndex);
	/** Post-place from a transition, only one per place */
	std::string postPlace(const std::string& place);
	/** Transition from post-place to place */
	std::string postPlaceTransition(const std::string& place, int tokenIndex);
	/** Variable representing the age of a specific token at a place */
	std::string tokenAgeVariable(const std::string& place, int tokenIndex);
	/** Max transition between two intermediate ageing places */
	std::string maxTransition(const std::string& place, int tokenIndex);
	/** Age transition between two intermediate ageing places */
	std::string ageTransition(const std::string& place, int tokenIndex);
	/** An intermediate ageing place*/
	std::string intermediateAgeingPlace(const std::string& place, int tokenIndex);
	/** to ensure unique names */
	bool hasIdentifier(const std::string& id);
};

} // DTAPN
} // PetriEngine

#endif // DTAPNTRANSLATOR_H
