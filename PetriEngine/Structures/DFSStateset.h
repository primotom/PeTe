#ifndef DFSSTATESET_H
#define DFSSTATESET_H


#include "PQL/Contexts.h"
#include <list>
#include "OrderableStateSet.h"
namespace PetriEngine { namespace Structures {


class DFSStateSet : public OrderableStateSet {
public:
	DFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context):
	OrderableStateSet(net,context){
	_countSkip =0;
	_countAdd =0;

	}

	bool add(State* state);

	bool add(Step* state);

	//Is some good boolean variable or marking in s1 greater than the one in s2, where true > false
	bool greater(State* s1, State* s2);

	//Is some good boolean variable in s1 greater than the one in s2, where true > false
	bool greaterBool(State* s1, State* s2);

	std::list<State*> States() {return _states;}
	Step popWating(){return stack.pop_back();}
	Step getWating(){return stack.back();}

	void writeStatistics();

	/** A step in the reachability search */
	struct Step{
		Step(Structures::State* s, unsigned int t){
			state = s;
			this->t = t;
		}
		Structures::State* state;
		unsigned int t;
	};

protected:
	//Is S1 less or equal to S2
	bool leq(State* s1, State* s2);

	//Is S1 less than S2
	bool less(State* s1, State* s2);

	const PetriNet* _net;
	PQL::MonotonicityContext* _context;

	//visited list
	std::list<State*> _states;

	//wating list
	std::list<Step> stack;

	int _countSkip;
	int _countAdd;



};

}}

#endif // DFSSTATESET_H
