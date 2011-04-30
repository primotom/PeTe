#include "StateConstraints.h"

namespace PetriEngine{
namespace Structures{

/** Attempts to merge two StateConstraints, return NULL if can't be merged */
StateConstraints* StateConstraints::merge(const StateConstraints* c) const{
	StateConstraints* nc = new StateConstraints(nPlaces, nVars);
	for(size_t p = 0; p < nPlaces; p++){
		if(!nc->setPlaceMax(p, pcs[p].max) ||
		   !nc->setPlaceMax(p, c->pcs[p].max) ||
		   !nc->setPlaceMin(p, pcs[p].min) ||
		   !nc->setPlaceMin(p, c->pcs[p].min)){
			delete nc;
			return NULL;
		}
	}
	for(size_t v = 0; v < nVars; v++){
		if(!nc->setVarMax(v, vcs[v].max) ||
		   !nc->setVarMax(v, c->vcs[v].max) ||
		   !nc->setVarMin(v, vcs[v].min) ||
		   !nc->setVarMin(v, c->vcs[v].min)){
			delete nc;
			return NULL;
		}
	}
	return nc;
}

/** True, if every marking satisfied by subset is also satisfied by this */
bool StateConstraints::isSuperSet(const StateConstraints* subset) const{
	for(size_t p = 0; p < nPlaces; p++){
		if(pcs[p].max < subset->pcs[p].max)
			return false;
		if(pcs[p].min > subset->pcs[p].min)
			return false;
	}
	for(size_t v = 0; v < nVars; v++){
		if(vcs[v].max < subset->vcs[v].max)
			return false;
		if(vcs[v].min > subset->vcs[v].min)
			return false;
	}
	return true;
}

void StateConstraints::reset(){
	for(size_t p = 0; p < nPlaces; p++){
		pcs[p].max = CONSTRAINT_INFTY;
		pcs[p].min = -CONSTRAINT_INFTY;
	}
	for(size_t v = 0; v < nVars; v++){
		vcs[v].max = CONSTRAINT_INFTY;
		vcs[v].min = -CONSTRAINT_INFTY;
	}
}

/** Merge the two sets of StateConstraints such that one from A and one from B is always satisfied, when one in the return value is
 * @remarks This will take ownership of the provided StateConstraints, and delete them or own them
 */
std::vector<StateConstraints*> StateConstraints::mergeAnd(const std::vector<StateConstraints*>& A, const std::vector<StateConstraints*>& B){
	std::vector<StateConstraints*> retval;
	typedef std::vector<StateConstraints*>::const_iterator iter;
	for(iter a = A.begin(); a != A.end(); a++){
		for(iter b = B.begin(); b != B.end(); b++){
			StateConstraints* nc = (*a)->merge(*b);
			if(nc)
				retval.push_back(nc);
		}
		delete *a;
	}
	for(iter b = B.begin(); b != B.end(); b++)
		delete *b;
	return retval;
}

/** Merge the two sets of StateConstraints such that either on from A or B is always satisfied, when one in the return value is
 * @remarks This will take ownership of the provided StateConstraints, and delete them or own them
 */
std::vector<StateConstraints*> StateConstraints::mergeOr(const std::vector<StateConstraints*>& A, const std::vector<StateConstraints*>& B){
	std::vector<StateConstraints*> retval, excluded;
	typedef std::vector<StateConstraints*>::const_iterator iter;
	for(iter a = A.begin(); a != A.end(); a++){
		bool exclude = false;
		for(iter b = B.begin(); b != B.end(); b++){
			exclude |= (*b)->isSuperSet(*a) && !(*b)->isEqual(*a);
			if(exclude) break;
		}
		if(!exclude)
			retval.push_back(*a);
		else
			excluded.push_back(*a);
	}
	for(iter b = B.begin(); b != B.end(); b++){
		bool exclude = false;
		for(iter a = A.begin(); a != A.end(); a++){
			exclude |= (*a)->isSuperSet(*b);
			if(exclude) break;
		}
		if(!exclude)
			retval.push_back(*b);
		else
			excluded.push_back(*b);
	}
	for(iter e = excluded.begin(); e != excluded.end(); e++)
		delete *e;
	return retval;
}

StateConstraints* StateConstraints::requirePlaceMin(const PetriNet& net, int place, MarkVal value){
	StateConstraints* c = new StateConstraints(net);
	c->setPlaceMin(place, value);
	return c;
}

StateConstraints* StateConstraints::requirePlaceMax(const PetriNet& net, int place, MarkVal value){
	StateConstraints* c = new StateConstraints(net);
	c->setPlaceMax(place, value);
	return c;
}

StateConstraints* StateConstraints::requirePlaceEqual(const PetriNet& net, int place, MarkVal value){
	StateConstraints* c = new StateConstraints(net);
	c->setPlaceMax(place, value);
	c->setPlaceMin(place, value);
	return c;
}


StateConstraints* StateConstraints::requireVarMin(const PetriNet& net, int var, MarkVal value){
	StateConstraints* c = new StateConstraints(net);
	c->setVarMin(var, value);
	return c;
}

StateConstraints* StateConstraints::requireVarMax(const PetriNet& net, int var, MarkVal value){
	StateConstraints* c = new StateConstraints(net);
	c->setVarMax(var, value);
	return c;
}

StateConstraints* StateConstraints::requireVarEqual(const PetriNet& net, int var, MarkVal value){
	StateConstraints* c = new StateConstraints(net);
	c->setVarMax(var, value);
	c->setVarMin(var, value);
	return c;
}


} // Structures
} // PetriEngines
