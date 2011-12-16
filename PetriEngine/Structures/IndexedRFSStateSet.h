#ifndef INDEXEDRFSSTATESET_H
#define INDEXEDRFSSTATESET_H

#include "PQL/Contexts.h"
#include "OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {
struct RStep{
	RStep(Structures::State* s, const PetriNet* net){
		state = s;

		int succ[net->numberOfTransitions()];
		for(int i = 0; i <  net->numberOfTransitions();i++){
			succ[i] = i;
		}
		int random;
		int i;
		do {
			random = rand() % net->numberOfTransitions();
			i = random;
			do{
				if(succ[i] >= 0){
					t.push_back(succ[i]);
					succ[i] = -1;
					i++;
					break;
				}
				i = (i+1) % net->numberOfTransitions();
			} while(i != random);
		} while(i != random);

	}

	Structures::State* state;
	bool operator==(const RStep& rhs) {return state==rhs.state;}
	std::list<unsigned int> t;
};
/** Indexed State Set class which will feed states in a random depth first manner */
class IndexedRFSStateSet : public OrderableStateSet {
public:
	IndexedRFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context, bool variance = true)
		: OrderableStateSet(net, context) {
		_variance = variance;
	}

	/** Add a state to the state set */
	bool add(State* state){return false;}

	bool add(State* state, unsigned int t);


	/** Get the next state to explore */
	State* getNextState(){return NULL;}
	RStep getNextStep();

	/** Get size of waiting */
	size_t waitingSize();

	/** Push a state to waiting without checking */
	RStep popWating();
	//void popT();
	RStep getWating(){return _stack.back();}

	void pushToWating(State* state);

private:
	typedef std::pair<int, State*> IndexedState;
	typedef std::list<IndexedState>::iterator visitedIter;
	typedef std::list<State*>::iterator waitingIter;

	std::list<RStep> _stack;
	std::list<IndexedState> _visited;

	bool _variance;
};

} // Structures
} // PetriEngine

#endif // INDEXEDRFSSTATESET_H
