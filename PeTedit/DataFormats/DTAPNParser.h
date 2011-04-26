#ifndef DTAPNPARSER_H
#define DTAPNPARSER_H

#include <PetriEngine/DTAPN/AbstractDTAPNBuilder.h>

#include <QXmlStreamReader>
#include <QIODevice>
#include <QHash>
#include <QList>

/** PNML Parser for DTAPNs */
class DTAPNParser
{
	/** Types if nodes that is assigned an id */
	enum NodeType{
		Place,
		Transition
	};
	/** Tuple of name and place/transition designation */
	struct NodeName{
		NodeName(){}
		NodeName(NodeType type, QString name){
			this->type = type;
			this->name = name;
		}
		NodeType type;
		QString name;
	};
	/** Arc entry, because we can't create arcs before all nodes are created */
	struct ArcEntry{
		ArcEntry(){}
		ArcEntry(QString src, QString dst, int startInterval, int endInterval){
			this->src = src;
			this->dst = dst;
			this->startInterval = startInterval;
			this->endInterval = endInterval;
		}
		QString src;
		QString dst;
		/** start interval for the arc (discard if out-arc) */
		int startInterval;
		/** end interval for the arc (discard if out-arc) */
		int endInterval;
	};
public:
	struct Query{
		QString name;
		QString query;
	};

	DTAPNParser() { builder = NULL; }
	/** Parse input and build result with builder */
	void parse(QIODevice* input, PetriEngine::DTAPN::AbstractDTAPNBuilder* builder);
	QList<Query> getQueries() { return _queries; }
private:
	void pnml();
	void net();
	void place();
	void transition();
	void arc();
	void query();
	void queries();
	void position(qreal& x, qreal& y);
	void value(QString& value);
	/** Builder for creating new petri net */
	PetriEngine::DTAPN::AbstractDTAPNBuilder* builder;
	QXmlStreamReader xml;
	QList<ArcEntry> arcs;
	/** Maps Ids to names */
	QHash<QString, NodeName> idmap;
	QList<Query> _queries;
};


#endif // DTAPNPARSER_H
