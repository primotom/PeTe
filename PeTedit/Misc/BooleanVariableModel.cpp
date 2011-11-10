#include "BooleanVariableModel.h"
#include "../NetItems/PetriNetScene.h"

#define COL_NAME	0
#define COL_VALUE	1
#define COL_COUNT	2

BooleanVariableModel::BooleanVariableModel(PetriNetScene* net)
	 : QAbstractTableModel(net)
{
	_net = net;
}

int BooleanVariableModel::rowCount(const QModelIndex &parent) const{
	if(parent.isValid()) return 0;
	return _variables.count();
}

int BooleanVariableModel::columnCount(const QModelIndex &parent) const{
	if(parent.isValid()) return 0;
	return COL_COUNT;
}

QVariant BooleanVariableModel::data(const QModelIndex &index, int role) const{
	if(!index.isValid() || (role != Qt::EditRole && role != Qt::DisplayRole))
		return QVariant();

	const Variable& variable = _variables.at(index.row());

	if(index.column() == COL_NAME)
		return variable.name;
	if(index.column() == COL_VALUE)
		return variable.value;

	return QVariant();
}

bool BooleanVariableModel::setData(const QModelIndex &index,
								   const QVariant &value,
								   int role){
	if(!index.isValid() || role != Qt::EditRole)
		return false;
	bool retVal = false;
	Variable& variable = _variables[index.row()];

	if(index.column() == COL_NAME){
		if(_net->isValidAvailableIdentifier(value.toString()) ||
		   variable.name == value.toString()){
			variable.name = value.toString();
			retVal = true;
		}
	}
	if(index.column() == COL_VALUE){
		variable.value = value.toBool();
		retVal = true;
	}

	emitDataChanged(index.row());
	return retVal;
}

QVariant BooleanVariableModel::headerData(int section,
										  Qt::Orientation orientation,
										  int role) const{
	if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
		return QVariant();
	if(section == COL_NAME)
		return tr("Name");
	if(section == COL_VALUE)
		return tr("Value");
	return QVariant();
}

Qt::ItemFlags BooleanVariableModel::flags(const QModelIndex &index) const{
	if(index.column() == COL_VALUE)
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void BooleanVariableModel::emitDataChanged(int row){
	emit dataChanged(this->index(row, 0), this->index(row, COL_COUNT));
}

/** Add a new variable to the model */
void BooleanVariableModel::addVariable(QString name, bool value){
	int row = _variables.size();
	this->beginInsertRows(QModelIndex(), row, row);

	int num = 0;
	while(!_net->isValidAvailableIdentifier(name)) {
		name = "x" + QString::number(num++);
	}

	Variable v;
	v.name = name;
	v.value = value;
	_variables.insert(row, v);
	this->endInsertRows();
}

/** Remove a variable from the model */
void BooleanVariableModel::removeVariable(const QModelIndex &index){
	this->beginRemoveRows(QModelIndex(), index.row(), index.row());
	_variables.removeAt(index.row());
	this->endRemoveRows();
}
