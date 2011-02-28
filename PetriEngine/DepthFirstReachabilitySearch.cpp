#include "DepthFirstReachabilitySearch.h"

namespace PetriEngine{

	bool isReachableDFS(CoverabilityTreeNode* coverTree, PetriNet net, Mark* initialMarking){

		// The new marking after firing some t.
		Mark* newMarking = new Mark[net.nPlaces()];

		bool deadEnd = true;
		// Fire each transition
		for(int t = 0; t< net.nTransitions(); t++){

			if(net.fire(t, initialMarking, newMarking)){
				deadEnd = false;
				// Create child with pointer to parent
				CoverabilityTreeNode* child = new CoverabilityTreeNode(coverTree, t, newMarking);

				// Check if newMarking same as some other marking
				bool isOld = child->findDuplicate(net);

				// Add child too tree
				coverTree->add(child);

				if(isOld){
					child->setOld(true);
					continue;
				} else {
					// add to hash map
					return isReachableDFS(child, net, newMarking);
				}
			}

		}

		if(deadEnd){
			// No t's enabled, so the node is "dead"
			coverTree->setDeadEnd(true);
		}
		return false;
	}

	/** Checks for reachability with DFS */
	bool reachable(PetriNet net, Mark* initialMarking){

		// Root node
		CoverabilityTreeNode* coverTree = new CoverabilityTreeNode(initialMarking);
		return isReachableDFS(coverTree, net, initialMarking);
	}

} // PetriEngine
