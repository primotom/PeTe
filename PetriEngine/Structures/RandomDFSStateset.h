#ifndef RANDOMDFSSTATESET_H
#define RANDOMDFSSTATESET_H

#include "PQL/Contexts.h"
#include "OrderableStateSet.h"
#include <list>

namespace PetriEngine { namespace Structures {

/** A step in the reachability search */
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

/** Orderable State Set class which will feed states in a Random Depth First manner */
class RandomDFSStateSet : public OrderableStateSet {
public:
	/** Create the state set */
	RandomDFSStateSet(const PetriNet& net, PQL::MonotonicityContext* context)
		: OrderableStateSet(net, context) {}

	/** Add a state to the state set */
	bool add(State* state){return false;}

	bool add(State* state, unsigned int t);

	/** Get the next state to explore */
	State* getNextState(){return NULL;}
	RStep getNextStep();
	/** Get size of waiting */
	size_t waitingSize();

	RStep popWating();
	//void popT();
	RStep getWating(){return _stack.back();}


	void pushToWating(State* state);

private:
	typedef std::list<State*>::iterator iter;

	std::list<RStep> _stack;
	std::list<State*> _visited;


};

} // Structures
} // PetriEngine

#endif // RANDOMDFSSTATESET_H
