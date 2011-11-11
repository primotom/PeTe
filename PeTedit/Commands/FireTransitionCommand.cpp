#include "FireTransitionCommand.h"

FireTransitionCommand::FireTransitionCommand(PetriNetScene *scene,
											 QString transition,
											 QStringList *trace){
	_scene = scene;
	_transition = transition;
	_trace = trace;
}

FireTransitionCommand::~FireTransitionCommand(){}

void FireTransitionCommand::undo(){
	_trace->pop_back();
	//TODO: Do something with Scene
}

void FireTransitionCommand::redo(){
	_trace->push_back(transition);
	//TODO: Do something with Scene
}
