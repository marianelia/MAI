//Лагуткина Мария, М8О-206Б-19
//Вариант 3
//Рациональная(несократимая) дробь представляется парой целых чисел(a, b), где a — числитель, b — знаменатель.
//Создать класс Rational для работы с рациональными дробями.Обязательно должны быть реализованы операции :
//-сложения add,  (a, b) + (c, d) = (ad + bc, bd);
//-вычитания sub, (a, b) – (c, d) = (ad – bc, bd);
//-умножения mul, (a, b) * (c, d) = (ac, bd);
//-деления div,   (a, b) / (c, d) = (ad, bc);
//-операции сравнения.
//Должна быть реализована функция сокращения дроби reduce(), которая обязательно вызывается при выполнении арифметических операций.
//Операции сложения, вычитания, умножения, деления, сравнения(на равенство, больше и меньше) 
//должны быть выполнены в виде перегрузки операторов.
//Необходимо реализовать пользовательский литерал для работы с константами типа Rational.


#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <conio.h>

using namespace std;

int gcd(int x, int y) {
	while (x != 0) {
		y %= x;
		swap(x, y);
	}
	return y;
}

class Rational {
public:
	Rational() {}
	Rational(int numerator, int denumerator) {
		a = numerator;
		b = denumerator;
	}
	~Rational() {}
	int GetA() const { return a; }
	int GetB() const { return b; }
	
	void reduce() {		//сокращение дроби
		int div = gcd(a, b);
		a /= div;
		b /= div;
	}

	friend Rational operator+(Rational fraction1, Rational fraction2);
	friend Rational operator-(Rational fraction1, Rational fraction2);
	friend Rational operator*(Rational fraction1, Rational fraction2);
	friend Rational operator/(Rational fraction1, Rational fraction2);
	friend bool		operator==(Rational fraction1, Rational fraction2);
	friend bool		operator<(Rational fraction1, Rational fraction2);
	friend bool		operator>(Rational fraction1, Rational fraction2);

private:
	int a, b;
};

Rational operator+(Rational fraction1, Rational fraction2) {	//перегрузка оператора +
	return Rational(fraction1.a * fraction2.b + fraction1.b * fraction2.a, fraction1.b * fraction2.b);
}
Rational operator-(Rational fraction1, Rational fraction2) {	//перегрузка оператора -
	return Rational(fraction1.a * fraction2.b - fraction1.b * fraction2.a, fraction1.b * fraction2.b);
}
Rational operator*(Rational fraction1, Rational fraction2) {	//перегрузка оператора *
	return Rational(fraction1.a * fraction2.a, fraction1.b * fraction2.b);
}
Rational operator/(Rational fraction1, Rational fraction2) {	//перегрузка оператора /
	return Rational(fraction1.a * fraction2.b, fraction1.b * fraction2.a);
}

bool operator==(Rational fraction1, Rational fraction2) {		//перегрузка оператора ==
	return (fraction1.a == fraction2.a)&& (fraction1.b == fraction2.b);
}
bool operator<(Rational fraction1, Rational fraction2) {		//перегрузка оператора <
	return ((fraction1.a * fraction2.b - fraction1.b * fraction2.a < 0) && (fraction1.b * fraction2.a > 0))
		 ||((fraction1.a * fraction2.b - fraction1.b * fraction2.a > 0) && (fraction1.b * fraction2.a < 0));
}
bool operator>(Rational fraction1, Rational fraction2) {		//перегрузка оператора >
	return ((fraction1.a * fraction2.b - fraction1.b * fraction2.a > 0) && (fraction1.b * fraction2.a > 0))
		|| ((fraction1.a * fraction2.b - fraction1.b * fraction2.a < 0) && (fraction1.b * fraction2.a < 0));
}

string Add(Rational fraction1, Rational fraction2) {	//сложение двух рациональных дробей 
	Rational fraction3 = fraction1 + fraction2;
	fraction3.reduce();
	string s = "(";
	s += to_string(fraction3.GetA());
	s += ",";
	s += to_string(fraction3.GetB());
	s += ")";
	return s;
}

string Sub(Rational fraction1, Rational fraction2) {	//разность двух рациональных дробей
	Rational fraction3 = fraction1 - fraction2;
	fraction3.reduce();
	string s = "(";
	s += to_string(fraction3.GetA());
	s += ",";
	s += to_string(fraction3.GetB());
	s += ")";
	return s;
}

string Mul(Rational fraction1, Rational fraction2) {	//умножение двух рациональных дробей
	Rational fraction3 = fraction1 * fraction2;
	fraction3.reduce();
	string s = "(";
	s += to_string(fraction3.GetA());
	s += ",";
	s += to_string(fraction3.GetB());
	s += ")";
	return s;
}

string Div(Rational fraction1, Rational fraction2) {	//деление двух рациональных дробей
	Rational fraction3 = fraction1 / fraction2;
	fraction3.reduce();
	string s = "(";
	s += to_string(fraction3.GetA());
	s += ",";
	s += to_string(fraction3.GetB());
	s += ")";
	return s;
}

string Com(Rational fraction1, Rational fraction2) {	//сравнение двух рациональных дробей
	if (fraction1 > fraction2) {
		return "the first fraction more than the second";
	}
	if (fraction1 < fraction2) {
		return "the second fraction more than the first";
	}
	if (fraction1 == fraction2) {
		return "fractions are equal";
	}
}

long double operator"" _rounding(long double d) {	//округление числа типа double до десятых
	return round(d*10)/10;
}

int main(){

	const string path = "data.txt";
	ifstream input(path);
	int a1, b1, a2, b2;

	long double d = 34.234556_rounding;

	if (input.is_open()) {		//ввод двух дробей в формате (a,b)
		input.ignore(1);
		try{
			if (!(input >> a1)) throw 1;
		}
		catch (...) { cerr << "ERROR INPUT"; return 1; }
		input.ignore(1);
		try {
			if ((!(input >> b1))|| (b1 == 0)) throw 1;
		}
		catch (...) { cerr << "ERROR INPUT"; return 1; }
		input.ignore(3);
		try {
			if (!(input >> a2)) throw 1;
		}
		catch (...) { cerr << "ERROR INPUT"; return 1; }
		input.ignore(1);
		try {
			if ((!(input >> b2))|| (b2 == 0)) throw 1;
		}
		catch (...) { cerr << "ERROR INPUT"; return 1; }
		input.ignore(1);

		Rational fraction1(a1, b1);
		Rational fraction2(a2, b2);

		cout << Add(fraction1, fraction2) << " - the sum of two fractions" << '\n';
		cout << Sub(fraction1, fraction2) << " - the difference of two fractions" << '\n';
		cout << Mul(fraction1, fraction2) << " - the multiplication of two fractions" << '\n';
		cout << Div(fraction1, fraction2) << " - the division of two fractions" << '\n';
		cout << Com(fraction1, fraction2) << " - the comparison of two fractions" << '\n';

		cout << "rounding for 34.234556: " << d;

	}
	else { cout << "ERROR"; }

	_getch();

	return 0;
}

