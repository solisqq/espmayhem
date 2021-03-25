#pragma once
#include "Arduino.h"

template<typename T, typename valT>
class ListIterator {
public:
    T itr{};
    explicit ListIterator(T node) : itr(node) {}

    ListIterator() : itr(nullptr) {}
    void swap(ListIterator& other) noexcept {
        std::swap(itr, other.itr);
        T backup = itr;
        itr = other.itr;
        other.itr = backup;
    }
    ListIterator& operator++() {//preincrement
        if (itr == nullptr) {
            return *this;
        }
        itr = *itr + 1;
        return *this;
    }
    ListIterator operator++(int) { //postincrement
        if (itr == nullptr) {
            return ListIterator();
        }
        ListIterator backup(itr);
        itr = *itr + 1;
        return backup;
    }
    bool operator==(const ListIterator& rhs) const {
        return rhs.itr == this->itr;
    }
    bool operator!=(const ListIterator& rhs) const {
        return rhs.itr != this->itr;
    }
    const valT& operator*() const {
        return itr->val;
    }
    valT operator->() const {
        return itr.val;
    }
};

template<class Type>
class List {
    int Count=0;
    int MaximumSize = 0;
public:
    class Node {
    public:
        Type val;
        Node(){}
        Node(Type v):val(v){}
        Node* next = nullptr;
        Node* prev = nullptr;
        Node* operator+(int value) {
            Node* current = this;
            for (int i = 0; i < value; i++) {
                if (current == nullptr) break;
                current = current->next;
            }
            return current;
        }
        Node* operator++() { //PRE-increment
            return this->next;
        }
    };
    Node *front = nullptr, *back = nullptr;
    List(){}
    List(const List<Type>& v) {
        for(typename List<Type>::Node *it=v.top(); it!=nullptr; it=it->next) pushBack(it->val);
        
    }
    ~List() {
        Node* currentNode = this->front;
        while(currentNode) {
            Node* nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
    }
    int count() const {return Count;}
    ListIterator<Node*, Type> begin() const { return ListIterator<Node*, Type>(front); }
    ListIterator<Node*, Type> end() const { return ListIterator<Node*, Type>(); }
    void setMaxSize(int maxSize) {

    }
    void pushFront(Type value) {
        Count++;
        if(front==nullptr) {
            front = back = new Node(value);
            return;
        }
        Node* newItem = new Node(value);
        newItem->next = front;
        front->prev = newItem;
        front = newItem;
        if (MaximumSize > 0 && Count>=MaximumSize) 
            popBack();
        
    }
    void pushBack(Type value) {
        Count++;
        if(front==nullptr) {
            front = back = new Node(value);
            return;
        }
        
        Node* newItem = new Node(value);
        newItem->prev = back;
        back->next = newItem;
        back = newItem;
        if (MaximumSize > 0 && Count >= MaximumSize)
            popFront();
    }
    void popBack() {
        if(Count<=0) return;
        Count--;
        if(Count==0) {
            delete front;
            front = nullptr;
            return;
        }
        Node* backup = back;
        backup->prev->next = nullptr;
        back = backup->prev;
        delete backup;
    }
    void popFront() {
        if(Count<=0) return;
        Count--;
        if(Count==0) {
            delete front;
            front = nullptr;
            return;
        }
        front->next->prev = nullptr;
        Node* backup = front->next;
        delete front;
        front = backup;
    }
    void erase() {
        Node* current = front;
        Node* toDel;
        while(current!=nullptr) {
            toDel=current;
            current = current->next;
            delete toDel;
        }
        front = nullptr;
        back = nullptr;
        Count = 0;
    }
    Type getAndPopFront() {
        if(Count<=0) return (Type)0;
        Type toRet = front->val;
        Count--;
        if(Count==0) {
            delete front;
            front = nullptr;
            return toRet;
        }
        front->next->prev = nullptr;
        Node* backup = front->next;
        delete front;
        front = backup;
        return toRet;
    }
    bool remove(const Type &_val) {
        Node *current = front;
        while(current!=nullptr) {
            if(current->val==_val) {
                if(current==front) {
                    popFront();
                    return true;
                }
                if(current==back) {
                    popBack();
                    return true;
                }
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
                Count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    bool removeAll(const Type& _val) {
        bool deleted = false;
        while (remove(_val)) { deleted = true; }
        return deleted;
    }
    void clear() {
        Node* currentNode = this->front;
        Count = 0;
        while(currentNode) {
            Node* nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
        front = back = nullptr;
    }
    String toString() const {
        String toRet="";
        Node *current = front;
        while(current!=nullptr) {
            toRet+=String(current->val)+", ";
            current = current->next;
        }
        return toRet;
    }
    Type getFront() const {return front->val; }
    Type getBack() const {return last->val; }
    Node* top() const { return front; }
    Node* last() const { return back; }
    void move(List<Type>& list) {
        if (list.front == nullptr || list.back==nullptr) return;
        
        if (front == nullptr) {
            this->front = list.front;
            this->back = list.back;
            this->Count = list.Count;
        }
        else {
            this->back->next = list.front;
            list.front->prev = this->back;
            this->back = list.back;
            this->Count += list.Count;
        }
        list.front = nullptr;
        list.back = nullptr;
        list.Count = 0;
    }
    void Union(const List<Type>& list) {
        for (auto item : list) 
            this->pushBack(item);
        
    }
    List<Type> operator+(Type b) {
		pushBack(b);
        return this;
	}
    const Type& at(unsigned int index) const {
        if (index > count()) return last()->val;
        Node* current = top();
        for (int i = 0; i < index; i++)
            current = current->next;

        return current->val;
    }
    Type& operator[](unsigned int index) {
        if(index>count()) return last()->val;
        Node* current = top();
        for(int i=0; i<index; i++) 
            current = current->next;
        
        return current->val;
    }
    /*virtual double calcAvg(int itemsCount=0) {
        if(itemsCount==0) itemsCount = this->count();
        if(itemsCount > this->count() || itemsCount==0) {return 0;}
        int i=0; 
        for(Node* it = this->last(); i<itemsCount; i++, it=it->prev) {
            it->val 
        }
        
    }*/
};