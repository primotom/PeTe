/* PeTe - Petri Engine exTremE
 * Copyright (C) 2011  Jonas Finnemann Jensen <jopsen@gmail.com>,
 *                     Thomas Søndersø Nielsen <primogens@gmail.com>,
 *                     Lars Kærlund Østergaard <larsko@gmail.com>,
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PETRINETSCENE_H
#define PETRINETSCENE_H

#include <QGraphicsScene>
#include <QUndoStack>
#include <QUndoGroup>
#include <QStandardItemModel>

#include <PetriEngine/AbstractPetriNetBuilder.h>

#include "NetItem.h"

class NetItem;
class PlaceItem;
class TransitionItem;
class ArcItem;
class ValidationIssuesModel;
class QueryModel;
class PNMLBuilder;
class BooleanVariableModel;
class PetriNetView;
class TAPAALExportBuilder;

/** A TAPN document, the model behind the view */
class PetriNetScene : public QGraphicsScene
{
    Q_OBJECT
public:
	/** Type for manageing scene mode */
	enum Mode {
		/** Pointer mode, where items can be modified and moved */
		PointerMode,
		/** Insert place mode, where a click inserts a new place */
		InsertPlaceMode,
		/** Insert transition mode, where a click inserts a new transition */
		InsertTransitionMode,
		/** Insert arc mode, where two clicks inserts a new arc */
		InsertArcMode
	};

	/** Create new instance of TapnScene
	 * @param undoGroup		UndoGroup to add undostack to
	 * @param parent		Widget to die, with unless killed otherwise.
	 */
	explicit PetriNetScene(QUndoGroup* undoGroup, PetriNetView *parent = 0);

	/** Notify the document that it's active  */
	void setActive(){
		this->_undoStack->setActive();
	}

	Mode mode() {return _mode;}
	void setMode(Mode mode);

	/** Get the undo stack for this scene */
	QUndoStack* undoStack() { return _undoStack; }

	/** Find a net item with the given name */
	NetItem* findNetItem(const QString& name);

	void addNetItem(NetItem* item);

	void removeNetItem(NetItem* item);

	/** Add new arc */
	void addArc(ArcItem* arc);
	/** Remove arc*/
	void removeArc(ArcItem* arc);
	/** Find an arc from start to end, NULL if none */
	ArcItem* findArc(NetItem* start, NetItem* end);

	/** Produce a copy using builder */
	void produce(PetriEngine::AbstractPetriNetBuilder* builder);
	//TODO: Make common base class with abstract addQuery method for PNMLBuilder and TAPAALExportBuilder
	/** Save, this extension method will also save queries */
	void produce(PNMLBuilder* builder);
	/** Save, this extension method will also save queries */
	void produce(TAPAALExportBuilder* builder);

	/** Get the variables */
	BooleanVariableModel* variables() { return this->_variables; }

	/** Get query model */
	QueryModel* queries() { return _queries; }

	/** Get validation issue model */
	ValidationIssuesModel* validationIssues() { return _validationIssues; }

	/** Check if this is a valid available identifier */
	bool isValidAvailableIdentifier(const QString& id) const;

	/** List variables names */
	QStringList variableNames() const;
	QStringList placeNames() const;

	/** Get the view that owns this scene */
	PetriNetView* view() { return _view; };

	/** File name of current document, empty if none */
	const QString& filename() const { return _filename; }
	/** Set the filename of the current document, empty of none */
	void setFilename(const QString& filename) { _filename = filename; }

	/** Get a list of selected NetItems*/
	QList<NetItem*> selectedNetItems();
private:
	void transitionItemDoubleClickEvent(TransitionItem* t);
	void placeItemDoubleClickEvent(PlaceItem* place);
	void arcItemDoubleClickEvent(ArcItem* arc);

	/** Filename of the current document, empty if none */
	QString _filename;

	/** Model representing the variables of the net */
	BooleanVariableModel* _variables;

	/** Model holding queries */
	QueryModel* _queries;

	/** Model holding validation issues, from last call to validate */
	ValidationIssuesModel* _validationIssues;

	/** Undo stack */
	QUndoStack* _undoStack;

	/** The view for this scene */
	PetriNetView* _view;

	Mode _mode;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void keyPressEvent(QKeyEvent* event);
private:
	/** Get NetItem at pos, or close by if none are there, NULL if none */
	NetItem* netItemCloseAt(QPointF pos, NetEntity::ItemType type = NetEntity::UndefinedItem);
	/** Get item at pos, or close by if none are there, NULL if none */
	QGraphicsItem* itemCloseAt(QPointF pos);
	void insertPlacePress(QGraphicsSceneMouseEvent* event);
	void insertTransitionPress(QGraphicsSceneMouseEvent* event);
	void insertArcPress(QGraphicsSceneMouseEvent* event);
	void insertArcMove(QGraphicsSceneMouseEvent* event);
	void insertArcRelease(QGraphicsSceneMouseEvent* event);
	void pointerPress(QGraphicsSceneMouseEvent* event);
	void pointerMove(QGraphicsSceneMouseEvent* event);
	void pointerRelease(QGraphicsSceneMouseEvent* event);
	/** Unselect item at mouseReleaseEvent if ControlModifier is down */
	bool unselectItemAtReleaseIfCtrlDown;
	/** Current selection rectangle, NULL if none */
	QGraphicsRectItem* selectionRect;
	/** Mode to enter at pointer release, if there's a modifier down */
	Mode modeAtReleaseIfModifier;
signals:
	void modeChanged(PetriNetScene::Mode mode);
	void validationIssuesFound();
public slots:
	/** Delete selected items (creates an undo command) */
	void deleteSelection();
	void updateSceneRect();
	/** Refresh the model with validation issues */
	void validate();
	/** Scroll to, select or otherwise highlight the issue */
	void showValidationIssue(const QModelIndex& index);
	/** Automatically arrange the layout */
	void autoArrange();
	/** Align selected items along alignOn */
	void alignSelectItems(Qt::Orientation alignOn);
};

#endif // PETRINETSCENE_H
