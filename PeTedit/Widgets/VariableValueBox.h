#ifndef VARIABLEVALUEBOX_H
#define VARIABLEVALUEBOX_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>

/** Wrapper class around QComboBox and QSpinBox to ease variable transition */
class VariableValueBox : public QWidget {
	Q_OBJECT
public:
	VariableValueBox(QWidget* parent = 0) : QWidget(parent) {
		_spinBoxActive = true;
		_comboBox = new QComboBox(this);
		_comboBox->hide();
		_spinBox = new QSpinBox(this);
	}

	/** Getters */
	QSpinBox* spinBox(){
		return _spinBox;
	}
	QComboBox* comboBox(){
		return _comboBox;
	}
	/** Which widget is active */
	void setSpinBoxActive(bool spinbox){
		if(spinbox && !_spinBoxActive){
			_comboBox->hide();
			_spinBox->show();
		} else if(!spinbox && _spinBoxActive){
			_spinBox->hide();
			_comboBox->show();
		}
	}

private:
	bool _spinBoxActive;
	QSpinBox* _spinBox;
	QComboBox* _comboBox;
};

#endif // VARIABLEVALUEBOX_H
