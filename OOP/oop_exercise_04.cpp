
#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <cmath>
#include <tuple>
#include <fstream>
#include <string>
#include <utility>

using namespace std;

template <class T>
using vertex_t = pair<T, T>;

template <class T>
istream& operator>> (istream& input, vertex_t<T>& v) {
    input >> v.first >> v.second;
    return input;
}

template<class T>
ostream& operator<< (ostream& output, const vertex_t<T> v) {
    output << "(" << v.first << "," << v.second << ")" << '\n';
    return output;
}


template <class T>
class Square{
public:
    vertex_t<T> a, b, c, d;
};

template <class T>
class Rectangle {
public:
    vertex_t<T> a, b, c, d;
};

template <class T>
class Trapezoid {
public:
    vertex_t<T> a, b, c, d;
};

template <class T>
double area(const Square<T>& sq) {
    return abs(sq.a.first*sq.b.second + sq.b.first*sq.c.second + sq.c.first*sq.d.second + sq.d.first*sq.a.second
        - sq.b.first*sq.a.second - sq.c.first*sq.b.second - sq.d.first*sq.c.second - sq.a.first*sq.d.second)/2;
}

template <class T>
double area(const Rectangle<T>& re) {
    return abs(re.a.first*re.b.second + re.b.first*re.c.second + re.c.first*re.d.second + re.d.first*re.a.second
        - re.b.first*re.a.second - re.c.first*re.b.second - re.d.first*re.c.second - re.a.first*re.d.second)/2;
}

template <class T>
double area(const Trapezoid<T>& tr) {
    return abs(tr.a.first*tr.b.second + tr.b.first*tr.c.second + tr.c.first*tr.d.second + tr.d.first*tr.a.second
        - tr.b.first*tr.a.second - tr.c.first*tr.b.second - tr.d.first*tr.c.second - tr.a.first*tr.d.second)/2;
}

template <class T>
istream& operator>> (istream& input, Square<T>& sq) {
    input >> sq.a >> sq.b >> sq.c >> sq.d;
    return input;
}

template <class T>
istream& operator>> (istream& input, Rectangle<T>& re) {
    input >> re.a >> re.b >> re.c >> re.d;
    return input;
}

template <class T>
istream& operator>> (istream& input, Trapezoid<T>& tr) {
    input >> tr.a >> tr.b >> tr.c >> tr.d;
    return input;
}

template <class T>
void print(const Square<T>& sq) {
    cout << sq.a << ' ' << sq.b << ' ' << sq.c << ' '  << sq.d << '\n';
    return;
}

template <class T>
void print(const Rectangle<T>& re) {
    cout << re.a << ' ' << re.b << ' ' << re.c << ' ' << re.d << '\n';
    return;
}

template <class T>
void print(const Trapezoid<T>& tr) {
    cout << tr.a << ' ' << tr.b << ' ' << tr.c << ' ' << tr.d << '\n';
    return;
}

template <class... Elements>
void print(const tuple<Elements...> t) {
    if (tuple_size<tuple<Elements...>>::value != 6) {
        cerr << "Incorrect number of figures entered.\n";
        return;
    }
    print(get<0>(t));
    print(get<1>(t));
    print(get<2>(t));
    print(get<3>(t));
    print(get<4>(t));
    print(get<5>(t));
    return;
}

template<class... Elements>
void area(const tuple<Elements...> t) {
    if (tuple_size<tuple<Elements...>>::value != 6) {
        cerr << "Incorrect number of figures entered.\n";
        return;
    }
    double sum_area = 0;
    sum_area += area(get<0>(t));
    sum_area += area(get<1>(t));
    sum_area += area(get<2>(t));
    sum_area += area(get<3>(t));
    sum_area += area(get<4>(t));
    sum_area += area(get<5>(t));
    cout <<"\nTotal area: "<< sum_area;
    return;
}

template<class T>
double lenght(vertex_t<T>& a, vertex_t<T>& b) {       //âû÷èñëåíèå äëèíû ñòîðîíû
    return sqrt((b.first - a.first)*(b.first - a.first) + ((b.second - a.second)*(b.second - a.second)));
}

template<class T>
int CorrectInput(pair<T, T>& a, pair<T, T>& b, pair<T, T>& c, pair<T, T>& d, int f) {      //ïðîâåðêà êîðåêòíîñòè ââåäåííîé ôèãóðû
    if ((a.first == c.first && a.second == c.second) || (b.first == d.first && b.second == d.second)) {
        return 1;
    }
    double len_ab = lenght(a, b);
    double len_bc = lenght(b, c);
    double len_cd = lenght(c, d);
    double len_da = lenght(d, a);
    double len_ac = lenght(a, c);
    double len_bd = lenght(b, d);
    //ïðîâåðêà êîðåêòíîñòè êâàäðàòà: ðàâåíñâî ñòðîí è äèàãîíàëåé
    if (f == 1) {
        if (abs(len_cd - len_ab)<numeric_limits<double>::epsilon() &&
            abs(len_cd - len_bc)<numeric_limits<double>::epsilon() &&
            abs(len_cd - len_da)<numeric_limits<double>::epsilon() &&
            abs(len_bd - len_ac)<numeric_limits<double>::epsilon()) {
            return 0;
        }
        return 1;
    }
    //ïðîâåðêà êîðåêòíîñòè ïðÿìîóãîëüíèêà: ðàâåíñâî ïðîòèâîïîëîæíûõ ñòîðîí è äèàãîíàëåé
    if (f == 2) {
        if (abs(len_ab - len_cd)<numeric_limits<double>::epsilon() &&
            abs(len_bc - len_da)<numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd)<numeric_limits<double>::epsilon()) {
            return 0;
        }
        return 1;
    }
    //ïðîâåðêà êîðåêòíîñòè ðàâíîáåäðåííîé òðàïåöèè: ðàâåíñâî áîêîâûõ ñòðîí è äèàãîíàëåé
    if (f == 3) {
        if (abs(len_ab - len_cd)<numeric_limits<double>::epsilon() &&
            abs(len_ac - len_bd)<numeric_limits<double>::epsilon()) {
            return 0;
        }
        return 1;
    }
    return 0;
}

int main() {
    //Square<int> s;
    //Rectangle<double> r;
    //Trapezoid<int> t;

    pair<int, int> a_int, b_int, c_int, d_int;
    pair<double, double> a_dob, b_dob, c_dob, d_dob;

    cout << "Enter squares of integer:" << '\n';
    cin >> a_int >> b_int >> c_int >> d_int;
    int f = CorrectInput(a_int, b_int, c_int, d_int, 1);
    if (f == 1) { cerr << "Error input: square"; return 2; }
    Square<int> s_i{a_int,b_int,c_int,d_int };

    cout << "Enter squares of double:" << '\n';
    cin >> a_dob >> b_dob >> c_dob >> d_dob;
    f = CorrectInput(a_dob, b_dob, c_dob, d_dob, 1);
    if (f == 1) { cerr << "Error input: square"; return 2; }
    Square<double> s_d{ a_dob,b_dob,c_dob,d_dob };

    cout << "Enter rectangle of integer:" << '\n';
    cin >> a_int >> b_int >> c_int >> d_int;
    f = CorrectInput(a_int, b_int, c_int, d_int, 2);
    if (f == 1) { cerr << "Error input: square"; return 2; }
    Rectangle<int> r_i{ a_int,b_int,c_int,d_int };

    cout << "Enter rectangle of double:" << '\n';
    cin >> a_dob >> b_dob >> c_dob >> d_dob;
    f = CorrectInput(a_dob, b_dob, c_dob, d_dob, 2);
    if (f == 1) { cerr << "Error input: square"; return 2; }
    Rectangle<double> r_d{ a_dob,b_dob,c_dob,d_dob };

    cout << "Enter trapezoid of integer:" << '\n';
    cin >> a_int >> b_int >> c_int >> d_int;
    f = CorrectInput(a_int, b_int, c_int, d_int, 3);
    if (f == 1) { cerr << "Error input: square"; return 2; }
    Trapezoid<int> t_i{ a_int,b_int,c_int,d_int };

    cout << "Enter trapezoid of double:" << '\n';
    cin >> a_dob >> b_dob >> c_dob >> d_dob;
    f = CorrectInput(a_dob, b_dob, c_dob, d_dob, 3);
    if (f == 1) { cerr << "Error input: square"; return 2; }
    Trapezoid<double> t_d{ a_dob,b_dob,c_dob,d_dob };

    auto tup = make_tuple(s_i, s_d, r_i, r_d, t_i, t_d);
    print(tup);
    area(tup);
    
    _getch();
    return 0;
}
