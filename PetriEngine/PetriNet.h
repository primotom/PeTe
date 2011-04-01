#ifndef PETRINET_H
#define PETRINET_H

#include <string>
#include <vector>
#include <climits>

namespace PetriEngine{

namespace PQL{
	class Condition;
	class AssignmentExpression;
}

class PetriNetFactory;

/** Type used for holding markings values */
typedef int MarkVal;
/** Type used for holding variable values */
typedef int VarVal;

#define EMPTY_MARKING(size)			(MarkVal*)calloc(size, sizeof(MarkVal));
#define EMPTY_ASSIGNMENT(size)		(VarVal*)calloc(size, sizeof(VarVal));

#define MARK_INF					INT_MAX

/** Efficient representation of PetriNet */
class PetriNet
{
	PetriNet(int places, int transitions, int variables);
public:
	~PetriNet();
	/** Fire transition if possible and store result in result */
	bool fire(unsigned int transition,
			  const MarkVal* marking,
			  const VarVal* assignment,
			  MarkVal* resultMarking,
			  VarVal* resultAssignment) const;
	/** Fire transition if possible and store result in result (Respect MARK_INF */
	bool fireWithMarkInf(unsigned int transition,
			  const MarkVal* marking,
			  const VarVal* assignment,
			  MarkVal* resultMarking,
			  VarVal* resultAssignment) const;
	unsigned int numberOfTransitions() const {return _nTransitions;}
	unsigned int numberOfVariables() const {return _nVariables;}
	unsigned int numberOfPlaces() const {return _nPlaces;}
	/** Returns the place offset, or -1 if not found */
	int lookupPlace(const std::string& name) const;
	/** Returns the variable offset, or -1 if not found */
	int lookupVariable(const std::string& name) const;
private:
	std::vector<std::string> _places;
	std::vector<std::string> _transitions;
	std::vector<std::string> _variables;
	/** Number of x variables
	 * @remarks We could also get this from the _places vector, but I don't see any
	 * any complexity garentees for this type.
	 */
	size_t _nPlaces, _nTransitions, _nVariables;
	/** Transition matrix, see transition vector */
	MarkVal* _tm;
	/** Get a transition vector
	 * @remarks On form [p1-][p2-]...[p1+][p2+]...
	 *			where p1- is number of tokens to be taken from p1.
	 *			and p1+ is the number of tokens to given to p1.
	 */
	const MarkVal* _tv(unsigned int t) const {return _tm + t * _nPlaces * 2;}
	MarkVal* _tv(unsigned int t) {return _tm + t * _nPlaces * 2;}

	VarVal* _ranges;
	PQL::Condition** _conditions;
	PQL::AssignmentExpression** _assignments;
	friend class PetriNetFactory;
};

} // PetriEngine

#endif // PETRINET_H
