#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QUndoGroup>
#include <QUndoStack>
#include <vector>

class PetriNetScene;

/** Model for representing fired transitions in a search trace */
class TraceModel : public QAbstractListModel
{
	Q_OBJECT
public:
	/** A trace element structure */
	struct TraceElement{
		TraceElement(){
			transitionName = "";
			transitionOffset = 0;
		}
	QString transitionName;
	unsigned int transitionOffset;
	};

	explicit TraceModel(PetriNetScene* net, QUndoGroup& undoGroup);

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section,
						Qt::Orientation orientation,
						int role) const;
	void populateTrace(std::vector<unsigned int> trace);

private:
	PetriNetScene* _net;
	QUndoStack* _undoStack;
	QStringList traceElements;
};


#endif // TRACEMODEL_H
