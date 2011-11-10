#ifndef BOOLEANVARIABLEDELEGATE_H
#define BOOLEANVARIABLEDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QStringList>

class BooleanVariableDelegate : public QItemDelegate{
	Q_OBJECT
public:
	BooleanVariableDelegate(QObject* parent=0);

	QWidget *createEditor(QWidget *parent,
						  const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;
	void setEditorData(QWidget *editor,
					   const QModelIndex &index) const;
	void setModelData(QWidget *editor,
					  QAbstractItemModel *model,
					  const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,
							  const QStyleOptionViewItem &option,
							  const QModelIndex &index) const;
private:
};

#endif // BOOLEANVARIABLEDELEGATE_H
