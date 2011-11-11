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
#ifndef ULTIMATESEARCH_H
#define ULTIMATESEARCH_H

#include "ReachabilitySearchStrategy.h"
#include "../Structures/State.h"
#include "../PQL/PQL.h"

namespace PetriEngine{
namespace Reachability{

/** Ultimate Search strategy, combination of the existing */
class UltimateSearch : public ReachabilitySearchStrategy
{
public:
	UltimateSearch(bool depthFirst = true){this->depthFirst = depthFirst;}
	ReachabilityResult reachable(const PetriNet &net,
								 const MarkVal* m0,
								 const VarVal* v0,
								 const BoolVal* b0,
								 PQL::Condition* query);
private:
	bool depthFirst;
};

} // Reachability
} // PetriEngine

#endif // ULTIMATESEARCH_H
