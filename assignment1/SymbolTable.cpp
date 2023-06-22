#include "SymbolTable.h"


//////////////////////////////////////////////////////////////////////
/////////////////////  SymbolTable Implementation ////////////////////
//////////////////////////////////////////////////////////////////////

void SymbolTable::split(DLinkedList<Symbol*>* pDLL, string& line, string& command, string& identifier, string& type, string& value) {
    
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
    if (choice == -1) throw InvalidInstruction(line); // exception

    i++; // skip a space
    
    if (choice == 1) { // INSERT
        command = temp;
        while (i < (int)line.length()) 
        {
            if (line[i] != ' ') {
                identifier = identifier + line[i];
            }
            else break;
            i++;
        }
        if (!isValidIdentifier(identifier)) throw InvalidInstruction(line);
        i++; // skip a space
        while (i < (int)line.length()) 
        {
            type = type + line[i];
            i++;
        }
        if (!isValidType(type)) {
            throw InvalidInstruction(line);
        }
        
        return;
        //cout << command << " " << identifier << " " << type << endl; 
    }

    if (choice == 2) { // ASSIGN
        command = temp;
        while (i < (int)line.length()) 
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

        Symbol* temp = new Symbol("BEGIN");
        pDLL->add(temp);
    }
    if (choice == 4) {  // END
        this->level--; 
        if (this->level < 0) throw UnknownBlock();

        if (i != (int)line.length() + 1) throw InvalidInstruction(line);

        Symbol* temp = new Symbol("END");
        pDLL->add(temp);
        
    }
    if (choice == 5) {  // LOOKUP
        command = temp;
        while (i < (int)line.length()) 
        {
            identifier = identifier + line[i];
            i++;
        }
        if (!isValidIdentifier(identifier)) throw InvalidInstruction(line);
        // NOTE: dont check the identifier added in the DLL yet
        return;
    }
    if (choice == 6) {  // PRINT
        if (i != (int)line.length() + 1) throw InvalidInstruction(line);
        this->print(pDLL);
        return;
    }
    if (choice == 7) {  // RPRINT
        if (i != (int)line.length() + 1) throw InvalidInstruction(line);
        this->rprint(pDLL);
        return;
    }
}

string splitIdentifier(string& str) {
    string result = "";
    int pos = 0;
    for (int i = 0; i < (int)str.length(); i++) 
    {
        if (str[i] != ' ') {
            pos++;
        }
        else break;
    }
    pos++;
    while (pos < (int)str.length() && str[pos] != ' ') {
        result = result + str[pos];
        pos++;
    }
    return result;
}

int SymbolTable::isValidCommand(string& command) {
    if (command == "INSERT") return 1;
    if (command == "ASSIGN") return 2;
    if (command == "BEGIN") return 3;
    if (command == "END") return 4;
    if (command == "LOOKUP") return 5;
    if (command == "PRINT") return 6;
    if (command == "RPRINT") return 7;
    return -1;
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

bool SymbolTable::isValidType(string& type) {
    if (type == "string" || type == "number") return true;
    return false;
}

bool SymbolTable::insert(DLinkedList<Symbol*>* pDLL) {
    Symbol* symb = new Symbol(this->command, this->identifier, this->type, this->value, this->level);
    int cur_level = level;
    if (pDLL->size() == 0) {
        pDLL->add(symb); 
        return true;
    }

    /*
    DLinkedList<char>::BWDIterator it;
     for(it = DLL.bbegin(); it != DLL.bend(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
    */
    
    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--)
    {
        //cout << "symb: " << (*it)->getCommand() << "  " << (*it)->getIdentifier() << "  " << (*it)->getLevel() << endl;
        if ((*it)->getCommand() == "END") cur_level++;
        if ((*it)->getCommand() == "BEGIN") {
            cur_level--;
            if (cur_level < this->level) {
                pDLL->add(symb);
                return true;
            }
        }
        if ((*it)->getIdentifier() == this->identifier && this->level == cur_level ) {
            delete symb;
            return false;
        }
    }
    pDLL->add(symb);
    return true;
}

int SymbolTable::isValidValue(string& value, string& line) {
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
    
    if ( (isNum == 2) &&   ( ( (int)value[0] != 39 || (int)value[(int)value.length() - 1 ] != 39) || ( (int)value.length() < 2) ) ) return -1;

    return isNum;
}

bool SymbolTable::assignIdenByValue(DLinkedList<Symbol*>* pDLL, string& line) {
    // check value có phải là indentifier khác hay ko?
    this->numEnd = 0;
    bool found = false;
    string vType = "", vValue = "";

    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--) {
        if ((*it)->getCommand() == "END") numEnd++;
        if ((*it)->getCommand() == "BEGIN") {
            if (numEnd > 0) numEnd--;
        }
        if ((*it)->getIdentifier() == this->value && (*it)->getCommand() == "INSERT" && this->numEnd == 0) {
            found = true;
            vType = (*it)->getType();
            vValue = (*it)->getValue();
            break;
        }
    }
    
    if (found == false) {
        return false;
    }
    
    this->numEnd = 0;
    found = false;
    bool foundIdent = false;
    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--) {
        if ((*it)->getCommand() == "END") numEnd++;
        if ((*it)->getCommand() == "BEGIN") {
            if (numEnd > 0) numEnd--;
        }
        if ((*it)->getIdentifier() == this->identifier && (*it)->getCommand() == "INSERT" && this->numEnd == 0) {
            foundIdent = true;
            if ((*it)->getType() == vType) {
                (*it)->sEtValue(vValue);
                return true;
            }
            else throw TypeMismatch(line);
        }
    }
    if (foundIdent == false) throw Undeclared(line);
    return false;
}

bool SymbolTable::assign(DLinkedList<Symbol*>* pDLL, string& line) {
    if (pDLL->size() == 0) {
        return false;
    }
    
    int isNum = isValidValue(value, line);

    if (assignIdenByValue(pDLL, line) == true) {
        return true;
    }
    else {
        if (isNum == -1) throw Undeclared(line);
    }
    
    if (isNum == -1) throw InvalidInstruction(line);

    // STRING OR NUMBER
    this->numEnd = 0;
    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--) {
        if ((*it)->getCommand() == "END") numEnd++;
        if ((*it)->getCommand() == "BEGIN") {
            if (numEnd > 0) numEnd--;
        }
        if ((*it)->getIdentifier() == this->identifier && this->numEnd == 0) {
            if ((*it)->getType() == "number") {
                if (isNum == 1) {
                    (*it)->sEtValue(value);
                    return true;
                } else throw TypeMismatch(line);
            }

            if ((*it)->getType() == "string") {
                if (isNum == 2) {
                    (*it)->sEtValue(value);
                    return true;
                } else throw TypeMismatch(line);
            }
        }
    }
    
    return false;
}

int SymbolTable::lookup(DLinkedList<Symbol*>* pDLL) {
    if (pDLL->size() == 0) {
        return -1;
    }
    
    this->numEnd = 0;
    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--) {
        if ((*it)->getCommand() == "END") this->numEnd++;
        if ((*it)->getCommand() == "BEGIN") {
            if (this->numEnd > 0) this->numEnd--;
        }
        if ((*it)->getIdentifier() == this->identifier && this->numEnd == 0) {
            return (*it)->getLevel();
        }
    }
    return -1;
}

void SymbolTable::print(DLinkedList<Symbol*>* pDLL){
    if (pDLL->size() == 0) return; // empty 

    DLinkedList<Symbol> dll2;

    this->numEnd = 0;
    
    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--) {
        if ((*it)->getCommand() == "END") this->numEnd++;
        if ((*it)->getCommand() == "BEGIN") {
            if (this->numEnd > 0) this->numEnd--;
        }
        else if ((*it)->getCommand() == "INSERT" && this->numEnd == 0) {
            if (dll2.size() == 0) {
                dll2.add(**it);
            }
            else {
                bool found = false;
                for (DLinkedList<Symbol>::BWDIterator it2 = dll2.bbegin(); it2 != dll2.bend(); it2--) {
                    if ((*it2).getIdentifier() == (*it)->getIdentifier()) {
                        found = true;
                        break;
                    }
                }

                if (found == false) {
                    dll2.add(**it);
                }
            }
        }
    }
    /*
    if (dll2.size() != 0) {
        int k = 0;
        for (DLinkedList<Symbol>::BWDIterator it2 = dll2.bbegin(); it2 != dll2.bend(); it2--) {
            cout << (*it2);
            if (k < dll2.size() - 1) cout << " ";
            else cout << endl;
            k++;
        }
    }*/
    
    if (dll2.size() != 0) dll2.printlnBW(&Symbol::toString);
    
}

void SymbolTable::rprint(DLinkedList<Symbol*>* pDLL){
    /*
    if (pDLL->size() == 0) return; // empty 
    //cout << "chay print" << endl;

    DLinkedList<Symbol*> dll3(&DLinkedList<Symbol*>::free, &Symbol::equals);

    this->numEnd = 0;
    
    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--) {
        if ((*it)->getCommand() == "END") this->numEnd++;
        if ((*it)->getCommand() == "BEGIN") {
            if (this->numEnd > 0) this->numEnd--;
        }
        else if ((*it)->getCommand() == "INSERT" && this->numEnd == 0) {
            if (dll3.size() == 0) {
                dll3.add(*it);
            }
            else {
                bool found = false;
                for (DLinkedList<Symbol*>::BWDIterator it2 = dll3.bbegin(); it2 != dll3.bend(); it2--) {
                    if ((*it2)->getIdentifier() == (*it)->getIdentifier() && (*it2)->getIdentifier() != "") {
                        found = true;
                        break;
                    }
                }

                if (found == false) {
                    dll3.add((*it));
                }
            }
        }
    }

    //dll3.println(&Symbol::toString);
    
    if (dll3.size() != 0) {
        for (DLinkedList<Symbol*>::BWDIterator it2 = dll3.bbegin(); it2 != dll3.bend(); it2--) {
            cout << (**it2);
            if (it2 != dll3.bend()) cout << " ";
            else cout << endl;
        }
    }
    */

   if (pDLL->size() == 0) return; // empty 

    DLinkedList<Symbol> dll2;

    this->numEnd = 0;
    
    for (DLinkedList<Symbol*>::BWDIterator it = pDLL->bbegin(); it != pDLL->bend(); it--) {
        if ((*it)->getCommand() == "END") this->numEnd++;
        if ((*it)->getCommand() == "BEGIN") {
            if (this->numEnd > 0) this->numEnd--;
        }
        else if ((*it)->getCommand() == "INSERT" && this->numEnd == 0) {
            if (dll2.size() == 0) {
                dll2.add(**it);
            }
            else {
                bool found = false;
                for (DLinkedList<Symbol>::BWDIterator it2 = dll2.bbegin(); it2 != dll2.bend(); it2--) {
                    if ((*it2).getIdentifier() == (*it)->getIdentifier()) {
                        found = true;
                        break;
                    }
                }

                if (found == false) {
                    dll2.add(**it);
                }
            }
        }
    }
    /*
    if (dll2.size() != 0) {
        int k = 0;
        for (DLinkedList<Symbol>::Iterator it2 = dll2.begin(); it2 != dll2.end(); it2++) {
            cout << (*it2);
            if (k < dll2.size() - 1) cout << " ";
            else cout << endl;
            k++;
        }
    }*/
    
    if (dll2.size() != 0) dll2.println(&Symbol::toString);
}

void SymbolTable::run(string filename) {
    ifstream myFile(filename);

    /*
    string st = "1.0000";
    string line = "line";
    //cout << isValidValue(st, line) << endl; // num

    Symbol* symb = new Symbol("","x", "string", "abc", 1);
    Symbol* symb2 = new Symbol("", "y", "string", "abc", 2);
    Symbol* symb3 = new Symbol("", "z", "string", "s", 3);

    
    DLinkedList<Symbol*> dList(&DLinkedList<Symbol*>::free, &Symbol::equals);
    dList.add(symb);
    dList.add(symb2);
    dList.add(symb3);
    for (DLinkedList<Symbol*>::Iterator it = dList.begin(); it != dList.end(); it++)
    {
        cout << (**it) << " ";
    }
    cout << "\n";
    for(DLinkedList<Symbol*>::BWDIterator it = dList.bbegin(); it != dList.bend(); it--){
        cout << (**it) << " "; //print the item
    }
    */
    
    
    DLinkedList<Symbol*> DLL(&DLinkedList<Symbol*>::free, &Symbol::equals);
    string line;
    
    if (myFile.is_open()){
        
        while (myFile.good()) {
            this->command = ""; 
            this->identifier = ""; 
            this->type = ""; 
            this->value = "";

            getline(myFile, line);
            if (line[0] == ' ' || line == "") throw InvalidInstruction(line);
            split(&DLL, line, this->command, this->identifier, this->type, this->value); // OK

            //cout << command << " " << identifier << " " << type << endl;
            int task = isValidCommand(this->command);

            if ( task == 1) {  // INSERT
                if (insert(&DLL)) {
                    cout << "success" << endl;
                } else {
                    throw Redeclared(line);
                }
            }

            if (task == 2 ) { // ASSIGN
                if (assign(&DLL, line)) {
                    cout << "success" << endl;
                } else {
                    throw Undeclared(line);
                }
            }

            if (task == 3 ) { // BEGIN
            }

            if (task == 4 ) { // END  
            }

            if (task == 5 ) { // LOOKUP
                int lokup = lookup(&DLL);
                if (lokup >= 0) {
                    cout << lokup << endl;
                } else {
                    throw Undeclared(line);
                }
            }

            if (task == 6 ) { // PRINT
            }

            if (task == 7 ) { // RPRINT
            }

        }

        if (level != 0) throw UnclosedBlock(this->level);   
    }
    myFile.close();
}