#ifndef PRIORITYLIST_H
#define PRIORITYLIST_H

#include <assert.h>
#include <map>
#include <list>

using namespace std;

namespace PetriEngine{
namespace Structures{

template<class Item>
class PriorityList : map<double, std::list<Item> >
{
private:
	typedef typename map::iterator Iter;
	typedef typename map::const_iterator ConstIter;
	size_t _size;
public:
	PriorityList(){
		this->clear();
	}

	/** Test if list is empty */
	bool empty() const{
		return this->empty();
	}

	/** Get the size of the list */
	size_t size() const{
		return _size;
	}

	/** Delete everything in the list */
	void clear(){
		this->clear();
		_size = 0;
	}

	/** Push a new item into the list */
	void push(double priority, const Item& item){
		_size += 1;
		Iter it = this->find(priority);
		if(it != this->end())
			it->second.push_back(item);
		else
			this->insert(pair<double, list<Item> >(priority, list<Item>(1, item)));
	}

	/** Pop item with lowest priority */
	Item pop(){
		assert(_size > 0);
		_size -= 1;
		assert(this->begin() != this->end());
		Iter it = this->begin();
		assert(!it->second.empty());
		Item retVal = it->second.back();
		it->scond.pop_back();
		if(it->second.empty())
			this->erase(it);
		return retVal;
	}

	/** Get the top most item */
	Item& top(){
		assert(size > 0);
		assert(this->begin() != this->end());
		Iter it = this->begin();
		assert(!it->second.empty());
		return it->second.back();
	}

	/** Get priority of top most item in queue */

};

}
}

#endif // PRIORITYLIST_H
