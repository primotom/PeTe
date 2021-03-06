TEMPLATE	 = app

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

QMAKE_CXXFLAGS	+= -std=c++0x $$system(llvm-config --cxxflags all)# | sed -e "s/-fomit-frame-pointer//")
QT		  		+= core gui svg
INCLUDEPATH		+= ../
LIBS			+= -L../ -lPetriEngine -llpsolve55 -lcolamd $$system(llvm-config --libs all) -lproc
PRE_TARGETDEPS	+= ../PetriEngine
CONFIG			+= link_pkgconfig
PKGCONFIG		+= igraph

SOURCES	   += \
	Main.cpp \
	MainWindow.cpp \
	NetItems/PetriNetScene.cpp \
	NetItems/PlaceItem.cpp \
	NetItems/NetItem.cpp \
	NetItems/ArcItem.cpp \
	NetItems/TransitionItem.cpp \
	NetItems/PetriNetView.cpp \
	Commands/InsertTransitionCommand.cpp \
	Commands/InsertPlaceCommand.cpp \
	Commands/MoveItemsCommand.cpp \
	Commands/InsertArcCommand.cpp \
	DataFormats/PNMLParser.cpp \
    Dialogs/EditArcDialog.cpp \
    Dialogs/EditPlaceDialog.cpp \
	Dialogs/QueryDialog.cpp \
	Dialogs/EditTransitionDialog.cpp \
    Widgets/QueryEdit.cpp \
    Widgets/QueryHighlighter.cpp \
    Commands/RenameItemCommand.cpp \
    Commands/EditPlaceCommand.cpp \
    Commands/EditArcCommand.cpp \
    Commands/EditTransitionCommand.cpp \
    Widgets/VariableDelegate.cpp \
    Commands/EditQueryCommand.cpp \
    Commands/AddRemoveQueryCommand.cpp \
    NetItems/PetriNetSceneBuilder.cpp \
    DataFormats/PNMLBuilder.cpp \
    Misc/ValidationIssuesModel.cpp \
    Misc/QueryModel.cpp \
    Misc/ProgressViewDelegate.cpp \
    Misc/QueryThread.cpp \
    Misc/VariableModel.cpp \
    Widgets/MemoryMonitor.cpp \
    Commands/AutoArrangeNetCommand.cpp \
    Misc/QueryListBuilder.cpp \
    DataFormats/DTAPNParser.cpp \
    Dialogs/ImportDTAPNDialog.cpp \
    Commands/DeleteItemsCommand.cpp \
    DataFormats/TAPAALExportBuilder.cpp \
	DataFormats/protocolparser.cpp \
    Misc/BooleanVariableModel.cpp \
    Widgets/BooleanVariableDelegate.cpp



HEADERS		+= \
	NetItems/TransitionItem.h \
	NetItems/NetItem.h \
	NetItems/PetriNetScene.h \
	NetItems/ArcItem.h \
	NetItems/PlaceItem.h \
	NetItems/PetriNetView.h \
	Commands/Commands.h \
	Commands/MoveItemsCommand.h \
	Commands/InsertPlaceCommand.h \
	Commands/InsertTransitionCommand.h \
	MainWindow.h \
    DataFormats/PNMLParser.h \
    Commands/InsertArcCommand.h \
    Dialogs/EditArcDialog.h \
    Dialogs/EditPlaceDialog.h \
    Dialogs/EditTransitionDialog.h \
	Dialogs/QueryDialog.h \
    Widgets/QueryEdit.h \
    Widgets/QueryHighlighter.h \
    Commands/RenameItemCommand.h \
    Commands/EditPlaceCommand.h \
    Commands/EditArcCommand.h \
    Commands/EditTransitionCommand.h \
    Widgets/VariableDelegate.h \
    Commands/EditQueryCommand.h \
    Commands/AddRemoveQueryCommand.h \
    NetItems/PetriNetSceneBuilder.h \
    DataFormats/PNMLBuilder.h \
    Misc/ValidationIssuesModel.h \
    Misc/QueryModel.h \
    Misc/ProgressViewDelegate.h \
    Misc/QueryThread.h \
    Misc/CustomDataRoles.h \
    Misc/VariableModel.h \
    Widgets/MemoryMonitor.h \
    Commands/AutoArrangeNetCommand.h \
    Misc/AbstractQueryListBuilder.h \
    Misc/QueryListBuilder.h \
    DataFormats/DTAPNParser.h \
    Dialogs/ImportDTAPNDialog.h \
    Commands/DeleteItemsCommand.h \
    DataFormats/TAPAALExportBuilder.h \
	DataFormats/protocolparser.h \
	Widgets/VariableValueBox.h \
    Misc/BooleanVariableModel.h \
    Widgets/BooleanVariableDelegate.h



FORMS		+= MainWindow.ui \
    Dialogs/EditArcDialog.ui \
    Dialogs/EditPlaceDialog.ui \
    Dialogs/EditTransitionDialog.ui \
    Dialogs/QueryDialog.ui \
    Dialogs/ImportDTAPNDialog.ui

TARGET		 = ../PeTe

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    Widgets/QueryReservedWords.list
