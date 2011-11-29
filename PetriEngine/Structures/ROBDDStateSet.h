#ifndef ROBDDSTATESET_H
#define ROBDDSTATESET_H

#include "PQL/Contexts.h"
#include <list>
#include <map>
namespace PetriEngine { namespace Structures {
using namespace std;
enum ROBDDreturn{
	ROBDDtrue,
	ROBDDupdate,
	ROBDDinsert,
	ROBDDnewbranch
};

class Node {
public:
	Node(int index, PQL::MonotonicityContext* context, const PetriNet& net){
		_index = index;
		_net = &net;

	}

	ROBDDreturn leq(State* X,bool tlExist){
		bool mytl = false;
		bool b = false;

		if(!tlExist && true){//TODO implement
			mytl = true;
			tlExist = true;
		}
		if(b){
			ROBDDreturn a;
			//Get smallest leaf larger or equal to x.index
			list<Node>::iterator biggest = NULL;
			for(list<Node>::iterator it = _leafs.begin(); it != _leafs.end();it++){
				int Xindex;
				if(_index > _net->numberOfBoolVariables())//TODO: Update if using inValuation
					Xindex = X->marking(_index - _net->numberOfBoolVariables());
				else
					Xindex = X->boolValuation(_index);

				if(Xindex < it->first){
					if(biggest == NULL || (*it).value < (*biggest).value )
						biggest = it;
				}
			}

			if(biggest != NULL)
				a = (*biggest).leq(X,tlExist);

		}



	}

private:
	const PetriNet* _net;
	bool _good;
	list<Node> _leafs;
	int _index;
	int _value;
};

class ROBDDStateSet {
public:
	ROBDDStateSet(const PetriNet& net, PQL::MonotonicityContext* context){
	_context = context;
	_net = &net;
	}

	bool add(State* state){
		if(_root->leq(state,false) == ROBDDtrue)
			return false;
		else
			return true;
	}


private:
	const PetriNet* _net;
	PQL::MonotonicityContext* _context;
	Node* _root;
};

}}
#endif // ROBDDSTATESET_H
