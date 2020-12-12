//Лагуткина Мария Сергеевна,М8О-206Б-19
//Вариант 10: квадрат, прямоугольник, трапеция

#include <iostream>
#include <fstream>
#include <memory>
#include <stack>
#include <vector>
#include <string>

using namespace std;
using vertex_t = pair<double, double>;

istream& operator>> (istream& input, vertex_t & v) {
    input >> v.first >> v.second;
    return input;
}
ostream& operator<< (ostream& output, const vertex_t v) {
    output << " ("<< v.first <<"," << v.second << ")" << '\n';
    return output;
}
double lenght(vertex_t a, vertex_t b) {
    return sqrt((b.first - a.first)*(b.first - a.first) + ((b.second - a.second)*(b.second - a.second)));
}

int CorrectInput(vertex_t& a, vertex_t& b, vertex_t& c, vertex_t& d, int fig) {
    if (((abs(a.first - c.first) < numeric_limits<double>::epsilon()) &&
        (abs(a.second - c.second) < numeric_limits<double>::epsilon())) || 
        ((abs(b.first - d.first) < numeric_limits<double>::epsilon()) &&
        (abs(b.second - d.second) < numeric_limits<double>::epsilon()))) {
        return 1;
    }
    double len_ab = lenght(a, b);
    double len_bc = lenght(b, c);
    double len_cd = lenght(c, d);
    double len_da = lenght(d, a);
    double len_ac = lenght(a, c);
    double len_bd = lenght(b, d);
    if (fig == 1) {
        if (abs(len_cd - len_ab) < numeric_limits<double>::epsilon() &&
            abs(len_cd - len_bc) < numeric_limits<double>::epsilon() &&
            abs(len_cd - len_da) < numeric_limits<double>::epsilon() &&
            abs(len_bd - len_ac) < numeric_limits<double>::epsilon()) {
            return 0;
        }
        return 1;
    }
    //проверка коректности прямоугольника: равенсво противоположных сторон и диагоналей
    if (fig == 2) {
        if (abs(len_ab - len_cd) < numeric_limits<double>::epsilon() && abs(len_bc - len_da) < numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd) < numeric_limits<double>::epsilon()) {
            return 0;
        }
        return 1;
    }
    //проверка коректности равнобедренной трапеции: равенсво боковых строн и диагоналей
    if (fig == 3) {
        if (abs(len_ab - len_cd) < numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd) < numeric_limits<double>::epsilon()) {
            return 0;
        }
        return 1;
    }
    return 0;
}

class Figure {
public:
    virtual void Set(istream& is) = 0;
    virtual ostream& Print(ostream& output) = 0;
    virtual ~Figure() {};
    virtual void Save(ofstream& os) const = 0;
    virtual void Load(ifstream& is) = 0;
    virtual size_t GetId() = 0;
};
enum figure_t {
    square = 0,
    rectangle = 1,
    trapezoid = 2
};

class Square : public Figure {
public:
    Square() {};
    Square(vertex_t A, vertex_t B, vertex_t C, vertex_t D) {
        a = A; b = B; c = C; d = D;
    };
    void Set(istream& is) override{
       is >> a >> b >> c >> d;
       if (CorrectInput(a, b, c, d, 1) == 1) {
           throw logic_error("It`s not a square");
       }
       id = 0;
    }
    size_t GetId() override{
        return id;
    }
    ostream& Print(ostream& output) {
        //output << "It's square\n";
        output << "A:" << a << '\n' << "B:" << b << '\n' << "C:" << c << '\n' << "D:" << d << '\n';
        return output;
    }
    void Save(ofstream& output) const {
        output << id << " ";
        output << a.first << ' ' << a.second << ' ' << b.first << ' ' << b.second << ' '
            << c.first << ' ' << c.second << ' ' << d.first << ' ' << d.second << '\n';
    }
    void Load(ifstream& input) override{
        input >> a.first >> a.second >> b.first >> b.second >>
            c.first >> c.second >> d.first >> d.second;
        id = 0;
    }
private:
    size_t id;
    vertex_t a;
    vertex_t b;
    vertex_t c;
    vertex_t d;
};


class Rectangle : public Figure {
public:
    Rectangle() {};
    Rectangle(vertex_t A, vertex_t B, vertex_t C, vertex_t D) {
        a = A; b = B; c = C; d = D;
    };
    void Set(istream& is) override {
        is >> a >> b >> c >> d;
        if (CorrectInput(a, b, c, d, 2) == 1) {
            throw logic_error("It`s not a square");
        }
        id = 1;
    }
    size_t GetId() {
        return id;
    }
    ostream& Print(ostream& output) {
        output << "A:" << a << '\n' << "B:" << b << '\n' << "C:" << c << '\n' << "D:" << d << '\n';
        return output;
    }
    void Save(ofstream& output) const {
        output << id << "   ";
        output << a.first << ' ' << a.second << ' ' << b.first << ' ' << b.second << ' '
            << c.first << ' ' << c.second << ' ' << d.first << ' ' << d.second << '\n';
    }

    void Load(ifstream& input) override {
        input >> a.first >> a.second >> b.first >> b.second >>
            c.first >> c.second >> d.first >> d.second;
        id = 1;
    }
private:
    size_t id;
    vertex_t a;
    vertex_t b;
    vertex_t c;
    vertex_t d;
};

class Trapezoid : public Figure {
public:
    Trapezoid() {};
    Trapezoid(vertex_t A, vertex_t B, vertex_t C, vertex_t D) {
        a = A; b = B; c = C; d = D;
    };
    void Set(istream& is) override {
        is >> a >> b >> c >> d;
        if (CorrectInput(a, b, c, d, 3) == 1) {
            throw logic_error("It`s not a square");
        }
        id = 2;
    }
    size_t GetId() {
        return id;
    }
    ostream& Print(ostream& output) {
        //output << "It's trapezoid\n";
        output << "A:" << a << '\n' << "B:" << b << '\n' << "C:" << c << '\n' << "D:" << d << '\n';
        return output;
    }
    void Save(ofstream& output) const {
        output << id << " ";
        output  << a.first <<' ' << a.second << ' ' << b.first << ' ' << b.second << ' '
             << c.first << ' ' << c.second  << ' ' << d.first << ' ' << d.second << '\n';
    }

    void Load(ifstream& input) override {
        input >> a.first >> a.second >> b.first >> b.second >>
            c.first >> c.second >> d.first >> d.second;
        id = 2;
    }
private:
    size_t id;
    vertex_t a;
    vertex_t b;
    vertex_t c;
    vertex_t d;
};

struct Factory {
    static shared_ptr<Figure> create(figure_t t) {
        switch (t) {
        case figure_t::square:
            return make_shared<Square>();
        case figure_t::rectangle:
            return make_shared<Rectangle>();
        case figure_t::trapezoid:
            return make_shared<Trapezoid>();
        }
    }
};

struct Memento {
    vector<shared_ptr<Figure>> state;   //переписать на указателях
    vector<size_t> id;
    Memento() {};
    Memento(vector<shared_ptr<Figure>> other){
        state = other;
    };
};

struct Originator {  //структура, хранящая стек состояний
    stack<Memento> mementos;
    void createMemento(vector<shared_ptr<Figure>> state) {
        mementos.emplace(state);

    }
    vector<shared_ptr<Figure>> restore() {
        if (!mementos.empty()) {
            vector<shared_ptr<Figure>> result = move(mementos.top().state);
            mementos.pop();
            return result;
        }
        else throw logic_error("undo stack empty");
    }

};

class Document {
public:
    void DSave(vector<shared_ptr<Figure>> &f,string& file) {
        ofstream os(file);
        if (!os) {
            cerr << "INCORRECT PATH\n";
            return;
        }
        os << f.size() << ' ';
        for (int j = 0; j < f.size(); ++j) {
            f[j]->Save(os);
        }
    }
    void DLoad(vector<shared_ptr<Figure>> &figures, string& file) {
        ifstream is(file);
        int i;
        shared_ptr<Figure> fig = nullptr;
        if (!is) {
            cerr << "INCORRECT PATH\n";
            return;
        }
        int s;
        is >> s;
        for (int j = 0; j < s; ++j) {
            is >> i;
            fig = Factory::create((figure_t)i);
            fig->Load(is);
            figures.push_back(move(fig));
        }
    }
};

void Usage() {
    cout << "Enter choice:\n"
        "0 - create document\n"
        "1 - add\n"
        "2 - remove\n"
        "3 - undo\n"
        "4 - save in file\n"
        "5 - load out file\n"
        "6 - print all figures\n"
        "7 - print usage\n\n";
    cout << "Types of figures:\n"
        "0 - square\n"
        "1 - rectangle\n"
        "2 - trapezoid\n";
}

int main(){
    Originator origin;
    char choice;
    int index;
    int i;
    int idx;
    string path ="";
    Document doc;
    vector<shared_ptr<Figure>> figures;
    shared_ptr<Figure> fig = nullptr;
    Usage();
    while (true) {
        cin >> choice;
        switch (choice) {
        case '1':
            cin >> i;   
            fig = Factory::create((figure_t)i);
            try {
                fig->Set(cin);
            }
            catch (logic_error) { cerr << "ERROR INPUT\n"; break; }
            origin.createMemento(figures);
            figures.push_back(move(fig));
            cout << "OK\n";
            break;
        case '2':
            cout << "Enter index: ";
            cin >> index; 
            if (index >= 0 && index < figures.size()) {

                origin.createMemento(figures);
                figures.erase(figures.begin() + index);
                cout << "OK\n";
                break;
            }
            else { cerr << "BORDER OVERLAY\n"; break; }
        case '3':
            figures = origin.restore();
            cout << "OK\n";
            break;

        case '4':
            cout << "Enter path: ";
            cin >> path;
            doc.DSave(figures, path);
            cout << "Ok\n";
            break;
        case '5':
            cout << "Enter path: ";
            cin >> path;
            figures.clear();
            doc.DLoad(figures, path);
            origin.createMemento(figures);
            cout << "OK\n";
            break;
        case '6':
            idx = 0;
            for (int j = 0; j < figures.size();++j) {
                cout << "No:" << ++idx << " - ";
                if (figures[j]->GetId() == 0) { cout << "It's square\n"; }
                if (figures[j]->GetId() == 1) { cout << "It's rectangle\n"; }
                if (figures[j]->GetId() == 2) { cout << "It's trapezoid\n"; }
                cout << "Figure coordinates:\n";
                figures[j]->Print(cout);
            }
            break;
        case '7':
            Usage();
            break;
        default:
            cerr << "ERROR COMMAND\n";
            break;
        }
        
    }

    return 0;
}

