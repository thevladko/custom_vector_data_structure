#include<iostream>
#include<initializer_list>
#include<stdexcept>
#include<algorithm>
using namespace std;

//initial size of the container. Standard for a default constructor
const size_t initial_size = 4;

template <class T>
class Vector {
//type aliases
public:
class Iterator;
class Constiterator;
using value_type = T;
using size_type = size_t;
using difference_type = ptrdiff_t;
using reference = value_type&;
using const_reference = const value_type&;
using pointer = value_type*;
using const_pointer = const value_type*;
using iterator = Iterator;
using const_iterator = Constiterator;

//stored variables of the "vector" class
private:
	size_type sz; //variable for the current amount of elements in the containter
	size_type max_sz; //to track the maximum allocated memory for the container
 	pointer values; //array. which is a pointer to some place in memory

public:
	//default constructor
	Vector() : Vector(initial_size) {};
	// constructor with a certain size parameter
	Vector(size_type new_sz) : sz{0} {
		if(new_sz < initial_size) {
			new_sz = initial_size;
		}
		values = new value_type[new_sz]; //allocating memory for the container
		max_sz = new_sz;
	}

	//constructor with initializer list
	Vector(initializer_list<value_type> li) : Vector(li.size()) {
		for(auto& elem : li) {
			values[sz++] = elem;
		}
	}

	//copy constructor
	Vector(const Vector& copy) : Vector(copy.size()) {
		for(size_t i = 0; i < copy.size(); ++i) {
			push_back(copy[i]);
		}
	}

	//destructor
	~Vector() { delete [] values; }

	//assignment operator
	Vector operator=(Vector src) {
		swap(max_sz, src.max_sz);
		swap(sz, src.sz);
		swap(values, src.values);
		return *this;
	}

	//OPERATORS
	reference operator[] (size_t _index) {
		if(_index >= sz) {
			throw runtime_error("Out of bounds!");
		}
		return values[_index];
	}

	const_reference operator[] (size_t _index) const {
		if(_index >= sz) {
			throw runtime_error("Out of bounds");
		}
		return values[_index];
	}

	//OTHER METHODS
	size_type size() const {
		return this->sz;
	}

	//optional method, just to see what the current maximum size of the container is.
	size_type get_max_size() const{
		return this->max_sz;
	}

	void push_back(value_type value) {
		if(sz >= max_sz) {
			reserve(max_sz*2);
		}
		values[sz++] = value;
	}

	void reserve(size_type new_size) {
		if (sz < max_sz) { return; } //if max_sz > then the actual size -> no need to reserve any space, we have enough.
		if (sz == 0) { return; }
		pointer buffer = new value_type[new_size]; //create a buffer pointer and initialize it by allocating memory with the "new_size"
		for(size_type i = 0; i < sz; ++i) { //copy all the values from "values" array into the buffer
			buffer[i] = values[i];
		}
		delete [] values; //release the memory that was allocated for the "values" array
		values = buffer; //make the "values" pointer now point at where "buffer" points
		max_sz = new_size;
	}

	bool empty() const { return sz == 0; }

	void pop_back() {
		if(sz == 0) {
			return;
		} else {
			--sz;
		}
	}

	void clear() {
		sz = 0;
	}

	void shrink_to_fit() {
		//TODO: this can be encapsulated into a separate function
		pointer buffer = new value_type[this->sz];
		for(size_type i = 0; i < sz; ++i) {
			buffer[i] = values[i];
		}
		delete [] values;
		values = buffer;
		max_sz = sz;
	}


//ITERATORS
public:
   class Constiterator {
public:
   using value_type = Vector::value_type;
   using difference_type = Vector::difference_type;
   using size_type = Vector::size_type;
   using reference = Vector::reference;
   using pointer = Vector::pointer;
   using iterator_category = std::forward_iterator_tag;

   private:
     pointer current;
     const Vector<value_type>* v;
     mutable int anzahl{0};

   public:
     Constiterator() {}
     Constiterator(pointer ptr, const Vector* b) : current{ptr}, v{b} {}

     const_reference operator * () {
       if(current < v->begin().current || current >= v->end().current ) {
         throw runtime_error("Trying to dereference iterator that is out of bounds!");
       }
       return *current;
     }

      friend bool operator == (const Constiterator &lop, const Constiterator &rop) {
       return lop.current == rop.current;
     }

     Constiterator& operator ++ () {
       if(current < v->begin().current || current >= v->end().current) {
         throw runtime_error("Iterator is out of bounds!. Cannot increment anymore!");
       }
       current++;
       return *this;
     }

     Constiterator operator ++ (int) {
       if(current < v->begin().current || current >= v->end().current) {
         throw runtime_error("Iterator is out of bounds!. Cannot increment anymore!");
       }
       Constiterator old = *this;
       current++;
       return old;
     }

     const_reference operator * () const {
       if(current < v->begin().current || current >= v->end().current) {
         throw runtime_error("Trying to dereference an iterator that is out of bounds!");
       }
       return *current;
     }

     const_pointer operator -> () const {
       if(current < v->begin().current || current >= v->end().current) {
         throw runtime_error("Trying to dereference an iterator that is out of bounds!");
       }
       return current;
     }

     friend bool operator != (const Constiterator &lop, const Constiterator &rop) {
       return lop.current != rop.current;
     }

     friend difference_type operator - (const Constiterator &lop, const Constiterator &rop) {
       return lop.current - rop.current;
     }

   };

public:
  class Iterator {

public:
   using value_type = Vector::value_type;
   using difference_type = Vector::difference_type;
   using size_type = Vector::size_type;
   using reference = Vector::reference;
   using pointer = Vector::pointer;
   using iterator_category = std::forward_iterator_tag;

private:
    pointer current;
    Vector<value_type>* v;
    mutable int anzahl{0};

public:

    Iterator() {} //for the Vector::Iterator name = it;

    Iterator(pointer initial, Vector* b) : current{initial}, v{b} {}

    operator Constiterator() const {
      return Constiterator(current, v);
    }

    friend bool operator == (const Iterator &lop, const Iterator &rop) {
      return lop.current == rop.current;
    }

    friend bool operator != (const Iterator &lop,const Iterator &rop) {
      return lop.current != rop.current;
    }

    Iterator& operator ++ () {
      if(current < v->begin().current || current >= v->end().current) {
        throw runtime_error("Iterator is out of bounds!. Cannot increment anymore!");
      }
      current++;
      return *this;
    }

    Iterator operator ++ (int) {
      if(current < v->begin().current || current >= v->end().current) {
        throw runtime_error("Iterator is out of bounds!. Cannot increment anymore!");
      }
      Iterator old = *this;
      current++;
      return old;
    }

    reference operator * () {
      if(current < v->begin().current || current >= v->end().current) {
        throw runtime_error("Trying to dereference an iterator that is out of bounds!");
      }
      return *current;
    }

    pointer operator -> () const {
      if(current < v->begin().current || current >= v->end().current) {
        throw runtime_error("Trying to dereference an iterator that is out of bounds! operator -> ");
      }
      return current;
    }

    friend difference_type operator - (const Iterator &lop, const Iterator &rop) {
      return lop.current - rop.current;
    }

    Iterator& operator -- () {
      current--;
      return *this;
    }

    Iterator operator -- (int) {
      Iterator old = *this;
      current--;
      return old;
    }

    bool operator < (const Iterator& rop) const {
      return this->current < rop.current;
    }

    bool operator <= (const Iterator& rop) const {
      return this->current <= rop.current;
    }

    bool operator > (const Iterator& rop) const {
      return this->current > rop.current;
    }

    bool operator >= (const Iterator& rop) const {
      return this->current >= rop.current;
    }
  };

  Iterator erase(const Iterator &pos) {
    auto diff = pos - begin();

    if (diff<0 || static_cast<size_type>(diff)>=sz) {
        throw runtime_error("Iterator out of bounds!");
    }
    size_type current{static_cast<size_type>(diff)};
    for (size_type i{current}; i < sz-1; ++i) {
        values[i]=values[i+1];
    }
    --sz;
    return Iterator(values + current, this);
  }

  Constiterator erase(const Constiterator &pos) {
    auto diff = pos - begin();

    if (diff<0 || static_cast<size_type>(diff)>=sz) {
        throw runtime_error("Iterator out of bounds!");
    }
    size_type current{static_cast<size_type>(diff)};
    for (size_type i{current}; i < sz-1; ++i) {
        values[i]=values[i+1];
    }
    --sz;
    return Constiterator(values + current, this);
  }

  Iterator insert(const Iterator &pos, const_reference val) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) >= sz) {
        throw runtime_error("Iterator out of bounds!");
    }
    size_type current{static_cast<size_type>(diff)};
    if (sz >= max_sz) {
        reserve(max_sz*2);
    }

    for (size_type i{sz}; i-->current;) {
        values[i+1] = values[i];
    }
    values[current] = val;
    ++sz;
    return Iterator(values + current, this);
  }

  Constiterator insert(const Constiterator &pos, const_reference val) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) >= sz) {
        throw runtime_error("Iterator out of bounds!");
    }
    size_type current{static_cast<size_type>(diff)};
    if (sz >= max_sz) {
        reserve(max_sz*2);
    }

    for (size_type i{sz}; i-->current;) {
        values[i+1] = values[i];
    }
    values[current] = val;
    ++sz;
    return Constiterator(values + current, this);
  }


  Iterator begin() {
    return Iterator(values, this);
  }

  Iterator end() {
    return Iterator(values + sz, this);
  }

  Constiterator begin() const {
    return Constiterator(values, this);
  }

  Constiterator end() const {
    return Constiterator(values + sz, this);
  }

};

template <class T>
ostream& operator<<(ostream& out, const Vector<T>& v) {
  out << '[';
  for(auto it = v.begin(); it != v.end();) {
    out << *it;
    if(++it != v.end()) {
      out << ',';
    }
  }
  out << ']';
  return out;
}
