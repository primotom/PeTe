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
#include "PQL.h"

#include "Contexts.h"

namespace PetriEngine {
namespace PQL {

/************ Escape methods on base classes ************/

Expr::~Expr(){}

bool Expr::pfree() const { return false; }

llvm::Value* Expr::codegen(CodeGenerationContext&) const { return NULL; }

Expr::Types Expr::type() const{ return Undefined; }

Condition::~Condition(){}

bool Condition::evaluate(Structures::State &state) const{
	return evaluate(EvaluationContext(state.marking(), state.intValuation(), state.boolValuation()));
}

void Condition::findConstraints(ConstraintAnalysisContext &context) const{
	context.canAnalyze = false;
}

llvm::Value* Condition::codegen(CodeGenerationContext&) const{
	return NULL;
}

std::string Condition::toTAPAALQuery(TAPAALConditionExportContext &context) const{
	context.failed = true;
	return " false ";
}

/*********** Assignment analysis ***********/

void AssignmentExpression::analyze(AnalysisContext& context){
	for(iter it = assignments.begin(); it != assignments.end(); it++){
		AnalysisContext::ResolutionResult result = context.resolve(it->identifier);
		if(result.success && !result.isPlace){
			it->offset = result.offset;
		}else if(result.isPlace){
			context.reportError(ExprError("You cannot assign to a place!"));
		}else{
			context.reportError(ExprError("Variable for assignment could not be resolved!"));
		}
		if(it->expr)
			it->expr->analyze(context);
		else
			it->cond->analyze(context);
	}
}

void AssignmentExpression::monoStatus(MonotonicityContext& context, std::vector<int>& status){
	for(const_iter it = assignments.begin(); it != assignments.end(); it++){
		if(it->cond)
			it->cond->monoStatus(context, status, it->offset);
	}
}

} // PQL
} // PetriEngine
