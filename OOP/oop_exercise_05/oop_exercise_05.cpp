// Лагуткина Мария Сергеевна, М8О-206Б-19
//Вариант 14: пятиугольник, список

#include <iostream>
#include <cmath>
#include <array>
#include <algorithm>
#include <iterator>
#include <memory>
#include <cmath>
#include <string>

using namespace std;

const double PI = 3.1415;

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
vertex_t<T> operator+(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {
	return { lhs.first + rhs.first, lhs.second + rhs.second };
}

template <class T>
vertex_t<T> operator-(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {
	return { lhs.first - rhs.first, lhs.second - rhs.second };
}

template <class T>
vertex_t<T> operator/(const vertex_t<T>& lhs, const double& rhs) {
	return { lhs.first / rhs, lhs.second / rhs };
}

template <class T>
double distance(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {		//расстояние между двумя точками
	return sqrt((lhs.first - rhs.first) * (lhs.first - rhs.first)
		+ (lhs.second - rhs.second) * (lhs.second - rhs.second));
}

template <class T>
class Pentagon {
public:
	vertex_t<T> a, center;
};

template <class T>
vertex_t<T> polar_to_vertex(double ro, double fi) {	//переход из полярной системы координат
	return { ro * cos(fi), ro * sin(fi) };
}

template <class T>
array<vertex_t<T>, 5> find_pentagon_vertexes(const Pentagon<T>& pt) {	//определение всех вершин пятиугольника
	vertex_t<T> ast, b, c, d, e;
	ast = pt.a - pt.center;
	double ro = distance(pt.a, pt.center);

	double fi = 0;
	if (ast.second >= 0) {
		if (ast.first != 0) {
			fi = atan(ast.second / ast.first);
		}
		else {
			fi = PI / 2;
		}
	}
	else {
		if (ast.first != 0) {
			fi = atan(ast.second / ast.first) + PI / 2;
		}
		else {
			fi = 3 * PI / 2;
		}
	}
	fi -= 2 * PI / 5;
	b = polar_to_vertex<T>(ro, fi);
	fi -= 2 * PI / 5;
	c = polar_to_vertex<T>(ro, fi);
	fi -= 2 * PI / 5;
	d = polar_to_vertex<T>(ro, fi);
	fi -= 2 * PI / 5;
	e = polar_to_vertex<T>(ro, fi);
	const auto& center_of_figure = pt.center;
	return { pt.a, b + center_of_figure, c + center_of_figure, d + center_of_figure, e + center_of_figure };
}

template <class T>
double area(array<vertex_t<T>, 5> &a) {
	return abs(a[0].first*a[1].second + a[1].first*a[2].second 
			+ a[2].first*a[3].second + a[3].first*a[4].second 
			+ a[4].first*a[0].second
			- a[1].first*a[0].second - a[2].first*a[1].second
			- a[3].first*a[2].second - a[4].first*a[3].second
			- a[0].first*a[4].second)/2;
}

template <class T>
istream& operator>> (istream& input, Pentagon<T>& p) {
	input >> p.a >> p.center;
	return input;
}

template <class T>
ostream& operator<< (ostream& output, array<vertex_t<T>,5> &a) {
	output << a[0] << ' ' << a[1] << ' ' << a[2] <<' ' << a[3] << ' ' << a[4] << '\n';
	return output;
}

template <class T>
class List {		//контейнер список
private:
	class List_el;
	unique_ptr<List_el> first;
	List_el *tail = nullptr;
	size_t size = 0;
	public:
		class Forward_iterator{
		public:
			using value_type = T;
			using reference = value_type & ;
			using pointer = value_type * ;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			explicit Forward_iterator(List_el *ptr) {
				it_ptr = ptr;
			}
			T &operator*() {
				return this->it_ptr->value;
			}
			Forward_iterator &operator++() {
				if(it_ptr == nullptr)
					throw std::length_error("out of list");
				*this = it_ptr->next();
				return *this;
			}
			Forward_iterator operator++(int) {
				Forward_iterator old = *this;
				++*this;
				return old;
			}
			bool operator==(const Forward_iterator &other) const {
				return it_ptr == other.it_ptr;
			}
			bool operator!=(const Forward_iterator &other) const {
				return it_ptr != other.it_ptr;
			}
		private:
			List_el *it_ptr;
			friend List;
		};

		Forward_iterator begin() {
			return Forward_iterator(first.get());
		}
		Forward_iterator end() {
			return Forward_iterator(nullptr);
		}
		void erase(size_t index) {					//удаление элемента по индексу и списка
			Forward_iterator it = this->begin();
			for (size_t i = 0; i < index; ++i){
				++it;
			}
			Forward_iterator begin = this->begin(), end = this->end();
			if (it == end) { throw length_error("out of border"); }
			if (it == begin){										//удаление из начала списка
				if (size == 0) { throw length_error("can't pop from empty list"); }
				if (size == 1){
					first = nullptr;
					tail = nullptr;
					--size;
					return;
				}
				unique_ptr<List_el> tmp = std::move(first->next_el);
				first = std::move(tmp);
				first->prev_el = nullptr;
				--size;
				return;
			}
			if (it.it_ptr == tail){			//удаление из конца списка
				if (size == 0) { throw length_error("can't pop from empty list"); }
				if (tail->prev_el){
					List_el *tmp = tail->prev_el;
					tail->prev_el->next_el = nullptr;
					tail = tmp;
				}else{
					first = nullptr;
					tail = nullptr;
				}
				--size;
				return;
			}
			if (it.it_ptr == nullptr) { throw std::length_error("out of broder"); }
	
			auto tmp = it.it_ptr->prev_el;
			unique_ptr<List_el> temp1 = move(it.it_ptr->next_el);
			it.it_ptr = it.it_ptr->prev_el;
			it.it_ptr->next_el = move(temp1);
			it.it_ptr->next_el->prev_el = tmp;
			--size;
		}
		void insert(size_t index, T &value) {			//вставка элемента в список
			Forward_iterator it = this->begin();
			if (index >= this->size) { it = this->end(); }
			else {
				for (size_t i = 0; i < index; ++i) {
					++it;
				}
			}
			unique_ptr<List_el> tmp = make_unique<List_el>(value);
			if (it == this->begin()){	//вставка в начало списка
				size++;
				unique_ptr<List_el> tmp = move(first);
				first = make_unique<List_el>(value);
				first->next_el = move(tmp);
				if (first->next_el != nullptr) {
					first->next_el->prev_el = first.get();
				}
				if (size == 1){
					tail = first.get();
				}
				if (size == 2){
					tail = first->next_el.get();
				}
				return;
			}
			if (it.it_ptr == nullptr){	//вставка в конец списка
				if (!size){
					first = make_unique<List_el>(value);
					tail = first.get();
					size++;
					return;
				}
				tail->next_el = make_unique<List_el>(value);
				List_el *tmp = tail;
				tail = tail->next_el.get();
				tail->prev_el = tmp;
				size++;
				return;
			}
			tmp->prev_el = it.it_ptr->prev_el;
			it.it_ptr->prev_el = tmp.get();
			tmp->next_el = std::move(tmp->prev_el->next_el);
			tmp->prev_el->next_el = std::move(tmp);
			size++;
		}
		List &operator=(List &other) {
			size = other.size;
			first = std::move(other.first);
		}

		T &operator[](size_t index) {
			if (index < 0 || index >= size){
				throw std::out_of_range("out of list");
			}
			Forward_iterator it = this->begin();
			for (size_t i = 0; i < index; i++){
				it++;
			}
			return *it;
		}
private:
	class List_el {
	public:
		T value;
		unique_ptr<List_el> next_el;
		List_el *prev_el = nullptr;
		List_el() = default;
		List_el(const T &new_value) : value(new_value) {}
		Forward_iterator next() {
			return Forward_iterator(this->next_el.get());
		}
	};
};

namespace Interface {
	void help() {
		cout << 
			"Usage: <element> is first vertex and center in pentagon.\n"
			"insert <index><element> - insertion new element of list\n"
			"erase <element>         - delete element of list\n"
			"print                   - print vertex all pentagons\n"
			"area <number>           - displaying the number of objects with an area less than the specified one\n"
			"help                    - print usage\n"
			"quit                    - quit out program\n";
	}

}

int main() {
	string input_s;
	int input_n;
	List<Pentagon<double>> list;
	Interface::help();
	while (true) {
		cin >> input_s;
		if (input_s == "insert") {
			cin >> input_n;
			Pentagon<double> p;
			cin >> p;
			list.insert(input_n, p);
			cout << "Ok\n";
		}
		else if (input_s == "erase") {
			cin >> input_n;
			try {
				list.erase(input_n);
				cout << "Ok\n";
			}
			catch (out_of_range) { cerr << "BORDER OVERLAY" << '\n'; }
			catch(length_error) { cerr << "BORDER OVERLAY" << '\n'; }
		}
		else if (input_s == "print") {
			for_each(list.begin(), list.end(), [](Pentagon<double> &P) {
				auto a = find_pentagon_vertexes(P);
				cout << a;
			}
			);
			cout << "Ok\n";
		}
		else if (input_s == "area") {
			cin >> input_n;
			cout << count_if(list.begin(), list.end(), [=](Pentagon<double> &P) {
				auto a = find_pentagon_vertexes(P);
				return area(a) < input_n; 
			}) << '\n';
			cout << "Ok\n";
		}
		else if (input_s == "help") {
			Interface::help();
		}
		else if (input_s == "quit") {
			return 0;
		}
		else { cout << "INCORECT INPUT\n"; }
	}
	return 0;
}
