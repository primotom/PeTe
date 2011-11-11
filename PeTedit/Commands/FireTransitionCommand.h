#ifndef FIRETRANSITIONCOMMAND_H
#define FIRETRANSITIONCOMMAND_H

#include <QUndoCommand>
#include <QStringList>

class PetriNetScene;

/** Command for firing a transition */
class FireTransitionCommand : public QUndoCommand
{
public:
	FireTransitionCommand(PetriNetScene* scene,
						  QString transition,
						  QStringList* trace);
	~FireTransitionCommand();
	QString transition() {return _transition;}
	void undo();
	void redo();
private:
	PetriNetScene* _scene;
	QString _transition;
	QStringList* _trace;
};

#endif // FIRETRANSITIONCOMMAND_H
