#include "PetriNetSceneBuilder.h"

#include "PetriNetScene.h"
#include "PlaceItem.h"
#include "ArcItem.h"
#include "TransitionItem.h"

PetriNetSceneBuilder::PetriNetSceneBuilder(QUndoGroup* undoGroup, QObject* sceneParent){
	scene = new PetriNetScene(undoGroup, sceneParent);
}

void PetriNetSceneBuilder::addPlace(const std::string &name, int tokens, double x, double y){
	PlaceItem* item = new PlaceItem(QPointF(x,y), name.c_str());
	item->setTokens(tokens);
	scene->addNetItem(item);
}

void PetriNetSceneBuilder::addVariable(const std::string& name, int initialValue, int range){
	scene->addVariable(name.c_str(), initialValue, range);
}

void PetriNetSceneBuilder::addTransition(const std::string &name,
										 const std::string &conditions,
										 const std::string &assignments,
										 double x,
										 double y){
	TransitionItem* item = new TransitionItem(QPointF(x,y), name.c_str());
	//TODO: Perhaps use a new constructor for this?
	item->setPreConditions(conditions.c_str());
	item->setPostConditions(assignments.c_str());
	scene->addNetItem(item);
}

void PetriNetSceneBuilder::addInputArc(const std::string &place, const std::string &transition, int weight){
	arcs.append({place.c_str(), transition.c_str(), weight});
}

void PetriNetSceneBuilder::addOutputArc(const std::string &transition, const std::string &place, int weight){
	arcs.append({transition.c_str(), place.c_str(), weight});
}


/** Extension method for loading queries */
void PetriNetSceneBuilder::addQuery(const QueryModel::Query& query){
	scene->queries()->insertQuery(query);
}

PetriNetScene* PetriNetSceneBuilder::makeScene(){
	foreach(ArcEntry entry, arcs){
		NetItem* start = scene->findNetItem(entry.src);
		NetItem* end = scene->findNetItem(entry.dst);
		if(start && end && start->type() != end->type()){
			ArcItem* item = new ArcItem(start, end);
			item->setWeight(entry.weight);
			scene->addArc(item);
		}
	}
	arcs.clear();
	return scene;
}
