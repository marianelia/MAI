#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <chrono>
#include <gmpxx.h>

namespace NSuperAlg {

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
                data.push_back(n);
            else {
                for (; n; n /= BASE)
                    data.push_back(n % BASE);
            }
        }
        void Initialize(const std::string &str);

        friend std::istream& operator>>(std::istream &in, TSuperAlg &rhs);
        friend std::ostream& operator<<(std::ostream &out, const TSuperAlg &rhs);

        TSuperAlg operator+(const TSuperAlg &rhs);
        TSuperAlg operator-(const TSuperAlg &rhs);
        TSuperAlg operator*(const TSuperAlg &rhs) const;
        TSuperAlg operator/(const TSuperAlg &rhs);
        TSuperAlg Power(int r);
        bool operator<(const TSuperAlg &rhs) const;
        bool operator==(const TSuperAlg &rhs) const;

    private:
        void DeleteLeadingZeros();
        std::vector<int32_t> data;
    };

    using bigInt_t = TSuperAlg;


    void TSuperAlg::Initialize(const std::string & str) {
        int size = static_cast<int>(str.size());
        for (int i = size - 1; i >= 0; i = i - TSuperAlg::RADIX) {
            if (i < TSuperAlg::RADIX) {
                data.push_back(static_cast<int32_t>(atoll(str.substr(0, i + 1).c_str()))); 
            }
            else { 
                data.push_back(static_cast<int32_t>(atoll(str.substr(i - TSuperAlg::RADIX + 1, TSuperAlg::RADIX).c_str()))); 
            }
        }
        DeleteLeadingZeros();
    }

    TSuperAlg TSuperAlg::operator+(const TSuperAlg &rhs){
        TSuperAlg res;
        int32_t carry = 0;
        size_t n = std::max(rhs.data.size(), data.size());
        res.data.resize(n);
        for (size_t i = 0; i < n; ++i) {
            int32_t sum = carry;
            if (i < rhs.data.size()) {
                sum += rhs.data[i];
            }
            if (i < data.size()) {
                sum += data[i];
            }
            carry = sum / TSuperAlg::BASE;
            res.data[i] = sum % TSuperAlg::BASE;
        }
        if (carry != 0) {
            res.data.push_back(1);
        }
        res.DeleteLeadingZeros();
        return res;
    }

    TSuperAlg TSuperAlg::operator-(const TSuperAlg &rhs){
        TSuperAlg res;
        int32_t carry = 0;
        size_t n = std::max(rhs.data.size(), data.size());
        res.data.resize(n + 1, 0);
        for (size_t i = 0; i < n; ++i) {
            int32_t diff = data[i] - carry;
            if (i < rhs.data.size()) {
                diff -= rhs.data[i];
            }

            carry = 0;
            if (diff < 0) {
                carry = 1;
                diff += TSuperAlg::BASE;
            }
            res.data[i] = diff % TSuperAlg::BASE;
        }
        res.DeleteLeadingZeros();
        return res;
    }

    TSuperAlg TSuperAlg::operator*(const TSuperAlg & rhs) const {
        size_t n = data.size() * rhs.data.size();
        TSuperAlg res;
        res.data.resize(n + 1);

        int k = 0;
        int r = 0;
        for (size_t i = 0; i < data.size(); ++i){
            for (size_t j = 0; j < rhs.data.size(); ++j)
            {
                k = rhs.data[j] * data[i] + res.data[i + j];
                r = k / TSuperAlg::BASE;
                res.data[i + j + 1] = res.data[i + j + 1] + r;
                res.data[i + j] = k % TSuperAlg::BASE;
            }
        }
        res.DeleteLeadingZeros();
        return res;
    }

    TSuperAlg TSuperAlg::operator/(const TSuperAlg &rhs) {
        TSuperAlg res, cv = TSuperAlg(0);
        res.data.resize(data.size());

        for (int i = (int)data.size() - 1; i >= 0; --i) {
            cv.data.insert(cv.data.begin(), data[i]);
            if (!cv.data.back())
                cv.data.pop_back();
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
            res.data[i] = x;
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
        if (data.size() != rhs.data.size()) {
            return data.size() < rhs.data.size();
        }

        for (int32_t i = data.size() - 1; i >= 0; --i) {
            if (data[i] != rhs.data[i]) {
                return data[i] < rhs.data[i];
            }
        }
        return false;
    }

    bool TSuperAlg::operator==(const TSuperAlg &rhs) const {
        if (data.size() != rhs.data.size()) {
            return false;
        }

        for (int32_t i = data.size() - 1; i >= 0; --i) {
            if (data[i] != rhs.data[i]) {
                return false;
            }
        }
        return true;
    }

    void TSuperAlg::DeleteLeadingZeros(){
        while (!data.empty() && data.back() == 0) {
            data.pop_back(); 
        }
    }

    std::istream& operator>>(std::istream &in, TSuperAlg &rhs) {
        std::string str;
        in >> str;
        rhs.Initialize(str);
        return in;
    }

    std::ostream& operator<<(std::ostream &out, const TSuperAlg &rhs) {
        if (rhs.data.empty()) {
            out << "0";
            return out;
        }
        out << rhs.data.back();
        for (int i = rhs.data.size() - 2; i >= 0; --i) {
            out << std::setfill('0') << std::setw(TSuperAlg::RADIX) << rhs.data[i];
        }
        return out;
    }

} // namespace NSuperAlg


/*
int main(){
    std::string strNum1, strNum2;
    NSuperAlg::bigInt_t zero("0");
    NSuperAlg::bigInt_t one("1");
    char action;
    auto start = std::chrono::steady_clock::now();
    while (std::cin >> strNum1 >> strNum2 >> action) {
        NSuperAlg::bigInt_t num1(strNum1);
        NSuperAlg::bigInt_t num2(strNum2);
        if (action == '+') {
            NSuperAlg::bigInt_t res = num1 + num2;
            std::cout << res << "\n";
        }
        else if (action == '-') {
            if (num1 < num2) {
                std::cout << "Error\n";
                continue;
            }
            NSuperAlg::bigInt_t res = num1 - num2;
            std::cout << res << "\n";
        }
        else if (action == '*') {
            NSuperAlg::bigInt_t res = num1 * num2;
            std::cout << res << "\n";
        }
        else if (action == '/') {
            if (num2 == zero) {
                std::cout << "Error\n";
                continue;
            }
            NSuperAlg::bigInt_t res = num1 / num2;
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
                NSuperAlg::bigInt_t  res = num1.Power(std::stoi(strNum2));
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
    auto finish = std::chrono::steady_clock::now();
    auto dur1 = finish - start;

    return 0;
}*/

int main()
{
    uint64_t bigint_time = 0;
    uint64_t gmp_time = 0;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::string strNum1, strNum2;
    NSuperAlg::bigInt_t num1("100000");
    NSuperAlg::bigInt_t num3("4");
    NSuperAlg::bigInt_t num2("12912912927376274372");
    NSuperAlg::bigInt_t bigint_res;

    mpz_class gpm_num1;
    mpz_class gpm_num2;
    mpz_class gpm_num3;
    mpz_class gpm_res;
    gpm_num1 = "100000";
    gpm_num2 = "12912912927376274372";
    gpm_num3 = "4";
    //сумма
    int n_sum = 100000000;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < n_sum; ++i)
    {
        bigint_res = num1 + num2;
    }
    end = std::chrono::system_clock::now();
    bigint_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    start = std::chrono::system_clock::now();
    for (int i = 0; i < n_sum; ++i)
    {
        gpm_res = gpm_num1 + gpm_num2;
    }
    end = std::chrono::system_clock::now();
    gmp_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Bigint sum time: " << (double)bigint_time / 1000000000 << " seconds\n";
    std::cout << "Gmp sum time: " << (double)gmp_time / 1000000000 << " seconds\n";
    //разность
    int n_sub = 100000000;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < n_sub; ++i)
    {
        bigint_res = num2 - num1;
    }
    end = std::chrono::system_clock::now();
    bigint_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    start = std::chrono::system_clock::now();
    for (int i = 0; i < n_sub; ++i)
    {
        gpm_res = gpm_num2 - gpm_num1;
    }
    end = std::chrono::system_clock::now();
    gmp_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "\nBigint sub time: " << (double)bigint_time / 1000000000 << " seconds\n";
    std::cout << "Gmp sub time: " << (double)gmp_time / 1000000000 << " seconds\n";
    //умножение
    int n_mult = 100000000;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < n_mult; ++i)
    {
        bigint_res = num2 * num1;
    }
    end = std::chrono::system_clock::now();
    bigint_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    start = std::chrono::system_clock::now();
    for (int i = 0; i < n_mult; ++i)
    {
        gpm_res = gpm_num2 * gpm_num1;
    }
    end = std::chrono::system_clock::now();
    gmp_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "\nBigint mult time: " << (double)bigint_time / 1000000000 << " seconds\n";
    std::cout << "Gmp mult time: " << (double)gmp_time / 1000000000 << " seconds\n";
    return 0;
}
