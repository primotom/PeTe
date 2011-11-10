#include "BooleanVariableDelegate.h"
#include <QtGui>

BooleanVariableDelegate::BooleanVariableDelegate(QObject *parent)
	: QItemDelegate(parent) {}

QWidget* BooleanVariableDelegate::createEditor(QWidget *parent,
											   const QStyleOptionViewItem&,
											   const QModelIndex&) const{
	QLineEdit* editor = new QLineEdit(parent);
	return editor;
}

void BooleanVariableDelegate::setEditorData(QWidget *editor,
											const QModelIndex &index) const{
	if(index.column() == 0) { // Name column
		QString value = index.model()->data(index, Qt::EditRole).toString();
		QLineEdit* txt = static_cast<QLineEdit*>(editor);
		txt->setText(value);
	}
}

void BooleanVariableDelegate::setModelData(QWidget *editor,
										   QAbstractItemModel *model,
										   const QModelIndex &index) const{
	if(index.column() == 0){
		QLineEdit* txt = static_cast<QLineEdit*>(editor);
		QString value = txt->text();
		model->setData(index, value, Qt::EditRole);
	}
}

void BooleanVariableDelegate::updateEditorGeometry(QWidget *editor,
												   const QStyleOptionViewItem &option,
												   const QModelIndex&) const{
	editor->setGeometry(option.rect);
}
