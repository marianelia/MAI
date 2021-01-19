// Лагуткина Мария, м80-106б-19
//вариант 10: квадрат, прямоугольник, трапеция
//
//Создать приложение, которое будет считывать из стандартного ввода данные фигур, согласно варианту задания, выводить их характеристики на экран и записывать в файл.Фигуры могут задаваться как своими вершинами, так и другими характеристиками(например, координата центра, количество точек и радиус).
//Программа должна :
//1.	Осуществлять ввод из стандартного ввода данных фигур, согласно варианту задания;
//2.	Программа должна создавать классы, соответствующие введенным данных фигур;
//3.	Программа должна содержать внутренний буфер, в который помещаются фигуры.Для создания буфера допускается использовать стандартные контейнеры STL.Размер буфера задается параметром командной строки.Например, для буфера размером 10 фигур: oop_exercise_08 10
//4.	При накоплении буфера они должны запускаться на асинхронную обработку, после чего буфер должен очищаться;
//5.	Обработка должна производиться в отдельном потоке;
//6.	Реализовать два обработчика, которые должны обрабатывать данные буфера :
//	        a.Вывод информации о фигурах в буфере на экран;
//	        b.Вывод информации о фигурах в буфере в файл.Для каждого буфера должен создаваться файл с уникальным именем.
//7.	Оба обработчика должны обрабатывать каждый введенный буфер.Т.е.после каждого заполнения буфера его содержимое должно выводиться как на экран, так и в файл.
//8.	Обработчики должны быть реализованы в виде лямбда - функций и должны хранится в специальном массиве обработчиков.Откуда и должны последовательно вызываться в потоке – обработчике.
//9.	В программе должно быть ровно два потока(thread).Один основной(main) и второй для обработчиков;
//10.	В программе должен явно прослеживаться шаблон Publish - Subscribe.Каждый обработчик должен быть реализован как отдельный подписчик.
//11.	Реализовать в основном потоке(main) ожидание обработки буфера в потоке - обработчике.Т.е.после отправки буфера на обработку основной поток должен ждать, пока поток обработчик выведет данные на экран и запишет в файл.


#include <iostream>
#include <fstream>
#include <exception>
#include <list>
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <future>
#include <condition_variable>

using namespace std;

struct Vertex {
    double x = 0;
    double y = 0;

    void Print() const {
        cout << "(" << x << ", " << y << ")";
    }
};

istream& operator>>(istream& input, Vertex& vertex) {
    input >> vertex.x >> vertex.y;
    return input;
}

ostream& operator<<(ostream& output, const Vertex& vertex) {
    output << vertex.x << " " << vertex.y;
    return output;
}

enum class FigureType {
    Square,
    Rectangle,
    Trapezoid   
};

string TypeToString(FigureType type) {
    switch (type) {
    case FigureType::Trapezoid:
        return "Trapezoid";
    case FigureType::Square:
        return "Square";
    case FigureType::Rectangle:
        return "Rectangle";
    default:
        throw  runtime_error("Undefined figure type");
    }
}

class Figure {
public:
    Figure() = default;
    Figure(vector<Vertex> vec) : stats_(vec) {}

    void Print() const {
        cout << TypeToString(GetType()) << ":\n ";
        bool is_first = true;
        for (const Vertex& v : stats_) {
            if (!is_first) {
                cout << ", ";
            }
            is_first = false;
            v.Print();
        }
        cout << "\n";
    }

    virtual FigureType GetType() const = 0;
    virtual ~Figure() {}

    friend  istream& operator>>(istream& input, Figure& rb);
    friend  ostream& operator<<(ostream& output, const Figure& rb);

protected:
    vector<Vertex> stats_;
};

istream& operator>>(istream& input, Figure& figure) {
    for (Vertex& v : figure.stats_) {
        input >> v;
    }
    return input;
}

ostream& operator<<(ostream& output, const Figure& figure) {
    output << static_cast<int>(figure.GetType()) << " ";
    for (const Vertex& v : figure.stats_) {
        output << v << " ";
    }
    return output;
}

class Square : public Figure {
public:
    Square() : Figure(vector<Vertex>(4)) {}
    Square(Vertex a, Vertex  b, Vertex  c, Vertex  d) : Figure({ a, b, c ,d}) {}
    virtual FigureType GetType() const override { return FigureType::Square; }
};

class Rectangle : public Figure {
public:
    Rectangle() : Figure(vector<Vertex>(4)) {}
    Rectangle(Vertex a, Vertex  b, Vertex  c, Vertex d) : Figure({ a, b, c, d }) {}
    virtual FigureType GetType() const override { return FigureType::Rectangle; }
};

class Trapezoid : public Figure {
public:
    Trapezoid() : Figure(vector<Vertex>(4)) {}
    Trapezoid(Vertex a, Vertex  b, Vertex  c, Vertex  d) : Figure({ a, b, c, d}) {}
    virtual FigureType GetType() const override { return FigureType::Trapezoid; }
};



class Document {
public:
    Document() {
        fut_con = async([&]() {Logger(); });

    }

    void Set_size(size_t s) { critical_size = s; }
    ~Document() {
        production_stopped = true;
        cv_consumption.notify_all();
        fut_con.get();
    }

    void Export() {     //запись в файл
        static int i = 0;
        ++i;
        ofstream out("log" + to_string(i) + ".txt");
        for (const auto& ptr : data) {
            out << *ptr << "\n";
        }
        out << "\n";
    }

    void Add(shared_ptr<Figure> figure_ptr) {
        std::unique_lock<std::mutex> lock(m);
        cv_production.wait(lock, [&] { return data.size() < critical_size; });

        data.push_back(move(figure_ptr));

        cv_consumption.notify_all();
    }

    void Print() const {
        static int i = 0;
        ++i;
        cout << i << ":\n";
        for (const auto& ptr : data) {
            ptr->Print();
        }
        cout << "\n";
    }

private:
    list<shared_ptr<Figure>> data;
    size_t critical_size = 3;

    mutex m;
    condition_variable cv_production;
    condition_variable cv_consumption;
    future<void> fut_con;
    bool production_stopped = false;

    void Logger() {
        while (!production_stopped) {
            std::unique_lock<std::mutex> lock(m);
            cv_consumption.wait(lock, [&] { return data.size() >= critical_size || production_stopped; });

            if (!data.empty()) {
                Print();
                Export();
                data.clear();
            }

            cv_production.notify_all();
        }
    }
};

double lenght(Vertex a, Vertex b) {
    return sqrt((b.x - a.x)*(b.x - a.x) + ((b.y - a.y)*(b.y - a.y)));
}
int CorrectInput(Vertex& a, Vertex& b, Vertex& c, Vertex& d, int fig) {
    if (((abs(a.x - c.x) < numeric_limits<double>::epsilon()) &&
        (abs(a.y - c.y) < numeric_limits<double>::epsilon())) ||
        ((abs(b.x - d.x) < numeric_limits<double>::epsilon()) &&
        (abs(b.y - d.y) < numeric_limits<double>::epsilon()))) {
        throw runtime_error("ERROR INPUT\n");
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
        throw runtime_error("ERROR INPUT\n");
    }
    //проверка коректности прямоугольника: равенсво противоположных сторон и диагоналей
    if (fig == 2) {
        if (abs(len_ab - len_cd) < numeric_limits<double>::epsilon() && abs(len_bc - len_da) < numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd) < numeric_limits<double>::epsilon()) {
            return 0;
        }
        throw runtime_error("ERROR INPUT\n");
    }
    //проверка коректности равнобедренной трапеции: равенсво боковых строн и диагоналей
    if (fig == 3) {
        if (abs(len_ab - len_cd) < numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd) < numeric_limits<double>::epsilon()) {
            return 0;
        }
        throw runtime_error("ERROR INPUT\n");
    }
    return 0;
}

void InputError() {
    throw runtime_error("ERROR INPUT\n");
}



int main() {
        try {
            Document doc;
            string cmd;
            size_t size = 3;
            cin >> size;
            doc.Set_size(size);
            while (cin >> cmd) {
             
                if (cmd == "Square") {
                    Vertex a, b, c, d;
                    if (!(cin >> a >> b >> c >> d)) {
                        InputError();
                    }
                    CorrectInput(a, b, c, d, 1);
                    doc.Add(make_shared<Square>(a, b, c, d));
                }
                else if (cmd == "Rectangle") {
                    Vertex a, b, c, d;
                    if (!(cin >> a >> b >> c >> d)) {
                        InputError();
                    }
                    CorrectInput(a, b, c, d, 2);
                    doc.Add(make_shared<Rectangle>(a, b, c, d));
                }
                else if (cmd == "Trapezoid") {
                    Vertex a, b, c, d;
                    if (!(cin >> a >> b >> c >> d)) {
                        InputError();
                    }
                    CorrectInput(a, b, c, d, 3);
                    doc.Add(make_shared<Trapezoid>(a, b, c, d));
                }
                else {
                    cout << cmd << " is not a command\n";
                }
            }
        }
        catch (exception& ex) {
            cerr << ex.what();
        }


    return 0;
}