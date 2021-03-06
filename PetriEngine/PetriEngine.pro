TEMPLATE		= lib

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

QMAKE_CXXFLAGS += $$system(llvm-config --cxxflags all)# | sed -e "s/-fomit-frame-pointer//")
QT			   -= gui core
CONFIG		   += staticlib link_pkgconfig
PKGCONFIG	   += igraph

SOURCES		   += \
	StandardOutputProgressReporter.cpp \
	PetriNet.cpp \
	Reachability/DepthFirstReachabilitySearch.cpp \
    PetriNetBuilder.cpp \
    ValidationBuilder.cpp \
	Reachability/ReachabilitySearchStrategy.cpp \
    PQL/Expressions.cpp \
    PQL/PQL.cpp \
    PQL/CompiledCondition.cpp \
    Structures/DistanceMatrix.cpp \
    LayoutBuilder.cpp \
    Reachability/RandomDFS.cpp \
    Reachability/HeuristicDFS.cpp \
    Reachability/BreadthFirstReachabilitySearch.cpp \
    DTAPN/DTAPNTranslator.cpp \
    DTAPN/DTAPNPrinter.cpp \
    Reachability/BestFirstReachabilitySearch.cpp \
    Structures/StateConstraints.cpp \
    Reachability/LinearOverApprox.cpp \
    Reachability/UltimateSearch.cpp \
    Reachability/RandomQueryGenerator.cpp \
    Reachability/StateSearch.cpp \
    Reachability/MonoDFS.cpp \
    Reachability/MonoDFSState.cpp \
    Reachability/MonoDFSBool.cpp \
    Reachability/MonoBFS.cpp \
    Reachability/MonoRandomDFS.cpp \
	Reachability/MonoBestFirstReachabilitySearch.cpp \
    Reachability/MonoNewBestFS.cpp \
    Reachability/IndexedSearch.cpp \
    Structures/IndexedStateSet.cpp \
	#Reachability/CoverabilityTreeNode.cpp
    Structures/DFSStateset.cpp \
    Structures/BFSOrderableStateSet.cpp \
    Structures/BestFSOrderableStateSet.cpp \
    Structures/RandomDFSStateset.cpp \
    Structures/IndexedBFSStateSet.cpp \
    Reachability/IndexedBFS.cpp \
    Structures/DFSIndexedStateSet.cpp \
    Reachability/IndexedDFS.cpp \
    Structures/IndexedBestFSStateSet.cpp \
    Reachability/IndexedBestFS.cpp \
    Structures/IndexedRFSStateSet.cpp \
    Reachability/IndexedRFS.cpp

HEADERS		   += \
	ProgressReporter.h \
	PetriNet.h \
	StandardOutputProgressReporter.h \
    PQL/PQLParser.h \
    Reachability/ReachabilitySearchStrategy.h \
    PetriNetBuilder.h \
    AbstractPetriNetBuilder.h \
    Reachability/ReachabilityResult.h \
    ValidationBuilder.h \
	Reachability/DepthFirstReachabilitySearch.h \
	Structures/State.h \
	Structures/StateSet.h \
    PQL/PQL.h \
    PQL/Expressions.h \
    Structures/BlockStack.h \
    Structures/PriorityQueue.h \
    PQL/CompiledCondition.h \
    PQL/Contexts.h \
    Structures/DistanceMatrix.h \
    Structures/StateAllocator.h \
    LayoutBuilder.h \
    Reachability/RandomDFS.h \
    Reachability/HeuristicDFS.h \
    Reachability/BreadthFirstReachabilitySearch.h \
    AbstractDTAPNBuilder.h \
    DTAPN/DTAPNTranslator.h \
    DTAPN/AbstractDTAPNBuilder.h \
    DTAPN/DTAPNPrinter.h \
    Reachability/BestFirstReachabilitySearch.h \
    Structures/StateConstraints.h \
    Reachability/LinearOverApprox.h \
    Structures/EnhancedPriorityQueue.h \
    Structures/Scale.h \
    Structures/BitField.h \
    Structures/BoundedStateAllocator.h \
    Reachability/UltimateSearch.h \
    Reachability/RandomQueryGenerator.h \
    Reachability/StateSearch.h \
    Structures/OrderableStateSet.h \
    Structures/NaiveListStateSet.h \
    Reachability/MonoDFS.h \
    Reachability/MonoDFSState.h \
    Reachability/MonoDFSBool.h \
    Reachability/MonoBFS.h \
    Reachability/MonoRandomDFS.h \
    Reachability/IndexedSearch.h \
    Reachability/MonoBestFirstReachabilitySearch.h \
    Reachability/MonoNewBestFS.h \
    Reachability/IndexedSearch.h \
    Structures/IndexedStateSet.h \
	#Reachability/CoverabilityTreeNode.h
    Structures/BFSOrderableStateSet.h \
    Structures/DFSStateset.h \
    Structures/BestFSOrderableStateSet.h \
    Structures/PriorityList.h \
    Structures/RandomDFSStateset.h \
    Structures/IndexedBFSStateSet.h \
    Reachability/IndexedBFS.h \
    Structures/DFSIndexedStateSet.h \
    Reachability/IndexedDFS.h \
    Structures/IndexedBestFSStateSet.h \
    Reachability/IndexedBestFS.h \
    Structures/IndexedRFSStateSet.h \
    Reachability/IndexedRFS.h

TARGET			= ../PetriEngine

include(flex.pri)
include(bison.pri)

FLEXSOURCES		+= \
	PQL/PQLQueryTokens.l \
	PQL/PQLAssignmentTokens.l \
	PQL/SUMoQueryTokens.l \
	PQL/PQLConditionTokens.l \
	PQL/PQLConditionAssignmentTokens.l
BISONSOURCES	+= \
	PQL/PQLQueryParser.y \
	PQL/PQLAssignmentParser.y \
	PQL/SUMoQueryParser.y \
	PQL/PQLConditionParser.y \
	PQL/PQLConditionAssignmentParser.y
