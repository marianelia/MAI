
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <chrono>
#include <algorithm>

std::vector<size_t> CountPrefix(std::vector<std::string>& p) {
    size_t n = p.size();
    std::vector<size_t> sp(n);
    sp[0] = 0;
    size_t v = 0;
    for (size_t i = 1; i < n; ++i) {
        while ((v != 0) && (p[i] != p[v])) {
            v = sp[v - 1];
        }
        if (p[i] == p[v]) {
            ++v;
        }
        sp[i] = v;
    }
    sp.push_back(0);
    return sp;
}

bool EqualPatternWord(std::vector<std::string>& p, std::string& word, size_t i) {
    if (i >= p.size()) { return false; }
    return p[i] == word;
}

size_t KmpSearch(std::vector<std::vector<std::string>>& text, std::vector<std::string>& p) {
    std::vector<size_t> sp = CountPrefix(p);
    size_t countLine;
    int countWord;
    size_t oc = 0;
    size_t j = 0;
    for (size_t line = 0; line < text.size(); ++line) { //по строкам
        for (size_t word = 0; word < text[line].size(); ++word) { //по словам
            while ((j > 0) && (!EqualPatternWord(p, text[line][word], j))) {
                j = sp[j - 1];
            }
            if (p[j] == text[line][word]) {
                j++;
            }
            if (j == p.size()) {
                countLine = line;
                countWord = word - p.size() + 1;
                while (countWord < 0) {
                    --countLine;
                    countWord += text[countLine].size();
                }
                std::cout << countLine + 1 << ", " << countWord + 1 << std::endl;
            }
        }
    }
    return oc;
}

bool isSpace(char c) {
    return ((c == ' ') || (c == '\t') || (c == '\n'));
}

enum TState {
    str,
    line
};

using timeDuration = std::chrono::nanoseconds;

std::string RandomString(size_t maxLength) {
    size_t length = rand() % maxLength;
    auto RandChar = []() -> char {
        const char charSet[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const size_t maxIdx = (sizeof(charSet) - 1);
        return charSet[rand() % maxIdx];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, RandChar);
    return str;
}

void RandomLine(size_t maxCount, std::vector<std::string>& strs) {
    std::vector<std::string> result;
    const size_t maxStrLen = 17;
    size_t count = rand() % maxCount;
    for (size_t i = 0; i < count; ++i) {
        strs.push_back(RandomString(maxStrLen));
    }
}

//int main(int argc, const char * argv[]) {
//    //std::srand(static_cast<unsigned int>(std::time(0)));
//
//    std::cout << "Enter amount of lines: ";
//    size_t linesCnt;
//    std::cin >> linesCnt;
//
//    std::string patternStr = "one two three four4 5fiv5e 9120asD one two three";
//    std::vector<std::string> pattern = {
//        std::string("one"), std::string("two"), std::string("three"), std::string("four4"),
//        std::string("5fiv5e"), std::string("9120asD"), std::string("one"),
//        std::string("two"), std::string("three")
//    };
//
//    std::vector<std::vector<std::string>> text;
//    std::string textStr;
//
//    const int maxLineLen = 20;
//    const int patternFrequency = 5;
//    int occurrances = 0;
//    for (size_t i = 0; i < linesCnt; ++i) {
//        std::vector<std::string> line;
//        RandomLine(maxLineLen, line);
//
//        if ((rand() % patternFrequency) == 0) {
//            ++occurrances;
//            size_t idx = 0;
//            if (line.size() != 0) {
//                idx = rand() % line.size();
//            }
//            auto it = std::begin(line);
//            while (idx > 0) {
//                ++it;
//                --idx;
//            }
//            line.insert(it, std::begin(pattern), std::end(pattern));
//        }
//
//        text.push_back(line);
//        for (std::string str : line) {
//            textStr += (str + " ");
//        }
//    }
//
//    std::cout << "TOTAL OCCURRANCES = " << occurrances << std::endl;
//
//    std::chrono::time_point<std::chrono::system_clock> start, end;
//    int64_t findTime;
//    int64_t kmpTime;
//
//    start = std::chrono::system_clock::now();
//    size_t pos = textStr.find(patternStr, 0);
//    size_t findRes = 0;
//    while (pos < textStr.size()) {
//        ++findRes;
//        pos = textStr.find(patternStr, pos + 1);
//    }
//    end = std::chrono::system_clock::now();
//    findTime = std::chrono::duration_cast<timeDuration>(end - start).count();
//
//    start = std::chrono::system_clock::now();
//    size_t kmpRes = KmpSearch(text, pattern);
//    end = std::chrono::system_clock::now();
//    kmpTime = std::chrono::duration_cast<timeDuration>(end - start).count();
//
//    std::cout << "kmp search time: " << kmpTime << std::endl;
//
//    std::cout << "string find time: " << findTime << std::endl;
//
//    return 0;
//}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::vector<std::string>> text;
    std::vector<std::string> pattern;
    std::vector<std::string> curLine;
    std::string curStr;
    int flagFirstLine = 1;
    TState state = line;
    char letter = getchar();
    while (letter != EOF) {
        switch (state) {
        case line:
            if (!isSpace(letter)) {
                state = str;
                break;
            }
            if (letter == '\n') {
                if (flagFirstLine == 1) {
                    pattern = std::move(curLine); 
                    flagFirstLine = 0;
                }
                else { text.push_back(std::move(curLine)); }
                curLine.clear();
            }
            letter = getchar();
            break;

        case str:
            if (isSpace(letter)) {
                curLine.push_back(std::move(curStr));
                curStr.clear();
                state = line;
                break;
            }
            curStr.push_back(std::tolower(letter));
            letter = getchar();
            break;
        }
    }
    if (pattern.size() == 0 || text.size() == 0) { return 0; }
    KmpSearch(text, pattern);
    return 0;
}

