#ifndef BOOLEANVARIABLEMODEL_H
#define BOOLEANVARIABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
class PetriNetScene;

/** Model for representing boolean variables in a PN */
class BooleanVariableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	/** A variable structure */
	struct Variable{
		Variable(){
			name = "";
			value = false;
		}
		QString name;
		bool value;
	};

	explicit BooleanVariableModel(PetriNetScene* net);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section,
						Qt::Orientation orientation,
						int role) const;

	bool setData(const QModelIndex &index,
				 const QVariant &value,
				 int role);
	void emitDataChanged(int row);
	Qt::ItemFlags flags(const QModelIndex &index) const;

	void addVariable(QString name = "", bool value = false);
	void removeVariable(const QModelIndex& index);

	const Variable& variable(int row) const{
		return _variables.at(row);
	}

	QStringList variableNames() const{
		QStringList names;
		foreach(const Variable& var, _variables)
			names.append(var.name);
		return names;
	}
private:
	QList<Variable> _variables;
	PetriNetScene* _net;
};

#endif // BOOLEANVARIABLEMODEL_H
