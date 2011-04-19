#ifndef PQLPARSER_H
#define PQLPARSER_H

#include <string>

namespace PetriEngine { namespace PQL {

class Condition;
class AssignmentExpression;
struct SUMoQuery;

Condition* ParseQuery(const std::string& queryString);
SUMoQuery* ParseSUMoQuery(const std::string& queryString);
AssignmentExpression* ParseAssignment(const std::string& assignmentString);

}}
#endif // PQLPARSER_H
