#include <bits/stdc++.h>
using namespace std;

template<class T>
class IList {
public:
    virtual void    add(T e)=0;
    virtual bool    add(int index, T e)=0;
    virtual T    removeAt(int index)=0;
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
    T    removeAt(int index);
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
T DLinkedList<T>::removeAt(int index){
    //if ((index < 0) || (index > this->count - 1)) return NULL;
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
    return data;
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

////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////// STAC_K ///////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
template<class T>
class Stack {
public:
    class Iterator; //forward declaration
    
protected:
    DLinkedList<T> DLL; //internal DLL
    void (*deleteUserData)(DLinkedList<T>*); //function pointer: be called to remove items (if they are pointer type)
    bool (*itemEqual)(T& lhs, T& rhs); //function pointer: test if two items (type: T&) are equal or not
    
public:
    Stack(  void (*deleteUserData)(DLinkedList<T>*)=0, 
            bool (*itemEqual)(T&, T&)=0){
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
    }
    void push(T item){
        DLL.add(0,item);
    }
    T pop(){
        return DLL.removeAt(0);
    }
    T& peek(){
        return DLL.get(0);
    }
    bool empty(){
        return DLL.empty();
    }
    int size(){
        return DLL.size();
    }
    void clear(){
        return DLL.clear();
    }
    bool remove(T item){
        return DLL.removeItem(item);
    }
    bool contains(T item){
        return DLL.contains(item);
    }
    string  toString(string (*item2str)(T&)=0 ){
        stringstream os;
        os << "FROM TOP: " << DLL.toString(item2str);
        return os.str();
    }
    void println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }

    Iterator top(){
        return Iterator(this, true);
    }
    Iterator bottom(){
        return Iterator(this, false);
    }
     
private:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T& )){
        if(itemEqual == 0) return lhs == rhs;
        else return itemEqual(lhs, rhs);
    }
    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
    
public:
    
    //Iterator: BEGIN
    class Iterator{
    private:
        Stack<T>* stach;
        typename DLinkedList<T>::Iterator DLLIt;
    public:
        Iterator(Stack<T>* stach=0, bool begin=true){
            this->stach = stach;
            if(begin){
                if(stach != 0) this->DLLIt = stach->DLL.begin();
                else this->DLLIt = 0;
            }
            else{
                if(stach != 0) this->DLLIt = stach->DLL.end();
                else this->DLLIt = 0;
            }
        }
        Iterator& operator=(const Iterator& iterator ){
            this->stach = iterator.stach;
            this->DLLIt = iterator.DLLIt;
            return *this;
        }
        
        T& operator*(){
            return *(this->DLLIt);
        }
        bool operator!=(const Iterator& iterator){
            return this->DLLIt != iterator.DLLIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            DLLIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(void (*removeItem)(T)=0){
            DLLIt.remove(removeItem);
        }
    };
    //Iterator: END
};

////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////// QUEUUE /////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
template<class T>
class Queue {
public:
    class Iterator; //forward declaration
    
protected:
    DLinkedList<T> DLL;
    void (*deleteUserData)(DLinkedList<T>*);
    bool (*itemEqual)(T& lhs, T& rhs);
    
public:
    Queue(
            void (*deleteUserData)(DLinkedList<T>*)=0, 
            bool (*itemEqual)(T&, T&)=0){
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
    }
    void push(T item){
        DLL.add(item);
    }
    T pop(){
        return DLL.removeAt(0);
    }
    T& peek(){
        return DLL.get(0);
    }
    bool empty(){
        return DLL.empty();
    }
    int size(){
        return DLL.size();
    }
    void clear(){
        DLL.clear();
    }
    bool remove(T item){
        return DLL.removeItem(item);
    }
    bool contains(T item){
        return DLL.contains(item);
    }
    string  toString(string (*item2str)(T&)=0 ){
        stringstream os;
        os << "FRONT-TO-REAR: " << DLL.toString(item2str);
        return os.str();
    }
    void println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }
    
    Iterator front(){
        return Iterator(this, true);
    }
    Iterator rear(){
        return Iterator(this, false);
    }
    
private:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T& )){
        if(itemEqual == 0) return lhs == rhs;
        else return itemEqual(lhs, rhs);
    }
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////

public:
    //Iterator: BEGIN
    class Iterator{
    private:
        Queue<T>* quEue;
        typename DLinkedList<T>::Iterator listIt;
    public:
        Iterator(Queue<T>* quEue=0, bool begin=true){
            this->quEue = quEue;
            if(begin){
                if(quEue != 0) this->listIt = quEue->DLL.begin();
                else this->listIt = 0;
            }
            else{
                if(quEue != 0) this->listIt = quEue->DLL.end();
                else this->listIt = 0;
            }
        }
        Iterator& operator=(const Iterator& iterator ){
            this->quEue = iterator.quEue;
            this->listIt = iterator.listIt;
            return *this;
        }
        
        T& operator*(){
            return *(this->listIt);
        }
        bool operator!=(const Iterator& iterator){
            return this->listIt != iterator.listIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            listIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(void (*removeItem)(T)=0){
            listIt.remove(removeItem);
        }
    };
    //Iterator: END
};
////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////// TREE /////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
class Symbol{
private:
    string command = "";
    string identifier = "";
    string type = "";
    int level = 0;
    string value = "";
    bool isFunction = false;
    
public:
    Symbol(string command = "", string identifier = "", string type = "", string value = "", bool isFucntion = false , int level = 0) {
        this->command = command;
        this->identifier = identifier; 
        this->type = type;
        this->value = value;
        this->level = level;
        this->isFunction = isFucntion;
    }
    Symbol(string& command) {
        this->command = command;
    }
    ~Symbol() {}

    friend ostream &operator<<( ostream &os, const Symbol& symb);
    //static section  
    static bool equals(Symbol& lhs, Symbol& rhs); //with pointer to symbol
    static bool equals(Symbol*& lhs, Symbol*& rhs); //with pointer to symbol
    static int compare(Symbol& lhs, Symbol& rhs);
    static int compare(Symbol*& lhs, Symbol*& rhs);
    static string toString(Symbol& symb);
    static string toString(Symbol*& symb);

    Symbol& operator=(const Symbol& symb) {
        this->command = symb.command;
        this->identifier = symb.identifier;
        this->level = symb.level;
        this->value = symb.value;
        this->type = symb.type;
        this->isFunction = symb.isFunction;
        return *this;
    }

    bool operator==(Symbol rhs) { // check identifier & level 
        return  (this->level == rhs.level) &&
                (this->identifier.compare(rhs.identifier) == 0);
    }
    bool operator>(Symbol rhs) {
        if (this->level > rhs.level) return true;
        if (this->level < rhs.level) return false;
        if (this->level == rhs.level && this->identifier.compare(rhs.identifier) > 0) 
            return true;
        return false;
    }
    bool operator<(Symbol rhs) {
        if (this->level < rhs.level) return true;
        if (this->level > rhs.level) return false;
        if (this->level == rhs.level && this->identifier.compare(rhs.identifier) < 0) 
            return true;
        return false;
    }
    bool operator>=(Symbol rhs) {
        if (this->level > rhs.level) return true;
        if (this->level == rhs.level && this->identifier.compare(rhs.identifier) >= 0)
            return true;
        return false;
    }
    bool operator<=(Symbol rhs) {
        if (this->level < rhs.level) return true;
        if (this->level == rhs.level && this->identifier.compare(rhs.identifier) <= 0)
            return true;
        return false;
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
    bool getIsfunction() {
        return this->isFunction;
    }
    void sEtIsfunction(bool isFunction) {
        this->isFunction = isFunction;
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
int Symbol::compare(Symbol& lhs, Symbol& rhs){
    if (lhs == rhs) return 0;
    if (lhs > rhs) return 1;
    return -1; 
}
int Symbol::compare(Symbol*& lhs, Symbol*& rhs){
    if (*lhs == *rhs) return 0;
    if (*lhs > *rhs) return 1;
    return -1;
}
string Symbol::toString(Symbol& symb){
    stringstream os;
    os << symb;
    return os.str();
}
string Symbol::toString(Symbol*& symb){
    return toString(*symb);
}

template <class T>
class SplayTree {
public:
    class Node;
    
protected:
    Node* pRoot;
    int num_splay = 0;
    int num_compare = 0;

protected:
    Node* search(Node* node, T& key) {
        if (node == nullptr) return nullptr;
        else {
            num_compare++;
            if (key == node->data) {
                Splay(this->pRoot, node);
                return node;
            }
            else if (key > node->data)
                return search(node->pRight, key);
            else {
                return search(node->pLeft, key);
            }
        }
    }
    Node* searchIden(Node* node, string& iden, int& level) {
        if (node == nullptr) return nullptr;
        else {
            num_compare++;
            if (node->data.getLevel() == level && iden == node->data.getIdentifier()) {        // found
                Splay(this->pRoot, node);
                return node;
            }
            else if ( level > node->data.getLevel() || (level == node->data.getLevel() && iden > node->data.getIdentifier()) )
                return searchIden(node->pRight, iden, level);
            else {
                return searchIden(node->pLeft, iden, level);
            }
        }
    }
    void remove(Node* _pRoot, T& key) {
        if (_pRoot == nullptr) return; // rong

        if (key > _pRoot->data) {
            remove(_pRoot->pRight, key);
        }
        else if (key < _pRoot->data) {
            remove(_pRoot->pLeft, key);
        }
        else {
            Node* temp1 = _pRoot;
            Splay(this->pRoot, temp1); 
            if (_pRoot->pLeft && _pRoot->pRight) {
                Node *temp = this->pRoot;
                Node *temp_left = temp->pLeft;
                temp_left->pParent = nullptr;
                Node *temp_right = temp->pRight;
                temp_right->pParent = nullptr;
                delete temp;
                Node* biggestNode = BiggestLeaf(temp_left);
                Splay(temp_left, biggestNode);
                temp_left->pRight = temp_right;
                temp_right->pParent = temp_left;
                this->pRoot = temp_left;
            }
            else if (_pRoot->pLeft) {
                Node *temp = this->pRoot;
                Node *temp_left = temp->pLeft;
                temp_left->pParent = nullptr;
                delete temp;
                Node* biggestNode = BiggestLeaf(temp_left);
                Splay(temp_left, biggestNode);
                this->pRoot = temp_left;
            }
            else if (_pRoot->pRight) {
                Node* temp = this->pRoot;
                Node* temp_right = temp->pRight;
                temp_right->pParent = nullptr;
                this->pRoot = temp_right;
                delete temp;
            }
            else {
                delete _pRoot;
                this->pRoot = nullptr;
            }
        }
    }

    Node* BiggestLeaf (Node* _pRoot) {
        if (_pRoot->pRight) {
            return BiggestLeaf(_pRoot->pRight);
        }
        else return _pRoot;
    }
    Node* rotateLeft(Node* _pRoot) {
        Node* p = _pRoot->pParent;
        Node* temp = _pRoot->pRight;
        _pRoot->pRight = temp->pLeft;
        if (temp->pLeft != nullptr) {
            temp->pLeft->pParent = _pRoot;
        }

        temp->pLeft = _pRoot;
        _pRoot->pParent = temp;
        temp->pParent = p;

        if (p != nullptr)
        {
            if (p->pLeft == _pRoot)
                p->pLeft = temp;
            else
            {
                p->pRight = temp;
            }
        }
        return temp;
    }
    Node* rotateRight(Node* _pRoot) {
        Node *p = _pRoot->pParent;
        Node *temp = _pRoot->pLeft;
        _pRoot->pLeft = temp->pRight;
        if (temp->pRight != nullptr) {
            temp->pRight->pParent = _pRoot;
        }

        temp->pRight = _pRoot;
        _pRoot->pParent = temp;
        temp->pParent = p;
        
        if (p != nullptr)
        {
            if (p->pLeft == _pRoot)
                p->pLeft = temp;
            else
            {
                p->pRight = temp;
            }
        }
        return temp;
    }

    string inOrderRec(Node* _pRoot) {
        stringstream ss;
        if (_pRoot != nullptr)
        {
            ss << inOrderRec(_pRoot->pLeft);
            ss << _pRoot->data << " ";
            ss << inOrderRec(_pRoot->pRight);
        }
        return ss.str();
    }
    string preOrderRec(Node* _pRoot) {
        stringstream ss;
        if (_pRoot != nullptr)
        {
            ss << _pRoot->data << " ";
            ss << preOrderRec(_pRoot->pLeft);
            ss << preOrderRec(_pRoot->pRight);
        }
        return ss.str();
    }
    string postOrderRec(Node* _pRoot) {
        stringstream ss;
        if (_pRoot != nullptr)
        {
            ss << postOrderRec(_pRoot->pLeft);
            ss << postOrderRec(_pRoot->pRight);
            ss << _pRoot->data << " ";
        }
        return ss.str();
    }
    int height(Node* _pRoot) {
        if (_pRoot == nullptr) {
            return 0;
        }
        else {
            int numLeft = 0;
            int numRight = 0;
            numLeft = 1 + height(_pRoot->pLeft);
            numRight = 1 + height(_pRoot->pRight);
            return numLeft > numRight ? numLeft : numRight; 
        }
    }
    void clear(Node*& _pRoot) {
        if (_pRoot)
        {
            clear(_pRoot->pLeft);
            clear(_pRoot->pRight);
            delete _pRoot;
        }
    }
    void Splay(Node*& _pRoot, Node*& new_node) {
        while (new_node->pParent != nullptr)
        {
            if (new_node->pParent == _pRoot) {
                if (new_node == new_node->pParent->pLeft) {
                    _pRoot = rotateRight(new_node->pParent);  // zig
                }
                else {
                    _pRoot = rotateLeft(new_node->pParent);  // zag
                }
            }
            else {
                Node *p = new_node->pParent;
                Node *g = p->pParent;
                if (new_node == new_node->pParent->pLeft && p == p->pParent->pLeft) {  // zig - zig
                    g = rotateRight(g);
                    p = rotateRight(p);
                    if (new_node->pParent == nullptr)
                        _pRoot = p;
                }
                else if (new_node == new_node->pParent->pRight && p == p->pParent->pRight) { // zag - zag
                    g = rotateLeft(g);
                    p = rotateLeft(p);
                    if (new_node->pParent == nullptr)
                        _pRoot = p;
                }
                else if (new_node == new_node->pParent->pLeft && p == p->pParent->pRight) {  // zig - zag
                    p = rotateRight(p);
                    g = rotateLeft(g);
                    if (new_node->pParent == nullptr)
                        _pRoot = g;
                }
                else {                                                                 // zag - zig
                    p = rotateLeft(p);
                    g = rotateRight(g);
                    if (new_node->pParent == nullptr)
                        _pRoot = g;
                }
            }
            num_splay++;
        }
    }
    void prettyPrintTree(Node* node, string prefix = "", bool isLeft = true)
    {
        if (node == nullptr)
        {
            cout << "Empty tree";
            return;
        }

        if (node->pRight)
        {
            prettyPrintTree(node->pRight, prefix + (isLeft ? "│   " : "    "), false);
        }

        cout << prefix + (isLeft ? "└── " : "┌── ") << node->data << endl;

        if (node->pLeft)
        {
            prettyPrintTree(node->pLeft, prefix + (isLeft ? "    " : "│   "), true);
        }
    }

public:
    SplayTree() : pRoot(nullptr) {}
    ~SplayTree() {
        clear();
    }

    class Node {
    private:
        Symbol data;
        Node* pLeft;
        Node* pRight;
        Node* pParent;
        
    public:
        Node(T value) : data(value), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
        ~Node() {}
        friend class SplayTree<T>;
        T getData() {
            return this->data;
        }
        void sEtData(T data) {
            this->data = data;
        }
        Node* getLeft() {
            return this->pLeft;
        }
        void sEtLeft(Node* pLeft) {
            this->pLeft = pLeft;
        }
        Node* getRight() {
            return this->pRight;
        }
        void sEtRight(Node* pRight) {
            this->pRight = pRight;
        }
        Node* getParent() {
            return this->pParent;
        }
        void sEtParent(Node* pParent) {
            this->pParent = pParent;
        }
        string getCommand() {
            return this->data.getCommand();
        }
        void sEtCommand(string& command) {
            this->data.sEtCommand(command); 
        }
        string getIdentifier() {
            return this->data.getIdentifier();
        }
        void sEtIdentifier(string& identifier) {
            this->data.sEtIdentifier(identifier); 
        }
        string getType() {
            return this->data.getIdentifier();
        }
        void sEtType(string& type) {
            this->data.sEtType(type); 
        }
        string getValue() {
            return this->data.getValue();
        }
        void sEtValue(string& value) {
            this->data.sEtValue(value); 
        }
        int getLevel() {
            return this->data.getLevel();
        }
        void sEtLevel(int& level) {
            this->data.sEtLevel(level); 
        }
        bool getIsfunction() {
            return this->data.getIsfunction();
        }
        void sEtIsfunction(bool isFunction) {
            this->data.sEtIsfunction(isFunction);
        }
    };

    void printTree()
    {
        prettyPrintTree(pRoot);
    }

    bool add(Node*& _pRoot, Node* new_node) {      
        Node* temp = _pRoot;
        Node* prev = nullptr;
        while (temp != nullptr) {
            prev = temp;
            num_compare++;
            if (new_node->data == temp->data) {  // sure không có bị trùng
                return false;
            }
            else if (new_node->data < temp->data) {
                temp = temp->pLeft;
            }
            else                        // check lại 2 symbol bằng nhau thì xóa node r throw
                temp = temp->pRight;
        }
        if (prev == nullptr) {          // root
            _pRoot = new_node;
            _pRoot->pParent = nullptr;
        }
        else {
            if (new_node->data < prev->data) {
                prev->pLeft = new_node;
                new_node->pParent = prev;
            }
            else {
                prev->pRight = new_node;
                new_node->pParent = prev;
            }
        }
        Splay(_pRoot, new_node);
        return true;
    }
    bool add(const T& new_value) {
        this->num_compare = 0; this->num_splay = 0;
        Node *new_node = new Node(new_value);
        return add(pRoot, new_node);
    }
    void remove(T& key) {
        remove(this->pRoot, key);
    }
    string inOrder() {
        return inOrderRec(this->pRoot);
    }
    string preOrder() {
        return preOrderRec(this->pRoot);
    }
    string postOrder() {
        return postOrderRec(this->pRoot);
    }
    Node* search(T& key) { //return NULL if not found
        this->num_compare = 0;
        this->num_splay = 0;
        return search(this->pRoot, key);
    }  
    Node* searchIden(string& iden, int& level) {
        this->num_compare = 0;
        this->num_splay = 0;
        return searchIden(this->pRoot, iden, level);
    }
    void clear() {
        clear(this->pRoot);
    }   
    Node* getRoot() {
        return this->pRoot;
    }
    void sEtRoot(Node* pRoot) {
        this->pRoot = pRoot;
    }
    int getHeight() {
        return this->height(this->pRoot);
    }  
    int getNumSplay() {
        return this->num_splay;
    }
    int getNumCompare() {
        return this->num_compare;
    }
    void sEtNumSplay(int nSplay) {
        this->num_splay = nSplay;
    }
    void sEtNumComare(int nComp) {
        this->num_compare = nComp;
    }
};

int main() {
    Queue<string> myQueue;
    myQueue.push("a");
    myQueue.push("b");
    myQueue.push("d");
    myQueue.push("c");
    cout << myQueue.pop() << endl;
    myQueue.println();
    cout << myQueue.size() << endl;
    
    /* 
    Symbol a("","a", "string", "abc", false, 0);
    Symbol b("", "b", "string", "abc", false, 0);
    Symbol c("", "c", "string", "abc", false, 0);
    Symbol d("", "d", "string", "abc", false, 0);
    Symbol e("", "a", "string", "abc", false, 1);
    Symbol f("", "b", "string", "abc", false, 1);
    Symbol g("", "a", "string", "abc", false, 2);
    Symbol h("", "b", "string", "abc", false, 2);
    SplayTree<Symbol> tree_symb;
    tree_symb.add(a);
    tree_symb.add(b);
    tree_symb.add(h);
    tree_symb.add(e);
	tree_symb.add(d);
    tree_symb.add(c);
    tree_symb.add(g);
    tree_symb.add(f);
    
    tree_symb.printTree();
    cout << "\nNum_comp: " << tree_symb.getNumCompare() << endl;
    cout << "Num_splay: " << tree_symb.getNumSplay() << endl;

    int level = 0;
    string iden = "a";
    int i = level;
    typename SplayTree<Symbol>::Node* temp = nullptr;
    while (i >= 0) {
        temp = tree_symb.searchIden(iden, i);
        i--;
    }
    if (temp) {
        cout << temp->getData() << endl;
    } else cout << "rong" << endl;

    tree_symb.printTree();
    cout << "\nNum_comp: " << tree_symb.getNumCompare() << endl;
    cout << "Num_splay: " << tree_symb.getNumSplay() << endl; */
    
    //cout << "\n" << tree_symb.preOrder() << endl;
    
    //cout << a.getCommand() << " " << a.getIdentifier() << " " << a.getLevel() << endl;
    //cout << b.getCommand() << " " << b.getIdentifier() << " " << b.getLevel() << endl;
    //a = b;
    //cout << a.getCommand() << " " << a.getIdentifier() << " " << a.getLevel() << endl;

    //typename SplayTree<Symbol>::Node* temp = new SplayTree<Symbol>::Node(a);
    //cout << temp->getData().getLevel() << endl;

    //typename SplayTree<Symbol>::Node* myRoot = tree_symb.getRoot();
    //cout << myRoot->getData();
    

    /*
    SplayTree<int> test_tree2;
    int arr[] = {700, 550, 300, 100, 400, 46, 23, 150, 76, 15 , 90 , 50 , 170, 350};
    for (int i = 0; i 1< sizeof(arr)/sizeof(int); i++)
    {
        test_tree2.add(arr[i]);
    }
    test_tree2.printTree();
    */
    //cout << test_tree2.getNumCompare() << endl;
    //cout << test_tree2.getNumSplay() << endl;
    //string resultPre = test_tree2.preOrder();
    //resultPre.pop_back();
    //cout << "\n" << resultPre << endl;

    return 0;
}