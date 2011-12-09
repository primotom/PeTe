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
	class StateIterator : public iterator<input_iterator_tag, Item> {
	private:
		typedef typename map<double, std::list<Item> >::iterator Iter;

		Item _i;
		PriorityList* _list;
		Iter _listPos;
		bool _end;
	public:
		friend class PriorityList;

		/** Iterator construction */
		StateIterator(Item item, PriorityList* list, Iter listPos) {
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
		}

		/** Get the item */
		Item item(){ return _i; }

		/** Pre-Increment iterator */
		StateIterator& operator++() {
			if(_end) return *this;

			if(_listPos->second.size() > 1){
				//More elements in this list
				bool found = false;
				for(typename list<Item>::iterator it = _listPos->second.begin(); it != _listPos->second.end(); it++){
					if(_i == *it)
						found = true;
					else if(_i != *it && found){
						_i = *it;
						return *this;
					}
				}
			}
			_listPos++;
			if(_listPos == _list->mapEnd()){
				this->_end = true;
				return *this;
			}
			_i = *(_listPos->second.begin());
			return *this;
		}

		/** Post-Increment iterator */
		StateIterator operator++(int) {
			StateIterator tmp(*this);
			this->operator ++();
			return tmp;
		}

		/** Comparison operators */
		bool operator==(const StateIterator& rhs) {return _i==rhs._i;}
		bool operator!=(const StateIterator& rhs) {return _i!=rhs._i;}
	};

private:
	typedef typename map<double, std::list<Item> >::iterator Iter;
	typedef typename map<double, std::list<Item> >::const_iterator ConstIter;
	typedef map<double, std::list<Item> > Map;
	size_t _size;

	Iter mapEnd(){
		return Map::end();
	}

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
		if(it != Map::end())
			it->second.push_back(item);
		else
			this->insert(pair<double, list<Item> >(priority, list<Item>(1, item)));
	}

	/** Pop item with lowest priority */
	Item pop(){
		assert(_size > 0);
		_size -= 1;
		assert(Map::begin() != Map::end());
		Iter it = Map::begin();
		assert(!it->second.empty());
		Item retVal = it->second.back();
		it->second.pop_back();
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
		Iter mPos = map<double, std::list<Item> >::begin();
		if(mPos == map<double, std::list<Item> >::end())
			return StateIterator();
		Item i = mPos->second.front();
		return StateIterator(i, this, mPos);
	}

	/** Get iterator past end of the list */
	StateIterator end() {
		return StateIterator();
	}

	/** Erase an item from the list */
	void remove(StateIterator it) {
		Iter mPos = it._listPos;
		for(typename list<Item>::iterator lit = mPos->second.begin(); lit != mPos->second.end(); lit++){
			if(*lit == it._i)
				mPos->second.erase(lit);
				_size -= 1;
		}
	}
};

}
}

#endif // PRIORITYLIST_H
