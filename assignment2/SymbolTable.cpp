#include "SymbolTable.h"



void SymbolTable::split() {
    int i = 0;
    // GET THE COMMAND
    string temp = "";
    while (i < (int)line.length()) 
    {
        if (line[i] != ' ') {
            temp = temp + line[i];
        }
        else break;
        i++;
    }

    int choice = isValidCommand(temp); 
    if (choice == -1) throw InvalidInstruction(line); // exception command
    this->command = temp;
    i++; // skip a space
    
    if (choice == 1) { // INSERT
        while (i < (int)line.length())    // identifier
        {
            if (line[i] != ' ') {
                identifier = identifier + line[i];
            }
            else break;
            i++;
        }
        if (!isValidIdentifier(identifier)) throw InvalidInstruction(line);
        i++; // skip a space
        while (i < (int)line.length())  // lấy type
        {
            if (line[i] != ' ') {
                type = type + line[i];
            }
            else break;
            i++;
        }
        if (!isValidType(type) && !isTypeFunction(type) ) { // không phải string
                                                            //  num hay function
            throw InvalidInstruction(line);
        }
        i++;
        while (i < (int)line.length())  // lấy static
        {
            if (line[i] != ' ') {
                statik = statik + line[i];
            }
            else break;
            i++;
        }

        this->isStatik = isStatic(statik); // trước mắt chỉ check valid hay không thôi, 
                                           // vô hàm inssert rồi check level == 0 && static == false

        return;
    }   
    if (choice == 2) { // ASSIGN
        while (i < (int)line.length())    // Iden
        {
            if (line[i] != ' ') {
                identifier = identifier + line[i];
            }
            else break;
            i++;
        }
        i++;
        if (!isValidIdentifier(identifier)) throw InvalidInstruction(line);
        
        while (i < (int)line.length()) 
        {
            value = value + line[i];
            i++;
        } // NOTE: dont check the validation yet
        
        return;
    }
    if (choice == 3) {  // BEGIN
        this->level++;
        if (i != (int)line.length() + 1) throw InvalidInstruction(line);
    }
    if (choice == 4) {  // END
        this->level--; 
        if (i != (int)line.length() + 1) throw InvalidInstruction(line);
        if (this->level < 0) throw UnknownBlock();
    }
    if (choice == 5) {  // LOOKUP
        while (i < (int)line.length()) 
        {
            identifier = identifier + line[i];
            i++;
        }
        if (!isValidIdentifier(identifier)) throw InvalidInstruction(line);
        // NOTE: dont check the identifier added in the tree yet
        return;
    }
    if (choice == 6) {  // PRINT
        if (i != (int)line.length() + 1) throw InvalidInstruction(line);
        return;
    }

}

int SymbolTable::isValidCommand(string& command) {
    if (command == "INSERT") return 1;
    if (command == "ASSIGN") return 2;
    if (command == "BEGIN") return 3;
    if (command == "END") return 4;
    if (command == "LOOKUP") return 5;
    if (command == "PRINT") return 6;
    return -1;
}
int SymbolTable::isValueNumberNString(string& value) {
    int isNum = 1;

    if ((int)value.length() == 1 && ((int)value[0] >= 48 && (int)value[0] <= 57)) return 1;

    if ( ((int)value[0] < 48 || (int)value[0] > 57) ||
         ((int)value[(int)value.length() - 1] < 48 || 
          (int)value[(int)value.length() - 1] > 57) ) isNum = 2;
    

    int i =  1;
    
    while (i < (int)value.length() - 1) {
        if ((int)value[i] < 48 || (int)value[i] > 57) {          // 0 - 9
            if (((int)value[i] < 97 || (int)value[i] > 122) &&   // a - z
                ((int)value[i] < 65 || (int)value[i] > 90) &&    // A - Z
                ((int)value[i] != 32) )
                    return -1;

            isNum = 2;
        }
        i++;
    }
    
    if ( (isNum == 2) && ( ( (int)value[0] != 39 || (int)value[(int)value.length() - 1 ] != 39) || ( (int)value.length() < 2) ) ) return -1;
    // -1 là invalid
    // 1 là number
    // 2 là string
    return isNum;
}
bool SymbolTable::isStatic(string& staticc) {
    if (staticc == "true") return true;
    if (staticc == "false") return false;
    else throw InvalidInstruction(line);
}
bool SymbolTable::isValidIdentifier(string& identifier) {
    if ((int)identifier[0] < 97 || (int)identifier[0] > 122) return false;
    for (int i = 1; i < (int)identifier.length(); i++) {    
        if (((int)identifier[i] < 97 || (int)identifier[i] > 122) &&   // a - z
            ((int)identifier[i] < 65 || (int)identifier[i] > 90) &&    // A - Z
            ((int)identifier[i] < 48 || (int)identifier[i] > 57) &&    // 0 - 9
            ((int)identifier[i] != 95))                                // _
                return false; 
    }
    return true;
}
bool SymbolTable::isTypeFunction(string& str) {
    if (str == "" || str[0] != '(') return false;
    int i = 1;
    int theLength = (int)str.length();
    string typee = "";
    int numPara = 0;

    // parameters
    while (i < theLength) {
        if (str[i] != ',' && str[i] != ')') {
            typee += str[i];
        }
        if (str[i] == ',') {
            if (!isValidType(typee)) return false; // check string hoặc number
            typee = "";
            numPara++;
        }
        if (str[i] == ')') {
            if (numPara == 0 && typee == "") {}  // trường hợp k có tham số: ()->string
            else if (!isValidType(typee)) return false;  // có thể k có tham số ?     
            typee = "";
            i++;
            break;
        }   
        i++;
    }

    // return value
    string tempReturn = "";
    if (i < theLength) {
        if ((theLength - i) != 8)  return false;
        if (str[i] != '-' && str[i+1] != '>') return false;
        i+=2;
        while (i < theLength) {
            tempReturn += str[i];
            i++;
        }
    }

    if (isValidType(tempReturn)) {
        this->isFunction = true;
        return true;
    }
    return false;
}
bool SymbolTable::isValidType(string& type) { // string or number
    if (type == "string" || type == "number") return true;
    return false;
}

void SymbolTable::myfckingtest() {
    string st = "'10a'";
    string line = "line";
    //cout << isValidValue(st, line) << endl; // num

    //Symbol(string command = "", string identifier = "", string type = "", string value = "", int level = 0)
    Symbol* a = new Symbol("","a", "string", "abc", 2);
    Symbol* b = new Symbol("", "b", "string", "abc", 2);
    Symbol* c = new Symbol("", "z", "string", "s", 3);

    string myType = "(string,number,number,string)->number";
    cout << isTypeFunction(myType) << endl;
  
    delete a;
    delete b;
    delete c;
}

void SymbolTable::insert() {
    SplayTree<Symbol>::Node* tempNode = nullptr;
    if (isStatik) {   // always level = 0
        Symbol new_symb(this->command, this->identifier, this->type, this->value, this->isFunction, 0);
        tempNode = tree_symb.search(new_symb);
        if (tempNode == nullptr) tree_symb.add(new_symb);
    }
    else {
        if (this->isFunction && this->level != 0) throw InvalidDeclaration(this->line);
        Symbol new_symb(this->command, this->identifier, this->type, this->value, this->isFunction, this->level);
        tempNode = tree_symb.search(new_symb);
        if (tempNode == nullptr) tree_symb.add(new_symb);
        if (this->level > 0) {
            myStack.push(new_symb); // for end
        }
        
    }
    
    if (tempNode != nullptr) {
        throw Redeclared(this->line);
    }
    else {
        cout << tree_symb.getNumCompare() << " " << tree_symb.getNumSplay() << endl;
    }
    
}
bool SymbolTable::assignNumString(int& total_nComp, int& total_nSplay) {
    /* 
        idea: ASSIGN x 123  |  ASSIGN x 'abc'
        1/ kiểm tra value thuộc 1 trong 2 string hay number: nếu ko thuộc 1 trong return false
        2/ tìm kiếm theo identifier (x)
            a/ tìm không ra -> Undeclared
            b/ tìm ra -> check type thuộc string | number
                + nếu trùng: in nump_comp vs nump_splay -> gán value vào Iden -> return true
                + khác: TypeMismatch

    */
    int checker = isValueNumberNString(this->value);
    if (checker > 0) { // number orr string
        SplayTree<Symbol>::Node* temp = nullptr;
        int i = this->level;
        while (i >= 0) {   // search với level giảm dần => make sure là từ block cao nhất đến thấp nhất
            temp = tree_symb.searchIden(this->identifier, i);
            if (temp) break; // found
            i--;
        }
        if (temp) {
            // identifier trong assign không được là hàm
            if (temp->getIsfunction()) throw TypeMismatch(this->line);

            string theType = temp->getData().getType(); 
            if (theType == "string" && checker == 2) { // match string
                temp->sEtValue(this->value);
                total_nComp += tree_symb.getNumCompare();
                total_nSplay += tree_symb.getNumSplay();
                return true;
            }
            if (theType == "number" && checker == 1) { // match number
                temp->sEtValue(this->value);
                total_nComp += tree_symb.getNumCompare();
                total_nSplay += tree_symb.getNumSplay();
                return true;
            }
            throw TypeMismatch(this->line);
        } else throw Undeclared(this->line);   // not found
    }
    return false;
}
bool SymbolTable::assignIden(int& total_nComp, int& total_nSplay){
    /*
        idea: ASSIGN a b
        1/ tìm b theo identifier trong tree: xác định valid? -> nếu không return false (vì có thể nó là function?)
            - nếu không tìm thấy b -> Undeclared
            - nếu tìm thấy -> lưu value và type của b
        2/ tìm a
            - không tìm thấy a -> Undeclared
            - nếu tìm thấy 
                + so sánh type của 2 thằng: 
                    * nếu trùng thì gán value của b vào a -> return true
                    * ngược lại TypeMismatch          
    */
    bool validIden = isValidIdentifier(this->value);
    if (validIden == false) return false;
    if (validIden) {
        SplayTree<Symbol>::Node* valueNode;
        SplayTree<Symbol>::Node* idenNode;
        
        // tìm iden theo value: tìm b
        int i = this->level;
        while (i >= 0) {   // search với level giảm dần => make sure là từ block cao nhất đến thấp nhất
            valueNode = tree_symb.searchIden(this->value, i);
            if (valueNode) break; // found
            i--;
        }
        if (valueNode == nullptr) throw Undeclared(this->line);
        total_nComp += tree_symb.getNumCompare();
        total_nSplay += tree_symb.getNumSplay();
        //cout << "\ntotal_nComp: " << total_nComp << endl;
        //cout << "total_nSplay: " << total_nSplay << endl;
        // tìm iden theo iden: tìm a
        i = this->level;
        while (i >= 0) {   // search với level giảm dần => make sure là từ block cao nhất đến thấp nhất
            idenNode = tree_symb.searchIden(this->identifier, i);
            if (idenNode) break; // found
            i--;
        }
        if (idenNode == nullptr) throw Undeclared(this->line);
        total_nComp += tree_symb.getNumCompare();
        total_nSplay += tree_symb.getNumSplay();

        
        // trường hợp a hoặc b là hàm -> như v sẽ gọi k đúng kiểu vì đã check isValidIdentifier
        if (idenNode->getIsfunction() || valueNode->getIsfunction()) throw TypeMismatch(this->line); 

        //cout << idenNode->getType() << "   ok?  " << valueNode->getType() << endl;
        if (idenNode->getType() == valueNode->getType()) {
            string tempVal = valueNode->getValue();
            idenNode->sEtValue(tempVal);     // gán value của b vào a
            return true;
        } else throw TypeMismatch(this->line);
    }
    return false;
}
bool SymbolTable::isMatch(string& typeA, string& typeB, int& total_nComp, int& total_nSplay) {
    // typeA là type của từng parameter: string hoặc number
    // typeB là chuỗi nhập vào: có thể là value của number, string hoặc iden khác
    int checker = isValueNumberNString(typeB);  // trả về 1 nếu number, 2 string, -1 invalid
    
    if (checker > 0) { // number or string
        if (typeA == "string" && checker == 2) { // match string
            return true;
        }
        if (typeA == "number" && checker == 1) { // match number
            return true;
        } 
        throw TypeMismatch(this->line);
    }
    // identifier
    bool validIden = isValidIdentifier(typeB);
    if (validIden == false) throw InvalidInstruction(this->line);
    if (validIden) {
        SplayTree<Symbol>::Node* strNode = nullptr;

        // tìm iden theo value
        int i = this->level;
        while (i >= 0) {   // search với level giảm dần => make sure là từ block cao nhất đến thấp nhất
            strNode = tree_symb.searchIden(typeB, i);
            if (strNode) break; // found
            i--;
        }
        if (strNode == nullptr) throw Undeclared(this->line);
        total_nComp += tree_symb.getNumCompare();
        total_nSplay += tree_symb.getNumSplay();

        // trường hợp str là hàm -> như v sẽ gọi k đúng kiểu vì đã check isValidIdentifier
        if (strNode->getIsfunction()) throw TypeMismatch(this->line); 

        if (strNode->getType() == typeA) {
            return true;
        } else throw TypeMismatch(this->line);
    }
    return false;
}
bool SymbolTable::isValidAssignFunctionValue(Queue<string>& mykuteValue, string& myValue) {
    // mục tiêu hàm này: check valid của từng parameters rồi push vào cái kute
    // input myValue: sum(a,'abc',123) | sum (number, string, number)->number
    int i = 0;
    int theLength = (int)myValue.length();
    if (myValue[theLength - 1] != ')') return false;
    string idenValue = ""; 

    // tách iden ra có valid hay k: sum
    while (i < theLength) {
        if (myValue[i] != '(') {
            idenValue += myValue[i];
        }
        else {
            break;
        }
        i++;
    }
    if (!isValidIdentifier(idenValue)) throw InvalidInstruction(this->line);
    if (i == theLength - 1) throw InvalidInstruction(this->line);

    // lấy từng parameter, kiểm tra valid rồi push vào kute
    // a -> 'abc' -> 123
    string para = "";
    int numPara = 0;    // số lượng tham số
    i++;
    
    while (i < theLength) {
        if (myValue[i] != ',' && myValue[i] != ')') {
            para += myValue[i];  // đang làm dở tại đây
        }
        if (myValue[i] == ',') {
            int checkParaNumOrString = isValueNumberNString(para);
            bool checkParaIden = isValidIdentifier(para);
           
            if (checkParaIden == false && checkParaNumOrString == -1) return false;  
            mykuteValue.push(para);
            numPara++;
            para = "";
        }
        if (myValue[i] == ')') {
            if (numPara == 0 && para == "") {
                break;
            }
            else {
                 int checkParaNumOrString = isValueNumberNString(para);
                bool checkParaIden = isValidIdentifier(para);
                if (checkParaIden == false && checkParaNumOrString == -1) return false;
                mykuteValue.push(para);
                para = "";
                break;
            }
        }
        i++;
    }
    
    if (i != theLength - 1) return false;
    mykuteValue.push(this->identifier); // as a return value
    
    //cout << "myValue: " << myValue << endl;
    //mykuteValue.println();
    return true;
}
bool SymbolTable::isValidAssignFunctionIden(Queue<string>& mykuteIden, string& myValue) {
    int i = 1;
    int theLength = (int)myValue.length();
    string typee = "";
    int numPara = 0;

    
    // parameters
    while (i < theLength) {
        if (myValue[i] != ',' && myValue[i] != ')') {
            typee += myValue[i];
        }
        if (myValue[i] == ',') {
            mykuteIden.push(typee);
            typee = "";
            numPara++;
        }
        if (myValue[i] == ')') {
            if (numPara == 0 && typee == "") {}  // trường hợp k có tham số: ()->string
            else {
                mykuteIden.push(typee);
            }
            typee = "";
            i++;
            break;
        }   
        i++;
    }

    // return value
    string tempReturn = "";
    if (i < theLength) {
        i+=2;
        while (i < theLength) {
            tempReturn += myValue[i];
            i++;
        }
    }
    mykuteIden.push(tempReturn);

    //cout << "myValueIden: " << myValue << endl;
    //mykuteIden.println();
    return true;
}
bool SymbolTable::assignFunc(int& total_nComp, int& total_nSplay) {
    /*
        idea: ASSIGN a sum(number,b)
        1/ Kiểm tra value trước, dần từ trái sang phải -> tách iden function đầu tiên (gặp '(')
            - kiểm tra iden function có phải function hay ko: không -> TypeMismatch, có lưu lại type của nó
            - từ '(' tới ')': lấy từng parameter split bằng dấu ','  || check trường hợp () trước: 
            - 
    */
    int i = 0;
    string idenValue = ""; // iden of value -> sum
    int theLength = (int)this->value.length();
    Queue<string> mykuteValue;
    Queue<string> mykuteIden;

    // ======> check valid trước rồi làm gì làm <=====
    if (!isValidIdentifier(this->identifier)) throw InvalidInstruction(this->line);
    string tempValue = this->value;
    if (!isValidAssignFunctionValue(mykuteValue, tempValue)) throw InvalidInstruction(this->line);
    
    // lấy iden of value
    while (i < theLength) {
        if (value[i] != '(') idenValue += value[i];
        else {
            break;
        }
        i++;
    }
    if (!isValidIdentifier(idenValue)) throw InvalidInstruction(this->line);

    SplayTree<Symbol>::Node* idenFunc = nullptr;
    
    idenFunc = tree_symb.searchIden(idenValue, 0);
    //cout << "\ngetNumCompare(): " << tree_symb.getNumCompare() << endl;
    //cout << "getNumSplay(): " << tree_symb.getNumSplay() << endl;
    total_nComp += tree_symb.getNumCompare();
    total_nSplay += tree_symb.getNumSplay();
    if (idenFunc == nullptr) throw Undeclared(this->line);
    if (idenFunc->getIsfunction() == false) throw TypeMismatch(this->line);
    //cout << "\nidenFunc: " << idenFunc->getIsfunction() << endl;
    
    string typeOfIdenValue = idenFunc->getType();
    if (!isValidAssignFunctionIden(mykuteIden, typeOfIdenValue)) throw InvalidInstruction(this->line);
    
    // compare 2 kute
    //cout << "\nmykuteValue.size(): " << mykuteValue.size() << "  mykuteIden.size(): " << mykuteIden.size() << endl;
    if (mykuteValue.size() != mykuteIden.size()) return false;
    //mykuteValue.println();
    //mykuteIden.println();
    while (!mykuteIden.empty()) {
        string typeA = mykuteIden.pop();
        string typeB = mykuteValue.pop();
        //cout << "\ntypeA: " << typeA << " || typeB: " << typeB << endl;
        //cout << "\ntotal_nComp: " << total_nComp << endl;
        //cout << "total_nSplay: " << total_nSplay << endl;
        if (!isMatch(typeA, typeB, total_nComp, total_nSplay)) return false;
    }
    
    return true;
}
void SymbolTable::assign() { 
    // number || string
    bool valid = false;
    int total_nComp = 0;
    int total_nSplay = 0;

    valid = assignNumString(total_nComp, total_nSplay);
    if (valid == true) {
        cout << total_nComp << " " << total_nSplay << endl;
        return;
    }

    // iden khac
    total_nComp = 0;
    total_nSplay = 0;
    valid = assignIden(total_nComp, total_nSplay);
    if (valid == true) {
        cout << total_nComp << " " << total_nSplay << endl;
        return;
    }

    // function
    total_nComp = 0;
    total_nSplay = 0;
    valid = assignFunc(total_nComp, total_nSplay);
    if (valid == true) {
        cout << total_nComp << " " << total_nSplay << endl;
        return;
    }
    else throw TypeMismatch(this->line);
}
void SymbolTable::endd() {
    //cout << "\nmyStack before: ";
    //myStack.println();    
    if (this->level >= 0) {
        while (myStack.size() > 0) {
            Symbol tempSymb = myStack.pop();
            if (tempSymb.getLevel() < this->level + 1 ) {
                myStack.push(tempSymb);
                break;
            }
            myStack2.push(tempSymb);
        }
    }
    while (!myStack2.empty()) {
        Symbol temp = myStack2.pop();
        //cout << "\n temp " << temp << endl;
        tree_symb.remove(temp);
    }
    //cout << "\nmyStack after: ";
    //myStack.println();
}
void SymbolTable::lookup() {
    int i = this->level;
    Symbol temp("",identifier,"","",false,i);
    Symbol* temp2 = nullptr;
    while (i >= 0) {
        temp.sEtLevel(i);
        temp2 = tree_symb.lookup(temp);
        if (temp2 != nullptr) {
            cout << temp2->getLevel() << endl;
            return;
        }
        i--; 
    }
    throw Undeclared(line);
}

void SymbolTable::run(string filename)
{
    //myfckingtest();
    
    ifstream myFile(filename);
    if (myFile.is_open()){
        while (myFile.good()) {
            this->command = ""; 
            this->identifier = ""; 
            this->type = ""; 
            this->value = "";
            this->line = "";
            this->statik = "";
            this->isStatik = false;
            this->isFunction = false;
            getline(myFile, line);
            if (line[0] == ' ' || line == "") throw InvalidInstruction(line);
            split();
            
            //cout << "\n\ncommand: " << command << " \niden: " << identifier << " \ntype: " << type << " \nstatik: " << statik << endl;
            
            if (command == "INSERT") {
                this->insert();
            }

            if (command == "ASSIGN") {
                this->assign();
            }

            if (command == "LOOKUP") {
                this->lookup();
            }

            if (command == "END") {
                this->endd();
            }

            if (command == "PRINT") {
                SplayTree<Symbol>::Node* root = nullptr;
                root = tree_symb.getRoot();
                if (root != nullptr) {
                    string resultPreOrder = tree_symb.preOrder();
                    resultPreOrder.pop_back();  // delete the last space
                    cout << resultPreOrder << endl;
                }
                
            }
        }

        //cout << "PreOrder: " << tree_symb.preOrder() << endl;
        if (level != 0) throw UnclosedBlock(this->level);
    }
}