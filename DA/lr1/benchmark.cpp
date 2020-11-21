#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>

using namespace std;

template <typename T, size_t size = 0>
class TArray {
protected:
    T array_[size];
public:
    TArray() {}
    TArray(size_t defValue) {
        for (auto& i : array_) {
            i = defValue;
        }
    }
    const size_t Size() {
        return size;
    }

    T* begin() {
        return &array_[0];
    }

    T* end() {
        return &array_[size];
    }

    const T& operator[](size_t index) const {
        return array_[index];
    }
    T& operator[](size_t index) {
        return array_[index];
    }
};

template <typename T>
class TVector {
public:
    TVector() : buf(nullptr), size(0), capacity(0) {}
    TVector(size_t newSize) {
        if (newSize == 0) {
            return;
        }
        size = newSize;
        capacity = newSize;
        buf = new T[size];
    }

    ~TVector() {
        delete[] buf;
        size = 0;
        capacity = 0;
        buf = nullptr;
    }

    void PushBack(T& value) {
        if (size < capacity) {
            buf[size++] = value;
            return;
        }
        size_t newCapacity = 1;
        if (capacity > 0) {
            newCapacity = capacity * 2;
        }
        capacity = newCapacity;
        T* newBuf = new T[capacity];
        memcpy(newBuf, buf, sizeof(T)*size);
        delete[] buf;
        buf = newBuf;
        buf[size++] = value;
        return;
    }

    const size_t Size() {
        return size;
    }

    T* begin() const {
        return buf;
    }

    T* end() const {
        if (buf) {
            return buf + size;
        }
        return nullptr;
    }

    T& operator[](size_t i) {
        return buf[i];
    }

    T& Get(size_t i) {
        return  this->buf[i];
    }

    void Set(size_t i, T& pair) {
        this->buf[i] = pair;
    }

    T* GetBuf() {
        return  this->buf;
    }

    void ReplaceBuf(T* newBuf) {
        memcpy(buf, newBuf, sizeof(T)*size);
    }

private:
    T *buf;
    size_t size;
    size_t capacity;
};

template <typename K, typename V>
struct TPair {
    K key;
    V value;
};

bool operator<(const TArray<char, 32> & lhs, const TArray<char, 32>& rhs) {
    int i = 0;
    while (lhs[i] == rhs[i]) {
        if (i == 31) {
            break;
        }
        i++;
    }
    return lhs[i] < rhs[i];
}

template <typename K, typename V>
bool operator<(const TPair<K, V>& lhs, const TPair<K, V>& rhs) {
    return lhs.key < rhs.key;
}

template <typename K, typename V>
istream& operator >> (istream& input, TPair<K, V>& pair) {
    for (int i = 0; i < pair.key.Size(); ++i) {
        input >> pair.key[i];
    }
    input >> pair.value;
    return input;
}

template <typename K, typename V>
ostream& operator<< (ostream& output, TPair<K, V>& pair) {
    for (int i = 0; i < pair.key.Size(); ++i) {
        output << pair.key[i];
    }
    output << '\t' << pair.value;
    return output;
}

template <typename K, typename V>
void CountingSort(int i, TVector<TPair<K, V>>& v) {
    TVector <TPair< TArray<char, 32>, uint64_t>> res(v.Size());
    int count[16];
    for (int j = 0; j < 16; ++j) {
        count[j] = 0;
    }
    for (int j = 0; j < v.Size(); ++j) {
        if (v[j].key[i] - '0' - 49 >= 0) {
            count[v[j].key[i] - '0' - 39]++;
        }
        else {
            count[v[j].key[i] - '0']++;
        }
    }
    for (int j = 1; j < 16; ++j) {
        count[j] += count[j - 1];
    }
    for (int j = v.Size() - 1; j >= 0; --j) {
        if (v[j].key[i] - '0' - 49 >= 0) {
            count[v[j].key[i] - '0' - 39]--;
            res.Set(count[v[j].key[i] - '0' - 39], v[j]);
        }
        else {
            count[v[j].key[i] - '0']--;
            res.Set(count[v[j].key[i] - '0'], v[j]);
        }
    }
    v.ReplaceBuf(res.GetBuf());
}

template <typename K, typename V>
void RadixSort(TVector<TPair<K, V>>& v) {
    for (int i = 31; i >= 0; --i) {
        CountingSort(i, v);
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TVector<TPair<TArray<char, 32>, uint64_t>> v;
    TPair<TArray<char, 32>, uint64_t> pair;
    pair.value = 0;

    auto start = chrono::steady_clock::now();

    while (cin >> pair) {
        v.PushBack(pair);
    }

    auto finish = chrono::steady_clock::now();
    auto dur1 = finish - start;

    start = chrono::steady_clock::now();
    stable_sort(v.begin(), v.end());
    //RadixSort(v);

    finish = chrono::steady_clock::now();
    auto dur2 = finish - start;

    start = chrono::steady_clock::now();

    for (size_t i = 0; i < v.Size(); ++i) {
        cout << v[i] << '\n';
    }

    finish = chrono::steady_clock::now();
    auto dur3 = finish - start;

    cerr << "input " << chrono::duration_cast<chrono::milliseconds>(dur1).count() << " ms" << endl;
    cerr << "sort " << chrono::duration_cast<chrono::milliseconds>(dur2).count() << " ms" << endl;
    cerr << "output " << chrono::duration_cast<chrono::milliseconds>(dur3).count() << " ms" << endl;
    return 0;
}