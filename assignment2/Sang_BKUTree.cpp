template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    public:
        K key;
        V value;
        Entry(K key, V value) : key(key), value(value) {}
    };

public:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5) { this->maxNumOfKeys = maxNumOfKeys; avl = new AVLTree; splay = new SplayTree; }
    ~BKUTree() { this->clear(); delete avl; delete splay; }
    void add(K key, V value)
    {
        Entry* en = new Entry(key, value);
        if (!avl->search(avl->root, key))
        {
            avl->add(en);
            splay->add(en);
            avl->search(avl->root, key)->corr = splay->search(splay->root, key);
            splay->search(splay->root, key)->corr = avl->search(avl->root, key);
            int a = keys.size(); if (a < maxNumOfKeys) { keys.push(key); }
            else { keys.pop(); keys.push(key); }
        }
        else
        {
            delete en;
            throw "Duplicate key";
        }
    };//ch?a làm
    void remove(K key)
    {
        if (!avl->search(avl->root, key))
        {
            throw "Not found";

        }
        else
        {
            Entry* en = new Entry(avl->search(avl->root, key)->entry->key, avl->search(avl->root, key)->entry->value);
            splay->search(splay->root, key)->entry = en;
            avl->remove(key);
            splay->remove(key);
            // xử lý queue keys
            vector <int>v; bool having_key_in_keys = false;
            while (!keys.empty()) { v.push_back(keys.front()); keys.pop(); }
            vector<int>::iterator it = v.begin();
            while (it != v.end()) {
                if (*it == key) {
                    vector<int>::iterator itt = it;
                    if (it == v.begin()) {
                        v.erase(itt);
                        it = v.begin();
                    }
                    else {
                        it--; v.erase(itt);
                    }
                    having_key_in_keys = true;
                }
                else it++;
            }
            v.push_back(splay->root->entry->key);
            if (v.size() > maxNumOfKeys) { v.erase(v.begin()); }
            while (!v.empty()) { keys.push(*v.begin()); v.erase(v.begin()); }
        }
    };//ch?a làm
    V search(K key, vector<K>& traversedList)
    {   //1
        if (splay->root->entry->key == key) { return splay->root->entry->value; }
        vector <int>v; bool having_key_in_keys = false;
        while (!keys.empty()) { v.push_back(keys.front()); keys.pop(); }
        vector<int>::iterator it = v.begin();
        while (it != v.end()) { if (*it == key) { having_key_in_keys = true; } it++; }
        while (!v.empty()) { keys.push(*v.begin()); v.erase(v.begin()); }
        //2
        if (having_key_in_keys)
        {
            splay->search(splay->root, key, traversedList);
            splay->splay_once(key);
            int a = keys.size(); if (a < maxNumOfKeys) { keys.push(key); }
            else { keys.pop(); keys.push(key); }return splay->search(splay->root, key)->entry->value;
        }
        else
        {
            BKUTree<int, int>::AVLTree::Node* _r = splay->root->corr;
            avl->search(_r, key, traversedList, nullptr);
            BKUTree<int, int>::AVLTree::Node* temp = avl->search(_r, key);
            _r = splay->root->corr;
            if (temp)
            {
                int a = keys.size(); if (a < maxNumOfKeys) { keys.push(key); }
                else { keys.pop(); keys.push(key); }
                splay->splay_once(key); return temp->entry->value;
            }
            else
            {
                avl->search(avl->root, key, traversedList, _r);
                BKUTree<int, int>::AVLTree::Node* temp2 = avl->search(avl->root, key);
                if (temp2) {
                    int a = keys.size(); if (a < maxNumOfKeys) { keys.push(key); }
                    else { keys.pop(); keys.push(key); }
                    splay->splay_once(key); return temp2->entry->value;
                }
                else {
                    throw "Not found";
                }
            }
        }

    };//ch?a làm

    void traverseNLROnAVL(void (*func)(K key, V value)) { avl->traverseNLR(func); };//ch?a làm
    void traverseNLROnSplay(void (*func)(K key, V value)) { splay->traverseNLR(func); };//ch?a làm

    void clear() {
        if (!splay->root) { return; }
        splay->copy_key(splay->root);
        this->avl->clear();
        this->splay->clear();
        avl->root = nullptr;
        splay->root = nullptr;
        while (!keys.empty()) { keys.pop(); }
    };//ch?a làm

    class SplayTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            typename AVLTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
            }
            void clear()
            {
                Entry* temp = this->entry; this->entry = nullptr; if (temp) { delete temp; }
                delete this;
            }
        };
        Node* root;

        SplayTree() : root(NULL) {};
        ~SplayTree() { this->clear(); };

        void add(K key, V value)
        {
            Entry* en = new Entry(key, value);
            this->add(en);
        };//ch?a làm
        void add(Entry* entry)
        {
            this->insert(root, entry);
            this->splay(entry->key);
        };//ch?a làm
        void remove(K key)
        {
            Node* temp = search(root, key);
            if (temp)
            {
                splay(temp->entry->key);
                if (temp->entry->key == key)
                {
                    Node* t = root;
                    if (!root->left)
                    {
                        root = root->right;
                    }
                    else
                    {
                        if (!root->left->right)
                        {
                            root = root->left;
                            root->right = t->right;
                        }
                        else
                        {
                            Node* temp = Largest_key(root->left);
                            splay(temp->entry->key);
                            temp->right = t->right;
                            root = temp;
                        }
                    }
                    t->clear();
                }
                else { throw "Not found"; return; }
                //doth
            }
            else { throw "Not found"; return; }
        };//ch?a làm
        V search(K key)
        {
            Node* temp = search(root, key);
            if (temp)
            {
                splay(temp->entry->key);
                if (temp->entry->key == key)
                    return temp->entry->value;
                else
                {
                    throw "Not found";
                }
            }
            else { throw "Not found"; }
        };//ch?a làm

        void traverseNLR(void (*func)(K key, V value))
        {
            traverseNLR(func, root);
        };//ch?a làm
        void traverseNLR(void (*func)(K key, V value), Node* t)
        {
            if (t) {
                func(t->entry->key, t->entry->value);
                if (t->left) { traverseNLR(func, t->left); }
                if (t->right) { traverseNLR(func, t->right); }
            }
        };
        void clear(Node* r)
        {
            if (r)
            {
                if (r->left)clear(r->left);
                if (r->right)clear(r->right);
                if (r)r->clear();
            }
        }
        void clear() {
            if (!root) { return; } clear(root); root = nullptr;
        };//ch?a làm
        //Addition for Splay Tree
        void insert(Node* t, Entry* entry)
        {
            K key = entry->key;
            if (!root) { Node* node = new Node(entry); root = node; }
            else
            {
                if (!t->left && key < t->entry->key) { Node* node = new Node(entry); t->left = node; }
                else if (!t->right && key > t->entry->key)
                {
                    Node* node = new Node(entry); t->right = node;
                }
                else if (t->left && key < t->entry->key)
                {
                    insert(t->left, entry);
                }
                else if (t->right && key > t->entry->key)
                {
                    insert(t->right, entry);
                }
                else if (key == t->entry->key)
                {
                    delete entry;
                    throw "Duplicate key";
                }
            }
        }
        Node* turnRight(Node* a) {
            Node* b = a->left; Node* d = b->right; a->left = d; b->right = a; return b;
        }
        Node* turnLeft(Node* a) {
            Node* b = a->right; Node* c = b->left; a->right = c; b->left = a; return b;
        }
        void splay(K key)
        {
            if (Search_for_splay(root, key))
            {
                while (this->root->entry->key != key)
                {
                    splay(root, key);
                }
            }
            else
            {
                throw "Not found";
            }
        }
        void splay_once(K key)
        {
            splay(root, key);
        }
        void splay(Node*& root, K key)
        {
            if (root == NULL || root->entry->key == key) return;
            //Zag case
            if (key > root->entry->key) //key in the right of root
            {
                if (root->right)
                {
                    if (root->right->entry->key == key)//zag
                        root = turnLeft(root);
                    else if (key > root->right->entry->key)//zag zag
                    {
                        if (root->right->right)
                        {
                            if (root->right->right->entry->key == key)
                            {
                                root = turnLeft(root);
                                root = turnLeft(root);
                            }
                            else
                            {
                                splay(root->right, key);
                            }
                        }
                    }
                    else//zag zig
                    {
                        if (root->right->left)
                        {
                            if (root->right->left->entry->key == key)
                            {
                                root->right = turnRight(root->right);
                                root = turnLeft(root);
                            }
                            else
                            {
                                splay(root->right, key);
                            }
                        }
                    }
                }
                else
                    return; // no key in the tree
            }
            else if (key < root->entry->key)//key in the left of root
            {
                if (root->left)
                {
                    if (root->left->entry->key == key)//zig
                        root = turnRight(root);
                    else if (key < root->left->entry->key)
                    {
                        if (root->left->left)//zig zig
                        {
                            if (root->left->left->entry->key == key)
                            {
                                root = turnRight(root);
                                root = turnRight(root);
                            }
                            else
                            {
                                splay(root->left, key);
                            }
                        }
                    }
                    else//zig zag
                    {
                        if (root->left->right)
                        {
                            if (root->left->right->entry->key == key)
                            {
                                root->left = turnLeft(root->left);
                                root = turnRight(root);
                            }
                            else
                            {
                                splay(root->left, key);
                            }
                        }
                    }
                }
                else
                    return;
            }
        }
        bool Search_for_splay(Node* t, K key)
        {
            if (!t)return false;
            if (t->entry->key == key)return true;
            else if (key > t->entry->key)return Search_for_splay(t->right, key);
            else return Search_for_splay(t->left, key);
        }
        //search
        Node* search(Node* t, K key)
        {
            if (!t)return nullptr;
            if (key > t->entry->key) { return search(t->right, key); }
            else if (key < t->entry->key) { return search(t->left, key); }
            else { return t; }
        }
        Node* search_splay(Node* t, K key)
        {
            if (!t)return nullptr;
            if (key > t->entry->key && t->right) { return search_splay(t->right, key); }
            else if (key > t->entry->key && !t->right) { return t; }
            else if (key < t->entry->key && t->left) { return search_splay(t->left, key); }
            else if (key < t->entry->key && !t->left) { return t; }
            else { return t; }
        }
        void search(Node* t, K key, vector<K>& v)
        {
            if (!t)return;
            if (key > t->entry->key) { v.push_back(t->entry->key);  search(t->right, key, v); }
            else if (key < t->entry->key) { v.push_back(t->entry->key);  search(t->left, key, v); }
            else { return; }
        }
        Node*& Largest_key(Node*& t)
        {
            if (!t->right) { return t; }
            else return Largest_key(t->right);
        }
        void copy_key(Node* t)
        {
            if (t)
            {
                t->entry = new Entry(t->entry->key, t->entry->value);
                copy_key(t->left); copy_key(t->right);
            }
        }
        //Addition for Splay Tree
    };
    class AVLTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            int height;
            typename SplayTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
                this->height = 1;
            }
            void clear()
            {
                Entry* temp = this->entry; this->entry = nullptr; if (temp) { delete temp; }
                delete this;
            }
        };

        Node* root;

        AVLTree() : root(NULL) {};
        ~AVLTree() { this->clear(); };
        void add(K key, V value)
        {
            Entry* en = new Entry(key, value);
            this->add(en);
        };//ch?a làm
        void add(Entry* entry)
        {
            this->insert(root, entry);
            update_height(root);
            update(root);
            update_height(root);
        };//ch?a làm
        void remove(K key)
        {
            del(root, key);
            update_height(root);
            if (root)update(root);
            update_height(root);
        };//ch?a làm
        V search(K key)
        {
            Node* temp = search(root, key);
            if (temp)return temp->entry->value;
            else { throw "Not found"; }
        };//ch?a làm

        void traverseNLR(void (*func)(K key, V value))
        {
            traverseNLR(func, root);
        }
        void traverseNLR(void (*func)(K key, V value), Node* t)
        {
            if (t) {
                func(t->entry->key, t->entry->value);
                if (t->left) { traverseNLR(func, t->left); }
                if (t->right) { traverseNLR(func, t->right); }
            }
        };//pre order chưa làm nè

        void clear(Node* r)
        {
            if (r)
            {
                if (r->left)clear(r->left);
                if (r->right)clear(r->right);
                if (r)r->clear();
            }
        }
        void clear()
        {
            if (!root) { return; }
            clear(root);
            root = nullptr;
        };
        //Addition for AVL tree
        void insert(Node* t, Entry* entry) {
            K key = entry->key;
            if (!root) { Node* node = new Node(entry); root = node; }
            else
            {
                if (!t->left && key < t->entry->key) { Node* node = new Node(entry); t->left = node; }
                else if (!t->right && key > t->entry->key)
                {
                    Node* node = new Node(entry); t->right = node;
                }
                else if (t->left && key < t->entry->key)
                {
                    insert(t->left, entry);
                }
                else if (t->right && key > t->entry->key)
                {
                    insert(t->right, entry);
                }
                else if (key == t->entry->key)
                {
                    delete entry;
                    throw "Duplicate key";
                }
            }
        }
        int height(Node* t)
        {
            if (!t)return 0; else return t->height;
        }
        void update_height(Node* t)
        {
            if (t)
            {
                if (!t->left && !t->right) { t->height = 1; }
                else
                {
                    update_height(t->left); update_height(t->right);
                    t->height = 1 + max(height(t->left), height(t->right));
                }
            }
        }
        Node* turnRight(Node* a) {
            Node* b = a->left; Node* d = b->right; a->left = d; b->right = a; return b;
        }
        Node* turnLeft(Node* a) {
            Node* b = a->right; Node* c = b->left; a->right = c; b->left = a; return b;
        }
        void update(Node*& t)
        {
            if (t->left)update(t->left);
            if (t->right)update(t->right);
            if (t)
            {
                if (height(t->left) - height(t->right) > 1)
                {
                    Node* p = t->left;
                    if (p) {
                        if (height(p->left) >= height(p->right)) { t = turnRight(t); update_height(root); }
                        else { t->left = turnLeft(t->left); t = turnRight(t); update_height(root); }
                    }
                    else { t->left = turnLeft(t->left); t = turnRight(t); update_height(root); }
                }
                if (height(t->left) - height(t->right) < -1)
                {
                    Node* p = t->right;
                    if (p) {
                        if (height(p->right) >= height(p->left))
                        {
                            t = turnLeft(t); update_height(root);
                        }
                        else { t->right = turnRight(t->right); t = turnLeft(t); update_height(root); }
                    }
                    else { t->right = turnRight(t->right); t = turnLeft(t); update_height(root); }
                }
            }
        }
        //Deletetion
        Node*& Largest_key(Node*& t)
        {
            if (!t->right) { return t; }
            else return Largest_key(t->right);
        }
        void del(Node*& t)
        {
            if (!t->left && !t->right) { Node* temp = t; t = nullptr; temp->clear(); }
            else if (!t->left) { Node* temp = t; t = t->right; temp->clear(); }
            else
            {
                if (!t->left->right)
                {
                    Node* temp = t;
                    t = Largest_key(t->left);
                    t->right = temp->right;
                    temp->clear();
                }
                else
                {
                    Node* temp = t; Node* temp_2 = Largest_key(t->left);
                    Largest_key(t->left) = temp_2->left; t = temp_2;
                    t->left = temp->left; t->right = temp->right;
                    temp->clear();
                }
            }
        }
        void del(Node*& t, K key)
        {
            if (key > t->entry->key) { if (!t->right) { throw "Not found"; return; } del(t->right, key); }
            else if (key < t->entry->key) { if (!t->left) { throw "Not found"; return; } del(t->left, key); }
            else { del(t); }
        }
        //Deletetion
        //Search
        Node* search(Node* t, K key)
        {
            if (!t)return nullptr;
            if (key > t->entry->key) { return search(t->right, key); }
            else if (key < t->entry->key) { return search(t->left, key); }
            else { return t; }
        }
        void search(Node* t, K key, vector<K>& v, Node* stop)
        {
            if (t == stop || t == nullptr)return;
            if (key > t->entry->key) { v.push_back(t->entry->key); search(t->right, key, v, stop); }
            else if (key < t->entry->key) { v.push_back(t->entry->key); search(t->left, key, v, stop); }
            else { return; }
        }
        //Search
        //Addition for AVL tree
    };
};