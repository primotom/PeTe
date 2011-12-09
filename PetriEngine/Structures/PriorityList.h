#ifndef PRIORITYLIST_H
#define PRIORITYLIST_H

#include <assert.h>
#include <map>
#include <list>
#include <iterator>

using namespace std;

namespace PetriEngine{
namespace Structures{

template<class Item>
class PriorityList : map<double, std::list<Item> >
{
public:
	/** Iterator for the priority list */
	class StateIterator : iterator<input_iterator_tag, Item> {
	private:
		Item* _i;
		PriorityList* _list;
		Iter _listPos;
		bool _end;
	public:
		/** Iterator construction */
		StateIterator(Item* item, PriorityList* list, Iter listPos) {
			_i = item;
			_list = list;
			_listPos = listPos;
			_end = false;
		}
		StateIterator(const StateIterator& mitem) {
			_i = mitem._i;
			_list = mitem._list;
			_listPos = mitem._listPos;
			_end = mitem._end;
		}
		/** End iterator constructor */
		StateIterator() {
			_end = true;
			_i = NULL;
			_list = NULL;
			_listPos = NULL;
		}

		/** Get the item */
		Item* item(){ return _i; }

		/** Pre-Increment iterator */
		StateIterator& operator++() {
			if(_end) return *this;

			if(_listPos->second.size() > 1){
				//More elements in this list
				bool found = false;
				for(list<Item>::iterator it = _listPos->second.begin(); it != _listPos->second.end(); it++){
					if(_i == *it)
						found = true;
					else if(_i != *it && found){
						_i = *it;
						return *this;
					}
				}
			}
			_listPos++;
			if(_listPos == _list->end()){
				this->_end = true;
				return *this;
			}
			_i = *(_listPos->second.begin());
			return *this;
		}

		/** Post-Increment iterator */
		StateIterator operator++(Item) {
			StateIterator tmp(*this);
			this->operator ++();
			return tmp;
		}

		friend class PriorityList;
	};

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
	double topPriority() const {
		assert(_size > 0);
		assert(this->begin() != this->end());
		ConstIter it = this->begin();
		assert(!it->second.empty());
		return it->first;
	}

	/** Get iterator at the beginning of the list */
	StateIterator begin() {
		Iter mPos = this->begin();
		if(mPos == this->end())
			return StateIterator();
		Item i = mPos->second.front();
		return StateIterator(&i, this, mPos);
	}

	/** Get iterator past end of the list */
	StateIterator end() {
		return StateIterator();
	}

	/** Erase an item from the list */
	void remove(StateIterator it) {

	}
};

}
}

#endif // PRIORITYLIST_H
