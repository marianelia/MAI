#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

namespace super_alg {

    class TSuperAlg {
    public:
        static const int BASE = 10000;
        static const int RADIX = 4;

        TSuperAlg() = default;
        TSuperAlg(std::string const &str) {
            this->Initialize(str);
        }
        TSuperAlg(int n){
            if (n < BASE)
                _data.push_back(n);
            else {
                for (; n; n /= BASE)
                    _data.push_back(n % BASE);
            }
        }
        void Initialize(const std::string &str);

        friend std::istream& operator>>(std::istream &in, TSuperAlg &rhs);
        friend std::ostream& operator<<(std::ostream &out, const TSuperAlg &rhs);

        TSuperAlg operator+(const TSuperAlg &rhs);
        TSuperAlg operator-(const TSuperAlg &rhs);
        TSuperAlg operator*(const TSuperAlg &rhs) const;
        TSuperAlg operator/(const TSuperAlg &rhs);
        //TSuperAlg const operator^(TSuperAlg &rhs);
        TSuperAlg Power(int r);
        bool operator<(const TSuperAlg &rhs) const;
        bool operator==(const TSuperAlg &rhs) const;

    private:
        void DeleteLeadingZeros();
        std::vector<int32_t> _data;
    };

    //using bigInt_t = uint64_t;
    using bigInt_t = TSuperAlg;


    void TSuperAlg::Initialize(const std::string & str) {
        int size = static_cast<int>(str.size());
        for (int i = size - 1; i >= 0; i = i - TSuperAlg::RADIX) {
            if (i < TSuperAlg::RADIX) {
                _data.push_back(static_cast<int32_t>(atoll(str.substr(0, i + 1).c_str()))); 
            }
            else { 
                _data.push_back(static_cast<int32_t>(atoll(str.substr(i - TSuperAlg::RADIX + 1, TSuperAlg::RADIX).c_str()))); 
            }
        }
        DeleteLeadingZeros();
    }

    // 159
    //  44
    // ---
    // 203
    TSuperAlg TSuperAlg::operator+(const TSuperAlg &rhs){
        TSuperAlg res;
        int32_t carry = 0;
        size_t n = std::max(rhs._data.size(), _data.size());
        res._data.resize(n);
        for (size_t i = 0; i < n; ++i) {
            int32_t sum = carry;
            if (i < rhs._data.size()) {
                sum += rhs._data[i];
            }
            if (i < _data.size()) {
                sum += _data[i];
            }
            carry = sum / TSuperAlg::BASE;
            res._data[i] = sum % TSuperAlg::BASE;
        }
        if (carry != 0) {
            res._data.push_back(1);
        }
        res.DeleteLeadingZeros();
        return res;
    }

    TSuperAlg TSuperAlg::operator-(const TSuperAlg &rhs){
        TSuperAlg res;
        int32_t carry = 0;
        size_t n = std::max(rhs._data.size(), _data.size());
        res._data.resize(n + 1, 0);
        for (size_t i = 0; i < n; ++i) {
            int32_t diff = _data[i] - carry;
            if (i < rhs._data.size()) {
                diff -= rhs._data[i];
            }

            carry = 0;
            if (diff < 0) {
                carry = 1;
                diff += TSuperAlg::BASE;
            }
            res._data[i] = diff % TSuperAlg::BASE;
        }
        res.DeleteLeadingZeros();
        return res;
    }

    TSuperAlg TSuperAlg::operator*(const TSuperAlg & rhs) const {
        size_t n = _data.size() * rhs._data.size();
        TSuperAlg res;
        res._data.resize(n + 1);

        int k = 0;
        int r = 0;
        for (size_t i = 0; i < _data.size(); ++i){
            for (size_t j = 0; j < rhs._data.size(); ++j)
            {
                k = rhs._data[j] * _data[i] + res._data[i + j];
                r = k / TSuperAlg::BASE;
                res._data[i + j + 1] = res._data[i + j + 1] + r;
                res._data[i + j] = k % TSuperAlg::BASE;
            }
        }
        res.DeleteLeadingZeros();
        return res;
    }

    TSuperAlg TSuperAlg::operator/(const TSuperAlg &rhs) {
        TSuperAlg res, cv = TSuperAlg(0);
        res._data.resize(_data.size());

        for (int i = (int)_data.size() - 1; i >= 0; --i) {
            cv._data.insert(cv._data.begin(), _data[i]);
            if (!cv._data.back())
                cv._data.pop_back();
            int x = 0, l = 0, r = BASE;
            while (l <= r) {
                int m = (l + r) / 2;
                TSuperAlg cur = rhs * TSuperAlg(std::to_string(m));
                if ((cur < cv)||(cur == cv)) {
                    x = m;
                    l = m + 1;
                }
                else {
                    r = m - 1;
                }
            }
            res._data[i] = x;
            cv = cv - rhs * TSuperAlg(std::to_string(x));
        }
        res.DeleteLeadingZeros();
        return res;

    }

    TSuperAlg TSuperAlg::Power(int p){
        TSuperAlg res(1);
        while (p) {
            if (p % 2 == 1)
                res = res * (*this);
            (*this) = (*this) * (*this);
            p /= 2;
        }
        return res;
    }

    bool TSuperAlg::operator<(const TSuperAlg &rhs) const {
        if (_data.size() != rhs._data.size()) {
            return _data.size() < rhs._data.size();
        }

        for (int32_t i = _data.size() - 1; i >= 0; --i) {
            if (_data[i] != rhs._data[i]) {
                return _data[i] < rhs._data[i];
            }
        }
        return false;
    }

    bool TSuperAlg::operator==(const TSuperAlg &rhs) const {
        if (_data.size() != rhs._data.size()) {
            return false;
        }

        for (int32_t i = _data.size() - 1; i >= 0; --i) {
            if (_data[i] != rhs._data[i]) {
                return false;
            }
        }
        return true;
    }

    void TSuperAlg::DeleteLeadingZeros(){
        while (!_data.empty() && _data.back() == 0) {
            _data.pop_back(); 
        }
    }

    std::istream& operator>>(std::istream &in, TSuperAlg &rhs) {
        std::string str;
        in >> str;
        rhs.Initialize(str);
        return in;
    }

    std::ostream& operator<<(std::ostream &out, const TSuperAlg &rhs) {
        if (rhs._data.empty()) {
            out << "0";
            return out;
        }
        //  0   1
        // [1] [2]  -> 2 01
        out << rhs._data.back();
        for (int i = rhs._data.size() - 2; i >= 0; --i) {
            out << std::setfill('0') << std::setw(TSuperAlg::RADIX) << rhs._data[i];
        }
        return out;
    }

} // namespace super_alg



int main(){
    std::string strNum1, strNum2;
    super_alg::bigInt_t zero("0");
    super_alg::bigInt_t one("1");
    char action;
    while (std::cin >> strNum1 >> strNum2 >> action) {
        super_alg::bigInt_t num1(strNum1);
        super_alg::bigInt_t num2(strNum2);
        if (action == '+') {
            super_alg::bigInt_t res = num1 + num2;
            std::cout << res << "\n";
        }
        else if (action == '-') {
            if (num1 < num2) {
                std::cout << "Error\n";
                continue;
            }
            super_alg::bigInt_t res = num1 - num2;
            std::cout << res << "\n";
        }
        else if (action == '*') {
            super_alg::bigInt_t res = num1 * num2;
            std::cout << res << "\n";
        }
        else if (action == '/') {
            if (num2 == zero) {
                std::cout << "Error\n";
                continue;
            }
            super_alg::bigInt_t res = num1 / num2;
            std::cout << res << "\n";
        }
        else if (action == '^') {
            if (num1 == zero) {
                if (num2 == zero) {
                    std::cout << "Error\n";
                    continue;
                }
                else {
                    std::cout << "0" << "\n";
                }
            }
            else if (num1 == one) {
                std::cout << "1" << "\n";
            }
            else {
                super_alg::bigInt_t  res = num1.Power(std::stoi(strNum2));
                std::cout << res << "\n";
            }
        }

        else if (action == '<') {
            std::cout << ((num1 < num2) ? "true" : "false") << "\n";
        }
        else if (action == '>') {
            std::cout << ((num2 < num1) ? "true" : "false") << "\n";
        }
        else if (action == '=') {
            std::cout << ((num1 == num2) ? "true" : "false") << "\n";
        }
        else { std::cout << "Error\n"; }
    }
    return 0;
}
