#pragma once

// doubly-linked list node
template<class Elem>
struct Link {
    Link* prev;     // previous link
    Link* succ;     // successor (next) link
    Elem val;       // the value
};

// doubly-linked list
template<class Elem>
class list {
public:
    class iterator;     // member type: iterator

    list() :first(nullptr), sz(0) {}

    ~list() {
        while (first) {
            auto next = first->succ;
            delete first;
            first = next;
        }
    }

    // iterator to first element
    iterator begin() { return iterator(first); }

    // iterator to one beyond last element
    iterator end() { return iterator(nullptr); }

    iterator insert(iterator p, const Elem& v); // insert v into list before p
    iterator erase(iterator p);                 // remove p from the list

    void push_back(const Elem& v);      // insert v at end
    void push_front(const Elem& v);     // insert v at front
    void pop_front();   // remove the first element
    void pop_back();    // remove the last element

    Elem& front();  // the first element
    Elem& back();   // the last element

    int size() const { return sz; };

private:
    Link<Elem>* first;
    int sz;
};

template<class Elem>
class list<Elem>::iterator {
public:
    explicit iterator(Link<Elem>* p) :curr(p) {}
    iterator& operator++() { curr = curr->succ; return *this; }  // forward
    iterator& operator--() { curr = curr->prev; return *this; }  // backward
    Elem& operator*() { return curr->val; }  // get value (dereference)
    bool operator==(const iterator& b) const { return curr == b.curr; }
    bool operator!=(const iterator& b) const { return curr != b.curr; }

private:
    Link<Elem>* curr;  // current link
};

template<class Elem>
void list<Elem>::push_front(const Elem& v) {
    if (!first)
        first = new Link<Elem>{nullptr, nullptr, v};
    else
        first = first->prev = new Link<Elem>{nullptr, first, v};
    ++sz;
}
