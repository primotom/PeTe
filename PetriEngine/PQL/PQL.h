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
#ifndef PQL_H
#define PQL_H
#include "../PetriNet.h"
#include "../Structures/State.h"
#include <string>
#include <list>
#include <vector>

namespace llvm{
	class Value;
	class BasicBlock;
	class LLVMContext;
}

namespace PetriEngine{
namespace PQL{

class AnalysisContext;
class EvaluationContext;
class DistanceContext;
class ConstraintAnalysisContext;
class CodeGenerationContext;
class TAPAALConditionExportContext;
class MonotonicityContext;

/** Representation of a PQL error */
class ExprError{
	std::string _text;
	int _offset;
	int _length;
public:
	ExprError(std::string text = "", int offset = -1, int length = 0){
		_text = text;
		_offset = offset;
		_length = length;
	}
	/** Human readable explaination of the error */
	const std::string& text() const {return _text;}
	/** Offset in the source, -1 if undefined */
	int offset() const {return _offset;}
	/** length in the source, 0 if not applicable */
	int length() const {return _length;}

	/** Convert error to string */
	std::string toString() const {
		return "Parsing error \"" + text() + "\"";
	}
	/** True, if this is a default created ExprError without any information */
	bool isEmpty() const {
		return _text.empty() && _offset == -1 && _length == 0;
	}
};



/** Representation of an expression */
class Expr{
public:
	/** Types of expressions */
	enum Types{
		/** Binary addition expression */
		PlusExpr,
		/** Binary subtraction expression */
		SubtractExpr,
		/** Binary multiplication expression */
		MultiplyExpr,
		/** Unary minus expression */
		MinusExpr,
		/** Literal boolean expression */
		BooleanExpr,
		/** Literal integer expression */
		LiteralExpr,
		/** Identifier expression */
		IdentifierExpr,
		/** Undefined type */
		Undefined
	};
public:
	/** Virtual destructor, an expression should know it subexpressions */
	virtual ~Expr();
	/** Perform context analysis */
	virtual void analyze(AnalysisContext& context) = 0;
	/** True, if the expression is p-free */
	virtual bool pfree() const;
	/** Evaluate the expression given marking and assignment */
	virtual int evaluate(const EvaluationContext& context) const = 0;
	/** Generate LLVM intermediate code for this expr  */
	virtual llvm::Value* codegen(CodeGenerationContext& context) const;
	/** Convert expression to string */
	virtual std::string toString() const = 0;
	/** Expression type */
	virtual Types type() const;
	/** Scale all nested literals by factor */
	virtual void scale(int factor) = 0;
	/** Explores expressions to find bad places and variables */
	virtual void isBad(MonotonicityContext& context) = 0;
};

/** Base condition */
class Condition{
public:
	/** Virtual destructor */
	virtual ~Condition();
	/** Evaluate condition */
	bool evaluate(Structures::State& state) const;
	/** Perform context analysis  */
	virtual void analyze(AnalysisContext& context) = 0;
	/** Evaluate condition */
	virtual bool evaluate(const EvaluationContext& context) const = 0;
	/** Analyze constraints for over-approximation */
	virtual void findConstraints(ConstraintAnalysisContext& context) const;
	/** Generate LLVM intermediate code for this condition  */
	virtual llvm::Value* codegen(CodeGenerationContext& context) const;
	/** Convert condition to string */
	virtual std::string toString() const = 0;
	/** Export condition to TAPAAL query (add EF manually!) */
	virtual std::string toTAPAALQuery(TAPAALConditionExportContext& context) const;
	/** Get distance to query */
	virtual double distance(DistanceContext& context) const = 0;
	/** Scale all nested literals by factor */
	virtual void scale(int factor) = 0;
	/** Explores assignment conditions to identify possible bad and good places */
	virtual void monoStatus(MonotonicityContext& context, std::vector<int>& variableStatus, int varIndex) = 0;
	/** Explores conditions to find bad places and variables */
	virtual void isBad(MonotonicityContext& context) = 0;
};

/** Assignment expression */
class AssignmentExpression{
private:
	struct VariableAssignment{
		std::string identifier;
		int offset;
		Expr* expr;
		Condition* cond;
	};
	typedef std::list<VariableAssignment>::iterator iter;
	typedef std::list<VariableAssignment>::const_iterator const_iter;
public:
	void prepend(const std::string& identifier, Expr* expr){
		VariableAssignment va;
		va.offset = -1;
		va.identifier = identifier;
		va.expr = expr;
		va.cond = NULL;
		assignments.push_front(va);
	}
	void prepend(const std::string& identifier, Condition* cond){
		VariableAssignment va;
		va.offset = -1;
		va.identifier = identifier;
		va.expr = NULL;
		va.cond = cond;
		assignments.push_front(va);
	}

	void analyze(AnalysisContext& context);
	/** Evaluate the assignment expression */
	void evaluate(const MarkVal* m,
				  const VarVal* a,
				  const BoolVal* b,
				  VarVal* result_a,
				  BoolVal* result_b,
				  VarVal* ranges,
				  size_t nInts,
				  size_t nBools) const;
	/** Perform status check on variables for MonotonicityContext */
	void monoStatus(MonotonicityContext &context, std::vector<int> &status);
	std::string toString(){
		std::string t;
		for(iter it = assignments.begin(); it != assignments.end(); it++){
			t = t + it->identifier + " := " + it->expr->toString() + "; ";
		}
		return t;
	}
private:
	std::list<VariableAssignment> assignments;
};

} // PQL
} // PetriEngine

#endif // PQL_H
