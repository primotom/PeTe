#include "protocolparser.h"
#include "../Misc/QueryModel.h"
#include "../NetItems/PetriNetSceneBuilder.h"

#include <QtGlobal>
#include <QDebug>

#include <iostream>

void ProtocolParser::parse(QIODevice *input, PetriEngine::AbstractPetriNetBuilder *builder, AbstractQueryListBuilder* qBuilder){
	this->qBuilder = qBuilder;

	bool wasOpen = input->isOpen();
	if(!wasOpen)
		input->open(QIODevice::ReadOnly);
	xml.setDevice(input);
	this->builder = builder;

	idmap.clear();
	arcs.clear();

	while(xml.readNextStartElement()){
		if(xml.name() == "protocol"){
			parseprotocol();
		}else
			xml.skipCurrentElement();
	}
	//Create arcs
	foreach(ArcEntry entry, arcs){
		if(!idmap.contains(entry.src)){
			qDebug()<<"Error during parse: Arc from \""<<entry.src<<"\", which is not defined";
			continue;
		}
		if(!idmap.contains(entry.dst)){
			qDebug()<<"Error during parse: Arc to \""<<entry.dst<<"\", which is not defined";
			continue;
		}
		std::string src = idmap[entry.src].name.toStdString();
		std::string dst = idmap[entry.dst].name.toStdString();
		if(idmap[entry.src].type == Place && idmap[entry.dst].type == Transition)
			builder->addInputArc(src, dst, entry.weight);
		else if(idmap[entry.src].type == Transition && idmap[entry.dst].type == Place)
			builder->addOutputArc(src, dst, entry.weight);
		else{
			//TODO: Consider reporting an error
			qDebug()<<"Error duing parse: Arc from \""<<src.c_str()<<"\" to \""<<dst.c_str()<<"\" is invalid";
		}
	}

	idmap.clear();
	arcs.clear();

	if(!wasOpen)
		input->close();
	this->builder = NULL;

}

void ProtocolParser::parseprotocol(){
	while(xml.readNextStartElement()){
		if(xml.name() == "messages"){
			parseVariable();
		}else if(xml.name() == "role"){
			parserole();
		}else
			xml.skipCurrentElement();
	}
}

void ProtocolParser::parseVariable(){
	while(xml.readNextStartElement()){
		if(xml.name() == "message"){
			QString name = xml.readElementText(QXmlStreamReader::SkipChildElements);
			builder->addBoolVariable(name.toStdString(), false);
		}else
			xml.skipCurrentElement();
	}
}


void ProtocolParser::parserole(){

	QString roleName = xml.attributes().value("name").toString();
	while(xml.readNextStartElement()){
		if(xml.name() == "states"){
			parseStates(roleName);
		}else if(xml.name() == "rule"){
			parseRule(roleName);
		}else
			xml.skipCurrentElement();
	}
}


void ProtocolParser::parseStates(QString roleName){

	while(xml.readNextStartElement()){
		if(xml.name() == "state"){
			parseState(roleName);
		}else
			xml.skipCurrentElement();
	}
}

void ProtocolParser::parseState(QString roleName){
	qreal x = 0, y = 0;
	QString name = xml.readElementText(QXmlStreamReader::SkipChildElements);
	name = roleName + "_" + name;
	int initialMarking = 0;
	if(xml.attributes().value("type").toString() == "initial")
	initialMarking = 1;

	//Create place
	builder->addPlace(name.toStdString(), initialMarking, x, y);
	//Map id to name
	idmap[name] = NodeName(Place, name);
}

void ProtocolParser::parseRule(QString roleName){//TODO
	qreal x = 0, y = 0;
	QString name = xml.attributes().value("id").toString();
	name = roleName + "_" + name;
	QString conditions, assignments;

	while(xml.readNextStartElement()){
		if(xml.name() == "pre"){
			parsePre(conditions, roleName, name);
		}else if(xml.name() == "post"){
			parsePost(assignments, roleName, name);
		}else
			xml.skipCurrentElement();
	}
	//Create transition
	builder->addTransition(name.toStdString(),
						   conditions.toStdString(),
						   assignments.toStdString(),
						   x, y);

	//Map id to name
	idmap[name] = NodeName(Transition, name);
}

void ProtocolParser::parsePre(QString& pre, QString roleName, QString transName){
	while(xml.readNextStartElement()){
		if(xml.name() == "current_state"){
			QString source = xml.readElementText(QXmlStreamReader::SkipChildElements);
			arcs.append(ArcEntry(roleName+ "_" + source, transName, 1));
		}else if(xml.name() == "received_message"){
			QString message = xml.readElementText(QXmlStreamReader::SkipChildElements);
			if (pre != "")
				pre += " && ";
			pre += message;
		}else
			xml.skipCurrentElement();
	}
}

void ProtocolParser::parsePost(QString& post, QString roleName, QString transName){
	while(xml.readNextStartElement()){
		if(xml.name() == "next_state"){
			QString destination = xml.readElementText(QXmlStreamReader::SkipChildElements);
			arcs.append(ArcEntry(transName,roleName+ "_" + destination , 1));
		}else if(xml.name() == "send_message"){
			QString message = xml.readElementText(QXmlStreamReader::SkipChildElements); //TODO Check syntax
			post += message + ":= true; ";
		}else
			xml.skipCurrentElement();
	}
}

