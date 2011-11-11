#include "TraceModel.h"

TraceModel::TraceModel(PetriNetScene *net, QUndoGroup* undoGroup){
	_net = net;
	_undoStack = new QUndoStack();
	undoGroup->setActiveStack(_undoStack);
}

int TraceModel::rowCount(const QModelIndex &parent) const{
	return traceElements.count();
}

QVariant TraceModel::data(const QModelIndex &index, int role) const{
	if(!index.isValid() || (role != Qt::EditRole && role != Qt::DisplayRole))
		return QVariant();

	return traceElements[index.row()];
}

QVariant TraceModel::headerData(int section,
								Qt::Orientation orientation,
								int role) const{
	if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
		return QVariant();
	return tr("Fired transition");
}

void TraceModel::populateTrace(std::vector<unsigned int> trace){
	//Do some stuff
}
