#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <utility>
#include <algorithm>
#include <chrono>

using namespace std;

const size_t LENGTH_KEY = 256;

class TString {
public:
    TString() : buf(nullptr), size(0), capacity(0) {}

    TString(char const* string) {
        this->size = strlen(string);
        this->capacity = this->size + 1;
        this->buf = new char[this->capacity];
        memcpy(this->buf,(void*)string, strlen(string) + 1);
        this->buf[this->size] = '\0';
    }
    //Конструктор с копированием строки из TString 
    TString(TString const& string) {
        this->capacity = string.capacity;
        this->size = string.size;
        this->buf = new char[this->capacity];
        memcpy(this->buf, (void*)string.buf, strlen(string.buf) + 1);
        this->buf[this->size] = '\0';
    }

    void Move(char* string) {
        delete[] this->buf;
        this->buf = string;
        this->size = strlen(string);
        this->capacity = this->size + 1;
        string = nullptr;
    }

    ~TString() {
        delete[] this->buf;
        this->buf = nullptr;
        size = 0;
        capacity = 0;
    }

    //присваивание строки (преобразование из TString в TString)
    TString& operator=(TString const& newString) {
        char* tmpString = new char[newString.capacity];
        memcpy(tmpString, (void*)newString.buf, strlen(newString.buf) + 1);
        delete[] this->buf;
        this->buf = tmpString;
        tmpString = nullptr;
        this->size = newString.size;
        this->capacity = newString.capacity;
        return *this;
    }

    char& operator[](size_t index){
        return buf[index];
    }

    size_t GetSize() const {
        return size;
    }

    char* GetString() const {
        return buf;
    }

private:
    char *buf;
    size_t size;
    size_t capacity;
};

istream& operator>>(istream& input, TString& string) {
    char bufInput[LENGTH_KEY];
    if (input >> bufInput) {
        string = bufInput;
    }
    return input;
}

ostream& operator<<(ostream& output, TString& string){
    for (size_t i = 0; i < string.GetSize(); ++i) {
        output << string[i]; 
    }
    return output;
}

bool operator<(TString& lhs, TString& rhs) {
    size_t minSize = min(lhs.GetSize(), rhs.GetSize());
    for (size_t i = 0; i < minSize; ++i) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] < rhs[i];
        }
    }
    return lhs.GetSize() < rhs.GetSize();
}

bool operator>(TString& lhs, TString& rhs) {
    return rhs < lhs;
}

class TAvlTree {
private:
    struct TAvlElement {
        TString key;
        uint64_t value;
        uint64_t hight;
        TAvlElement *left;
        TAvlElement *right;
        TAvlElement() : key(), value(), left(nullptr), right(nullptr), hight(0) {}
        TAvlElement(TString newKey, uint64_t newValue) 
            : key(newKey), value(newValue), left(nullptr), right(nullptr), hight(1) {}
        TAvlElement(TString newKey, uint64_t newValue, size_t newHight)
            : key(newKey), value(newValue), left(nullptr), right(nullptr), hight(newHight) {}
    };

    TAvlElement *root;

    void DeleteTree(TAvlElement* el) {
        if (el == nullptr) {
            return;
        }
        DeleteTree(el->left);
        DeleteTree(el->right);
        delete el;
    }

    //приведение к нижнему регистру
    void Lower(TString& str) {
        size_t size = str.GetSize();
        for (size_t i = 0; i < size; ++i) {
            str[i] = tolower(str[i]);
        }
    }

    uint64_t RecountHight(TAvlElement* el){
        uint64_t tl = (el->left != nullptr ? el->left->hight : 0);
        uint64_t tr = (el->right != nullptr ? el->right->hight : 0);
        return max(tl,tr) + 1;
    }

    int CountBalance(TAvlElement* el) {
        uint64_t tl = (el->left != nullptr ? el->left->hight : 0);
        uint64_t tr = (el->right != nullptr ? el->right->hight : 0);
        return tl - tr;
    }

    TAvlElement* RightRotate(TAvlElement* x) {
        TAvlElement* y = x->left;
        x->left = y->right;
        y->right = x;
        x->hight = RecountHight(x);
        y->hight = RecountHight(y);
        return y;
    }

    TAvlElement* LeftRotate(TAvlElement* y) {
        TAvlElement* x = y->right;
        y->right = x->left;
        x->left = y;
        y->hight = RecountHight(y);
        x->hight = RecountHight(x);
        return x;
    }

    TAvlElement* RecountBalance(TAvlElement* el) {
        if (el == nullptr) { return nullptr; }
        el->hight = RecountHight(el);
        int bal = CountBalance(el);
        if (bal == 2) {
            if (CountBalance(el->left) == -1) {
                el->left = LeftRotate(el->left);
                return RightRotate(el);
            }
            else {
                return RightRotate(el);
            }
        }
        if (bal == -2) {
            if (CountBalance(el->right) == 1) {
                el->right = RightRotate(el->right);
                return LeftRotate(el);
            }
            else {
                return LeftRotate(el);
            }
        }
        return el;
    }

    TAvlElement* InsertElement(TAvlElement* el, TString key, uint64_t value) {
        if (el == nullptr) {
            el = new TAvlElement(key, value);
            cout << "OK\n";
            return el;
        }
        if (key < el->key) {
            el->left = InsertElement(el->left, key, value);
        }
        else if(key > el->key) {
            el->right = InsertElement(el->right, key, value);
        }
        else { cout << "Exist\n"; }
        return RecountBalance(el);  //пересчет высоты, баланса и соответстующие ему повороты
    }

    TAvlElement* DeleteLeftMinimum(TAvlElement* el, TAvlElement* tmp) {
        if (tmp->left == nullptr) {
            TAvlElement* tmpRight = tmp->right;
            el->key = move(tmp->key);
            el->value = tmp->value;
            delete tmp;
            tmp = tmpRight;
            cout << "OK\n";
        }
        else {
            tmp->left = DeleteLeftMinimum(el, tmp->left);
        }
        return RecountBalance(tmp);
    }

    TAvlElement* DeleteElement(TAvlElement* el, TString key) {
        if (el == nullptr) {
            cout << "NoSuchWord\n";
            return nullptr;
        }
        if (key < el->key) {
            el->left = DeleteElement(el->left, key);
        }
        else if (key > el->key) {
            el->right = DeleteElement(el->right, key);
        }
        else { 
            TAvlElement* tmp;
            if (el->left == nullptr && el->right == nullptr) {
                delete el;
                cout << "OK\n";
                return nullptr;
            }
            if (el->left == nullptr) {
                tmp = el->right;
                delete el;
                cout << "OK\n";
                return tmp;
            }
            if (el->right == nullptr) {
                tmp = el->left;
                delete el;
                cout << "OK\n";
                return tmp;
            }
            el->right = DeleteLeftMinimum(el, el->right);
        }
        return RecountBalance(el);  //пересчет высоты, баланса и соответстующие ему повороты
    }

    TAvlElement* SearchElement(TAvlElement* el, TString key) {
        if (el == nullptr) {
            return nullptr;
        }
        TAvlElement* tmp = el;
        while (tmp != nullptr) {
            if (key < tmp->key) {
                tmp = tmp->left;
            }
            else if (key > tmp->key) {
                tmp = tmp->right;
            }
            else { return tmp; }
        }
        return nullptr;
    }

    void SerializeTree(std::ostream& output, TAvlElement* el){
        if (el == nullptr) {
            return; 
        }
        bool haveLeft = false;
        bool haveRight = false;
        size_t keySize = el->key.GetSize();
        output.write((char*)(&keySize), sizeof(keySize));
        output.write(el->key.GetString(), keySize);
        output.write((char*)&el->value, sizeof(el->value));
        output.write((char*)&el->hight, sizeof(el->hight));
        if (el->left != nullptr) {
            haveLeft = true;
        }
        output.write((char*)(&haveLeft), sizeof(haveLeft));
        if (el->right != nullptr) {
            haveRight = true;
        }
        output.write((char*)(&haveRight), sizeof(haveRight));
        if (haveLeft) {
            SerializeTree(output, el->left);
        }
        if (haveRight) {
            SerializeTree(output, el->right); 
        }
    }

    TAvlElement* DeserializeTree(std::istream& input, TAvlElement* el) {
        (void)(el);
        TAvlElement* root = nullptr;
        size_t keySize;
        bool haveLeft = false;
        bool haveRight = false;
        input.read((char*)(&keySize), sizeof(keySize));
        if (input.gcount() == 0) {
            return root;
        }
        uint64_t val;
        uint64_t hi;
        char* key = new char[keySize + 1];
        key[keySize] = '\0';
        input.read(key, keySize);
        input.read((char*)(&val), sizeof(val));
        input.read((char*)(&hi), sizeof(hi));
        input.read((char*)(&haveLeft), sizeof(haveLeft));
        input.read((char*)(&haveRight), sizeof(haveRight));

        root = new TAvlElement();
        root->key.Move(key);
        root->value = val;
        root->hight = hi;

        if (haveLeft) {
            root->left = DeserializeTree(input, root);
        }
        else { root->left = nullptr; }
        if (haveRight) {
            root->right = DeserializeTree(input, root);
        }
        else { root->right = nullptr; }

        return root;
    }
    
   

    bool SaveInFile(TString& file) {
        ofstream output(file.GetString(), std::ios::binary | std::ios::out);
        if (!output) {
            return false;
        }
        SerializeTree(output, this->root);
        return true;
    }

    bool LoadOfFile(TString file) {
        ifstream input(file.GetString(), std::ios::binary | std::ios::in);
        if (!input) {
            return false;
        }
        DeleteTree(root);
        root = DeserializeTree(input,nullptr);
        input.close();
        return true;
    }

public:
    TAvlTree() : root(nullptr) {}
    ~TAvlTree() { DeleteTree(root); }

    void Insert() {
        TString key;
        uint64_t value = 0;
        cin >> key >> value;
        Lower(key);
        this->root = InsertElement(this->root, move(key), value);
    }

    void Delete() {
        TString key;
        cin >> key;
        Lower(key);
        this->root = DeleteElement(this->root, move(key));
    }

    void Search(TString key) {
        Lower(key);
        TAvlElement* tmp = SearchElement(this->root, move(key));
        if (tmp != nullptr) {
            cout << "OK: " << tmp->value << '\n';
        }
        else {
            cout << "NoSuchWord\n";
        }
    }

    void SaveAndLoad(){
        TString cmd;
        TString path;
        std::cin >> cmd >> path;
        if (cmd[0] == 'S'){
            if (!SaveInFile(path)){
                std::cout << "ERROR: can't open file\n";
                return;
            }
        }
        if (cmd[0] == 'L') {
            if (!LoadOfFile(path)){
                std::cout << "ERROR: can't open file\n";
                return;
            }
        }
        std::cout << "OK\n";
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    TAvlTree tree;
    TString command;
    while (cin >> command) {
        if (command[0] == '+') {
            tree.Insert(); 
        }
        else if (command[0] == '-') {
            tree.Delete(); 
        }
        else if (command[0] == '!') {
            tree.SaveAndLoad(); 
        }
        else {
            tree.Search(command); 
        }
    }
    return 0;
}

