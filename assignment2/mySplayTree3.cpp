#include <bits/stdc++.h>
using namespace std;

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
        T data;
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
    
    Symbol a("","a", "string", "abc", false, 45);
    Symbol b("", "b", "string", "abc", false, 60);
    Symbol c("", "c", "string", "abc", false, 50);
    Symbol d("", "d", "string", "abc", false, 70);
    Symbol e("", "e", "string", "abc", false, 80);
    Symbol f("", "f", "string", "abc", false, 10);
    Symbol g("", "g", "string", "abc", false, 25);
    Symbol h("", "h", "string", "abc", false, 35);
    SplayTree<Symbol> tree_symb;
    tree_symb.add(a);
    tree_symb.add(b);
    tree_symb.add(c);
    tree_symb.add(d);
	tree_symb.add(e);
    tree_symb.add(f);
    tree_symb.add(g);
    tree_symb.add(h);
    
    //cout << "\n" << tree_symb.getNumCompare() << endl;
    //cout << tree_symb.getNumSplay() << endl;
    //cout << "\n" << tree_symb.preOrder() << endl;
    
    //cout << a.getCommand() << " " << a.getIdentifier() << " " << a.getLevel() << endl;
    //cout << b.getCommand() << " " << b.getIdentifier() << " " << b.getLevel() << endl;
    //a = b;
    //cout << a.getCommand() << " " << a.getIdentifier() << " " << a.getLevel() << endl;

    typename SplayTree<Symbol>::Node* temp = new SplayTree<Symbol>::Node(a);
    cout << temp->getData().getLevel() << endl;

    typename SplayTree<Symbol>::Node* myRoot = tree_symb.getRoot();
    cout << myRoot->getData();
    

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