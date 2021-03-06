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
#ifndef ABSTRACTDTAPNBUILDER_H
#define ABSTRACTDTAPNBUILDER_H

#include <string>

namespace PetriEngine {
namespace DTAPN {

/** Abstract base class for all DTAPN builders */
class AbstractDTAPNBuilder{
public:
	virtual void addPlace(const std::string& name,
						  int tokens,
						  int maxInvariantAge = -1,
						  double x = 0,
						  double y = 0) = 0;

	virtual void addTransition(const std::string& name,
							   double x =0,
							   double y = 0) = 0;

	virtual void addInputArc(const std::string& place,
							 const std::string& transition,
							 int startInterval,
							 int endInterval) = 0;

	virtual void addOutputArc(const std::string& transition,
							  const std::string& place) = 0;

	virtual ~AbstractDTAPNBuilder(){}
};

} // DTAPN
} // PetriEngine

#endif // ABSTRACTDTAPNBUILDER_H
