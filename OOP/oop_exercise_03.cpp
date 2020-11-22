// Лагуткина Мария, М8О-206Б-19
//Вариант 10: фигуры квадрат, прямоугольник, трапеция
//Разработать классы согласно варианту задания, классы должны наследоваться от базового класса Figure.Фигуры являются фигурами вращения.
//Все классы должны поддерживать набор общих методов :
//1.       Вычисление геометрического центра фигуры;
//2.       Вывод в стандартный поток вывода std::cout координат вершин фигуры;
//3. Вычисление площади фигуры;
//Создать программу, которая позволяет :
//•       Вводить из стандартного ввода std::cin фигуры, согласно варианту задания.
//•       Сохранять созданные фигуры в динамический массив std::vector<Figure*>
//•       Вызывать для всего массива общие функции(1 - 3 см.выше).Т.е.распечатывать для каждой фигуры в массиве геометрический центр, координаты вершин и площадь.
//•       Необходимо уметь вычислять общую площадь фигур в массиве.
//•       Удалять из массива фигуру по индексу;

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
    Point() { 0, 0; };
    Point(double new_x, double new_y) {
        x = new_x;
        y = new_y;
    }
    double x;
    double y;
};

istream& operator >> (istream& input, Point& point) {
   return input >> point.x >> point.y;
}

ostream& operator << (ostream& output, Point& point) {
    return output << "(" << point.x << "," << point.y << ")";
}

class Figure {
public:
    virtual Point Center() const = 0;
    virtual ostream& Print(ostream& output) = 0;
    virtual double Area() const = 0;
    virtual ~Figure(){};
protected:
    Point a;
    Point b;
    Point c;
    Point d;
};

class Square : public Figure {
public:
    Square() {};
    Square(Point A, Point B, Point C, Point D) { a = A; b = B; c = C; d = D; };
    Point Center()const override {
        Point ans;
        ans.x = (a.x + c.x) / 2;
        ans.y = (a.y + c.y) / 2;
        return ans;
    }
    double Area()const override{
        Point d1;
        double diag;
        d1.x = abs(c.x - a.x);
        d1.y = abs(c.y - a.y);
        diag = sqrt(d1.x*d1.x + d1.y*d1.y);
        return diag*diag / 2;
    }
    ostream& Print (ostream& output) override{
        output << "A" << a << '\n' << "B" << b << '\n' << "C" << c << '\n' << "D" << d << '\n';
        return output;
    }
private:
    Point a;
    Point b;
    Point c;
    Point d;
};

class Rectangle : public Figure {
public:
    Rectangle() {};
    Rectangle(Point A, Point B, Point C, Point D) { a = A; b = B; c = C; d = D; };
    Point Center()const override {
        Point ans;
        ans.x = (a.x + c.x) / 2;
        ans.y = (a.y + c.y) / 2;
        return ans;
    }
    double Area()const override {
        return abs(a.x*b.y + b.x*c.y + c.x*d.y + d.x*a.y - b.x*a.y - c.x*b.y - d.x*c.y - a.x*d.y);
    }
    ostream& Print(ostream& output) {
        output << "A" << a << '\n' << "B" << b << '\n' << "C" << c << '\n' << "D" << d << '\n';
        return output;
    }
private:
    Point a;
    Point b;
    Point c;
    Point d;
};

class Trapezoid : public Figure {       //ðàâíîáåäðåííàÿ òðàïåöèÿ
public:
    Trapezoid() {};
    Trapezoid(Point A, Point B, Point C, Point D) { a = A; b = B; c = C; d = D; };
    Point Center()const override {
        Point ans;
        ans.x = (a.x + b.x + c.x + d.x) / 4;
        ans.y = (a.y + b.y + c.y + d.y) / 4;
        return ans;
    }
    double Area()const override {
        return abs(a.x*b.y + b.x*c.y + c.x*d.y + d.x*a.y - b.x*a.y - c.x*b.y - d.x*c.y - a.x*d.y);
    }
    ostream& Print(ostream& output) {
        output << "A" << a << '\n' << "B" << b << '\n' << "C" << c << '\n' << "D" << d << '\n';
        return output;
    }
private:
    Point a;
    Point b;
    Point c;
    Point d;
};

double lenght(Point a, Point b) {       //âû÷èñëåíèå äëèíû ñòîðîíû
    return sqrt((b.x - a.x)*(b.x - a.x) + ((b.y - a.y)*(b.y - a.y)));
}

int CorrectInput(Point& a, Point& b, Point& c, Point& d, int fig){ 
    if ((a.x == c.x && a.y == c.y) || (b.x == d.x && b.y == d.y)) {
        return 1;
    }
    double len_ab = lenght(a, b);
    double len_bc = lenght(b, c);
    double len_cd = lenght(c, d);
    double len_da = lenght(d, a);
    double len_ac = lenght(a, c);
    double len_bd = lenght(b, d);
    if (fig == 1) {
        if (abs(len_cd - len_ab)<numeric_limits<double>::epsilon() && 
            abs(len_cd - len_bc)<numeric_limits<double>::epsilon() &&
            abs(len_cd - len_da)<numeric_limits<double>::epsilon() && 
            abs(len_bd - len_ac)<numeric_limits<double>::epsilon()) {
            return 0;
        }
        return 1;
    }
   //проверка коректности прямоугольника: равенсво противоположных сторон и диагоналей
    if (fig == 2) {
        if (abs(len_ab - len_cd)<numeric_limits<double>::epsilon() &&
            abs(len_bc - len_da)<numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd)<numeric_limits<double>::epsilon()) {      
            return 0;
        }
        return 1;
    }
    //проверка коректности равнобедренной трапеции: равенсво боковых строн и диагоналей
    if (fig == 3 ) {
        if (abs(len_ab - len_cd)<numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd)<numeric_limits<double>::epsilon()) {  
            return 0;
        }
        return 1;
    }
    return 0;
}

int main() {
    vector<Figure*> figures;
    Figure* f = nullptr;
    Point a, b, c, d;
    int t;
    int count_sq = 0, count_rest = 0, count_trap = 0;
    cout << "Enter the number of squares, restangles, trapezoids:" << '\n';
    cin >> count_sq >> count_rest >> count_trap;
    if (count_sq < 0 || count_rest < 0 || count_trap < 0) {
        cerr << "Error input: count";
        return 1;
    }
    if (count_sq == 0 && count_rest == 0 && count_trap == 0) {
        return 0;
    }

    for (int i = 0; i < count_sq; ++i) {    
        cout << "Enter squares:" << '\n';
        cin >> a >> b >> c >> d;
        t = CorrectInput(a, b, c, d, 1);
        if(t == 1) { cerr << "Error input: square"; return 2; }
        f = new Square{a,b,c,d};
        figures.push_back(f);
    }
    for (int i = 0; i < count_rest; ++i) {   
        cout << "Enter rectangle:" << '\n';
        cin >> a >> b >> c >> d;
        t = CorrectInput(a, b, c, d, 2);
        if (t == 1) { cerr << "Error input: rectangle"; return 3; }
        Rectangle r(a, b, c, d);
        f = new Rectangle{ a,b,c,d };
        figures.push_back(f);
    }
    for (int i = 0; i < count_trap; ++i) {  
        cout << "Enter trapezoid:" << '\n';
        cin >> a >> b >> c >> d;
        t = CorrectInput(a, b, c, d, 3);
        if (t == 1) { cerr << "Error input: trapezoid"; return 4; }
        Trapezoid tr(a, b, c, d);
        f = new Trapezoid{ a,b,c,d };
        figures.push_back(f);
    }
    int ind = -1;
    cout << "\nWant to delete a figure? \nSpecify the index of the figure.\n" <<
        "If you don't want to delete the figure, press -1\n";
    cin >> ind;
    if (ind > -1 && ind < figures.size()) {
        delete figures[ind];
        figures[ind] = nullptr;
        cout << "Delete successfully\n";
    }
    else
        cout << "Delete error\n";
    for (ind; ind < figures.size()-1; ++ind) {
        figures[ind] = figures[ind + 1];
    }
    figures.pop_back();

    double sum_area = 0, i = 0;
    for (auto& x : figures) {
        cout <<"No:"<< ++i << '\n' << "Figure coordinates:" << '\n';
        x->Print(cout);
        cout << "Figure center: " << x->Center() << '\n';
        cout << "Figure area: " << x->Area() << '\n';
        sum_area += x->Area();
    }
    cout << "\nTotal area: " << sum_area;
    for (int i = 0; i < figures.size(); ++i) {
        delete figures[i];
    }
    return 0;
}
