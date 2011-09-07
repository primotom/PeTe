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
#include "EditQueryCommand.h"

#include "../Misc/QueryModel.h"

EditQueryCommand::EditQueryCommand(QueryModel *model,
								   int row,
								   const QueryModel::Query& query){
	_model = model;
	_row = row;
	_query = query;
}

void EditQueryCommand::undo(){
	swap();
}

void EditQueryCommand::redo(){
	swap();
}

void EditQueryCommand::swap(){
	QueryModel::Query q = _model->query(_row);
	_model->setQuery(_query, _row);
	_query = q;
}