#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

template<class T>
class IList {
public:
    virtual void    add(T e)=0;
    virtual bool    add(int index, T e)=0;
    virtual bool    removeAt(int index)=0;
    virtual bool    removeItem(T item, void (*removeItemData)(T)=0)=0;
    virtual bool    empty()=0;
    virtual int     size()=0;
    virtual void    clear()=0;
    virtual T&      get(int index)=0;
    virtual int     indexOf(T item)=0;
    virtual bool    contains(T item)=0;
    virtual string  toString(string (*item2str)(T&)=0 )=0;
    virtual string  toStringBW(string (*item2str)(T&)=0 )=0;
};

template<class T>
class DLinkedList : public IList<T>{
public:
    class Node; //Forward declaration
    class Iterator; //Forward declaration
    class BWDIterator; //Forward declaration
protected:
    Node *head; //this node does not contain user's data
    Node *tail; //this node does not contain user's data
    int count;
    bool (*itemEqual)(T& lhs, T& rhs); //function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T>*); //function pointer: be called to remove items (if they are pointer type)
    
public:
    DLinkedList(
            void (*deleteUserData)(DLinkedList<T>*)=0, 
            bool (*itemEqual)(T&, T&)=0); 
    DLinkedList(const DLinkedList<T>& DLL);
    DLinkedList<T>& operator=(const DLinkedList<T>& DLL);
    ~DLinkedList();
    
    //Inherit from IList: BEGIN
    void    add(T e);
    bool    add(int index, T e);
    bool    removeAt(int index);
    bool    removeItem(T item, void (*removeItemData)(T)=0);
    bool    empty();
    int     size();
    void    clear();
    T&      get(int index);
    int     indexOf(T item);
    bool    contains(T item);
    string  toString(string (*item2str)(T&)=0 );
    string  toStringBW(string (*item2str)(T&)=0 );
    //Inherit from IList: END

    void    println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }

    void    printlnBW(string (*item2str)(T&)=0 ){
        cout << toStringBW(item2str) << endl;
    }

    void sEtDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T>*) = 0){
        this->deleteUserData = deleteUserData;
    }

    static void free(DLinkedList<T> *DLL){
        typename DLinkedList<T>::Iterator it = DLL->begin();
        while(it != DLL->end()){
            delete *it;
            it++;
        }
    }

    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }

    BWDIterator bbegin(){
        return BWDIterator(this, true);
    }
    BWDIterator bend(){
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T& )){
        if(itemEqual == 0) return lhs == rhs;
        else return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T>& DLL);
    void removeInternalData();
    Node* getPreviousNodeOf(int index);

//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
public:
    
    class Node{
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;
        
    public:
        Node(Node* next=0, Node* prev=0){
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node* next=0, Node* prev=0){
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };
    
//////////////////////////////////////////////////////////////////////
    class Iterator{
    private:
        DLinkedList<T>* pList;
        Node* pNode;
        
    public:
        Iterator(DLinkedList<T>* pList=0, bool begin=true){
            if(begin){
                if(pList !=0) this->pNode = pList->head->next;
                else pNode = 0;
            }
            else{
                if(pList !=0) this->pNode = pList->tail;
                else pNode = 0;
            }
            this->pList = pList;
        }
        
        Iterator& operator=(const Iterator& iterator){
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T)=0){
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node* pNext = pNode->prev; //MUST prev, so iterator++ will go to end
            if(removeItemData != 0) removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }
        
        T& operator*(){
            return pNode->data;
        }
        bool operator!=(const Iterator& iterator){
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            pNode = pNode->next;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };

    //Backward Iterator
    class BWDIterator{
    private:
        DLinkedList<T>* pList;
        Node* pNode;
        
    public:
        BWDIterator(DLinkedList<T>* pList=0, bool last=true){
            if(last){
                if(pList !=0) this->pNode = pList->tail->prev;
                else pNode = 0;
            }
            else{
                if(pList !=0) this->pNode = pList->head;
                else pNode = 0;
            }
            this->pList = pList;
        }
        
        BWDIterator& operator=(const BWDIterator& iterator){
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T)=0){
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node* pNext = pNode->next; //MUST next, so iterator-- will go to head
            if(removeItemData != 0) removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }
        
        T& operator*(){
            return pNode->data;
        }
        bool operator!=(const BWDIterator& iterator){
            return pNode != iterator.pNode;
        }
        // Prefix -- overload 
        BWDIterator& operator--(){
            pNode = pNode->prev;
            return *this; 
        }
        // Postfix -- overload 
        BWDIterator operator--(int){
            BWDIterator iterator = *this; 
            --*this; 
            return iterator; 
        }
    };
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
DLinkedList<T>::DLinkedList(
        void (*deleteUserData)(DLinkedList<T>*), 
        bool (*itemEqual)(T&, T&) ) {
    head = new Node();
    tail = new Node();
    head->next = tail; tail->next = 0;
    tail->prev = head; head->prev = 0;
    count = 0;
    this->itemEqual = itemEqual;
    this->deleteUserData = deleteUserData;
}

template<class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T>& DLL){
    this->head = new Node();
    this->tail = new Node();
    head->next = tail; tail->next = 0;
    tail->prev = head; head->prev = 0;
    copyFrom(DLL);
}

template<class T>
DLinkedList<T>& DLinkedList<T>::operator=(const DLinkedList<T>& DLL){
    removeInternalData();
    copyFrom(DLL);
    return *this;
}

template<class T>
DLinkedList<T>::~DLinkedList() {
    removeInternalData();
    if (head != NULL) delete head;
    if (tail != NULL) delete tail;
}

template<class T>
void DLinkedList<T>::add(T e) {
    Node* node = new Node(e, tail, tail->prev); 
    node->prev = tail->prev;
    tail->prev->next = node;
    tail->prev = node;
    node->next = tail;
    this->count +=1;
}

template<class T>
bool DLinkedList<T>::add(int index, T e) {
    if((index < 0) || (index > this->count)) return false;
    
    Node* newNode = new Node(e);

    int mid = this->count/2;
    if(index <= mid){
        Node* runNode = head;
        int k = -1;
        while (k < index-1) {
	  	    k++;
	  	    runNode = runNode->next;
        }
        
        runNode->next->prev = newNode;
        newNode->next = runNode->next;
        newNode->prev = runNode;
        runNode->next = newNode;
    }
    else {
        Node* runNode = tail;
        int k = this->count;
        while (k > index) {
	  	    k--;
	  	    runNode = runNode->prev;    
        }
       
        (runNode->prev)->next = newNode;
        newNode->prev = runNode->prev;
        newNode->next = runNode;
        runNode->prev = newNode;
    }
    this->count += 1;
    return true;
}

template<class T>
typename DLinkedList<T>::Node* DLinkedList<T>::getPreviousNodeOf(int index){
    Node* prevNode=0;
    int cursor;
    
    int mid = this->count/2;
    if(index < mid){
        //searching from: head
        prevNode = head;
        cursor = -1;
        while(cursor < index - 1){
            prevNode = prevNode->next;
            cursor += 1;
        }
    }
    else{
        //searching from: tail
        prevNode = tail;
        cursor = this->count;
        while(cursor >= index){
            prevNode = prevNode->prev;
            cursor -= 1;
        }
    }
    
    return prevNode;
}

template<class T>
bool DLinkedList<T>::removeAt(int index){
    if((index < 0) || (index > this->count - 1)) return false;

    T data;
    int mid = this->count/2;
    if(index < mid){
        Node* runNode = head;
        int k = -1;
        while (k < index-1) {
	  	    k++;
	  	    runNode = runNode->next;
        }
        Node* curNode = runNode->next;
        runNode->next->next->prev = runNode;
        runNode->next = runNode->next->next;
        curNode->next = 0;
        curNode->prev = 0;
        data = curNode->data;
        delete curNode;
    }
    else {
        Node* runNode = tail;
        int k = this->count;
        while (k > index) {
	  	    k--;
	  	    runNode = runNode->prev;    
        }
        runNode->prev->next = runNode->next;
        runNode->next->prev = runNode->prev;
        runNode->next = 0;
        runNode->prev = 0;
        data = runNode->data;
        delete runNode;
    
    }
    this->count -= 1;
    return true;
}

template<class T>
bool DLinkedList<T>::empty(){
    return (this->count == 0);
}

template<class T>
int  DLinkedList<T>::size(){
    return this->count;
}

template<class T>
void DLinkedList<T>::clear(){
    removeInternalData();

    head->next = tail; 
    head->prev = 0;
    tail->prev = head;
    tail->next = 0;
    this->count = 0;
}

template<class T>
T& DLinkedList<T>::get(int index){
    //if((index < 0) || (index > this->count - 1)) return ; throw exception here
    
    Node* prevNode = getPreviousNodeOf(index);
    
    //HERE: prevNode points to previous item (at index - 1); ready for get
    return prevNode->next->data;
}

template<class T>
int  DLinkedList<T>::indexOf(T item){
    Node* runNode = head;
    int idx = -1;
    while (runNode->next != tail) {
        idx++;
        if (DLinkedList<T>::equals((runNode->next)->data, item, this->itemEqual)) {
            return idx;
        }
        runNode = runNode->next;
    }
    return -1;
}

template<class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)){
    Node* runNode = head;
    while (runNode->next != tail) { //runNode->next
        if (DLinkedList<T>::equals( (runNode->next)->data, item, this->itemEqual)) {
            Node* curNode = runNode->next;
            runNode->next->next->prev = runNode;
            runNode->next = runNode->next->next;
            if (runNode->next == tail) tail->next = runNode;
            curNode->next = 0;
            curNode->prev = 0;

            if (removeItemData) removeItemData(curNode->data);
            delete curNode;
            this->count -= 1;
            return true;
        }
        runNode = runNode->next;
    }
    return false;
}


template<class T>
bool DLinkedList<T>::contains(T item){
    Node* runNode = head;
    while (runNode->next != tail) {
        if (DLinkedList<T>::equals((runNode->next)->data, item, this->itemEqual)) {
            return true;
        }
        runNode = runNode->next;
    }
    return false;
}

template<class T>
string DLinkedList<T>::toString(string (*item2str)(T&) ){
    if(this->count <= 0) return "";
    
    stringstream itemos;
    Node* ptr = head->next;
    while(ptr != tail){
        if(item2str != 0) itemos << item2str(ptr->data) << " ";
        else itemos << ptr->data << " ";

        ptr = ptr->next;
    }
    //remove the last ", "
    string itemstr = itemos.str();
    itemstr.pop_back();
    return itemstr;
}

template<class T>
string DLinkedList<T>::toStringBW(string (*item2str)(T&) ){
    if(this->count <= 0) return "";
    
    stringstream itemos;
    Node* ptr = tail->prev;
    while(ptr != head){
        if(item2str != 0) itemos << item2str(ptr->data) << " ";
        else itemos << ptr->data << " ";

        ptr = ptr->prev;
    }
    //remove the last ", "
    string itemstr = itemos.str();
    itemstr.pop_back();
    return itemstr;
}


template<class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T>& DLL){
    //Initialize this DLL to the empty condition
    this->count = 0;
    this->head->next = this->tail; this->tail->next = 0;
    this->tail->prev = this->head; this->head->prev = 0;

    //Copy pointers from "DLL"
    this->deleteUserData = DLL.deleteUserData;
    this->itemEqual = DLL.itemEqual;

    //Copy data from "DLL"
    Node* ptr= DLL.head->next;
    while(ptr != DLL.tail){
        this->add(ptr->data);
        ptr = ptr->next;
    }
}

template<class T>
void DLinkedList<T>::removeInternalData(){
    //Remove user's data stored in nodes
    if(this->deleteUserData != 0) deleteUserData(this);
    
    //Remove nodes
    if((head != 0) & (tail != 0)){
        Node* ptr = head->next;
        while(ptr != tail){
            Node* next = ptr->next;
            delete ptr;
            ptr = next;
        }
    }
}


class Symbol{
private:
    string command = "";
    string identifier = "";
    string type = "";
    int level = 0;
    string value = "";
public:
    Symbol(string command = "", string identifier = "", string type = "", string value = "", int level = 0) {
        this->command = command;
        this->identifier = identifier; 
        this->type = type;
        this->value = value;
        this->level = level;
    }
    Symbol(string& command) {
        this->command = command;
    }
    ~Symbol() {}

    friend ostream &operator<<( ostream &os, const Symbol& symb);
    //static section  
    static bool equals(Symbol& lhs, Symbol& rhs); //with pointer to symbol
    static bool equals(Symbol*& lhs, Symbol*& rhs); //with pointer to symbol
    static string toString(Symbol& symb);
    static string toString(Symbol*& symb);

    bool operator==(Symbol rhs) { // check identifier & level 
        return  (this->identifier == rhs.identifier) &&
                (this->level == rhs.level);
    }

    bool operator>(Symbol rhs) {
        return (this->level > rhs.level);
    }

    bool operator<(Symbol rhs) {
        return (this->level < rhs.level);
    }

    bool operator>=(Symbol rhs) {
        return (this->level >= rhs.level);
    }

    bool operator<=(Symbol rhs) {
        return (this->level <= rhs.level);
    }
    
    void print() {
        cout << this->identifier + "//" << this->level;
    }

    void println() {
        cout << this->identifier + "//" << this->level << endl;
    }

    string getCommand() {
        return this->command;
    }
    void sEtCommand(string command) {
        this->command = command; 
    }

    string getIdentifier() {
        return this->identifier;
    }
    void sEtIdentifier(string identifier) {
        this->identifier = identifier; 
    }

    string getType() {
        return this->type;
    }
    void sEtType(string type) {
        this->type = type; 
    }

    string getValue() {
        return this->value;
    }
    void sEtValue(string value) {
        this->value = value; 
    }

    int getLevel() {
        return this->level;
    }
    void sEtLevel(int level) {
        this->level = level; 
    }
};

ostream &operator<<( ostream &os, const Symbol& symb){
    os << symb.identifier + "//" << symb.level;
    return os;
}

bool Symbol::equals(Symbol& lhs, Symbol& rhs){
    return lhs == rhs;
}

bool Symbol::equals(Symbol*& lhs, Symbol*& rhs){
    return  *lhs == *rhs;
}

string Symbol::toString(Symbol& symb){
    stringstream os;
    os  << symb;
    return os.str();

}
string Symbol::toString(Symbol*& symb){
    return toString(*symb);
}

class SymbolTable {  
protected:
    int level = 0;
    int numEnd = 0;
    string command = "", identifier = "", type = "", value = "";

public:
    SymbolTable() {}
    ~SymbolTable() {}

    void split(DLinkedList<Symbol*>* pDLL, string& line, string& command, string& identifier ,string& type, string& value);
    
    int  isValidCommand(string& command);
    bool isValidIdentifier(string& identifier);
    bool isValidType(string& identifier);
    int isValidValue(string& value, string& line); // check sau
    
    bool assignIdenByValue(DLinkedList<Symbol*>* pDLL, string& line);
    bool insert(DLinkedList<Symbol*>* pDLL);
    bool assign(DLinkedList<Symbol*>* pDLL, string& line);
    int lookup(DLinkedList<Symbol*>* pDLL);
    void print(DLinkedList<Symbol*>* pDLL);
    void rprint(DLinkedList<Symbol*>* pDLL);
    void run(string filename);
};
#endif