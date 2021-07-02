#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <bitset>
#include <fstream>
#include <algorithm>

////////////////////////////////////// Trie /////////////////////////////////////////

class TrieNode {
private:
    std::map<uint8_t, TrieNode*> path;
    uint32_t code;

public:
    friend class Trie;
    TrieNode( uint32_t num):code(num) {}
    void Clear();
    virtual ~TrieNode();
    
};

class Trie {
private:
    TrieNode* currentNode;
    std::vector<TrieNode*> path;

public:
    Trie() :
        path(0xff + 1), //255
        maxCode(1),
        nextIncrease(2),
        codeLen(1),
        currentNode(nullptr) {
        uint8_t sym = 0x0;
        for (uint32_t i = 0; i <= 0xff; ++sym, ++i) {  //conversion from char to string
            path[sym] = new TrieNode(maxCode);
            if (maxCode == nextIncrease) {
                nextIncrease *= 2;
                ++codeLen;
            }
            ++maxCode;
        }
    }

    uint32_t maxCode;     //next code to give, used in calculating of codeLen
    uint32_t codeLen;     //current lenght of code in bits
    uint32_t nextIncrease;//next value of maxCode that will increase codeLen

    void Clear();
    uint32_t GetCode(std::string& str);
    TrieNode* CheckWord(uint8_t sym);
    void AddWord(std::string & str);
    virtual ~Trie();

};

void TrieNode::Clear() {
    for (std::pair<uint8_t, TrieNode*> elem : path){
        delete elem.second;
    }
        path.clear();
}

TrieNode::~TrieNode() {
    for (std::pair<uint8_t, TrieNode*> elem : path)
        delete elem.second;
}

void Trie::Clear() {
    //root array stays in place
    maxCode = 0xff + 2;
    nextIncrease = 512;
    codeLen = 9;
    currentNode = nullptr;
    for (TrieNode* node : path){
        node->Clear();
    }
}

    uint32_t Trie::GetCode(std::string& str) {
        TrieNode* currNode = path[(uint8_t)str[0]];
        for (int i = 1; i < str.length(); ++i)  {
            currNode = currNode->path[(uint8_t)str[i]];
        }
        return currNode->code;
    }

    TrieNode* Trie::CheckWord(uint8_t sym) {
        if (currentNode) {
            if (currentNode->path.count(sym)) {
                currentNode = currentNode->path[sym];
            } else {
                currentNode = nullptr;
            }
        } else {
            currentNode = this->path[sym];
        }
        return currentNode;
    }

    void Trie::AddWord(std::string & str) {
        TrieNode* currNode = path[(uint8_t)str[0]];
        for (int i = 1; i < str.length() - 1; ++i)  {
            currNode = currNode->path[(uint8_t)str[i]];
        }
        currNode->path[(uint8_t)str[str.length() - 1]] = new TrieNode(maxCode);
        if (maxCode == nextIncrease) {
            nextIncrease *= 2;
            ++codeLen;
        }
        ++maxCode;
    }
    Trie::~Trie() {
        for (TrieNode* node : path){
            delete node;
        }
    }

/////////////////////////////////////// LZW //////////////////////////////////

class LZW{
private:
    uint64_t buffer = 0;
    uint32_t bufferCount = 0;
    void WriteBits(std::ostream& file);
    void AddToBuffer(uint32_t code, uint32_t len, std::ostream& file);

    void ReadBits(uint32_t len,std::istream& file);
    void Flush();
    bool Empty(std::istream& file);
    uint32_t GetBits(uint32_t len, std::istream& file);
    int32_t GetCodeLen(const uint32_t maxCode);

    friend Trie;

public:
    void EncodeLZW(std::istream& inputData, std::ostream& outputData, uint32_t maxCodeLen);
    int DecodeLZW(std::istream& encodedData, std::ostream& decodedData);
};

void LZW::WriteBits(std::ostream& file) {
    uint8_t byte = 0;
    while (bufferCount >= 8) {      //8 bits from begin of buffer
        bufferCount -= 8;
        byte = (uint8_t) (buffer >> (bufferCount));     //delete first 8 bits stored
        if (bufferCount == 0) {
            buffer = 0;
        }
        else {
            buffer = buffer << (64 - bufferCount);
            buffer = buffer >> (64 - bufferCount);
        }
        file.write((char*)&byte, 1);
    }
}

void LZW::AddToBuffer(uint32_t code, uint32_t len, std::ostream& file) {
    buffer = (buffer << len) | (uint64_t) code;
    bufferCount += len;
    if (bufferCount >= 24)
        WriteBits(file);
}
   
void LZW::ReadBits(uint32_t len, std::istream& file) {
    uint8_t byte = 0;
    while (bufferCount < len) {
        byte = 0;
        file.read((char*)&byte, 1);
        buffer = (buffer << 8) | (uint64_t)(byte);
        bufferCount += 8;
        file.peek();            //to trigger eof
        if (file.eof()) {
            return;
        }
    }
}

void LZW::Flush() {
    buffer = 0;
    bufferCount = 0;
}

bool LZW::Empty(std::istream& file) {
    file.peek();
    return (bufferCount == 0) && file.eof();
}

uint32_t LZW::GetBits(uint32_t len, std::istream& file) {
    uint32_t symbol = 0;
    if (bufferCount < len){
        ReadBits(len, file);
    }
    if (bufferCount < len) {
        //input data ended
        //return what have
        uint32_t a = buffer;
        file.peek();
        buffer = 0;
        bufferCount = 0;
        return a;
    }
    //get len bits from begin of bufferCount
    bufferCount -= len;
    symbol = (uint32_t)(buffer >> bufferCount);
    //delete first len bits stored
    if (bufferCount == 0) {
        symbol = buffer;
        buffer = 0;
    } else {
        buffer = buffer << (64 - bufferCount);
        buffer = buffer >> (64 - bufferCount);
    }
    return symbol;
}

int32_t LZW::GetCodeLen(const uint32_t maxCode) {
    return (uint32_t)ceil(log2(maxCode));
}

void LZW::EncodeLZW(std::istream& inputData, std::ostream& outputData, uint32_t maxCodeLen) {
    Trie trie;
    uint32_t exitCode = 0;
    std::string prev;
    std::string full;
    uint8_t curr;
    //write in file maxCodeLen
    AddToBuffer(maxCodeLen, 32, outputData);
    inputData.peek();       //check for eof
    while (!inputData.eof()) {
        inputData.read((char*)&curr, 1);    //read symbol
        full += curr;
        if (trie.CheckWord(curr)) {
            prev += curr;
        } else {
            AddToBuffer(trie.GetCode(prev), trie.codeLen, outputData);
            trie.AddWord(full);
            if (trie.codeLen > maxCodeLen) {    //reset the dictionary
                trie.Clear();
            }
            prev = curr;
            full = curr;
            trie.CheckWord(curr);
        }
        inputData.peek();
    }
    if (!(prev.empty())) {
        AddToBuffer(trie.GetCode(prev), trie.codeLen, outputData);
    }
    AddToBuffer(exitCode, trie.codeLen, outputData);
    Flush();
}

int LZW::DecodeLZW(std::istream& encodedData, std::ostream& decodedData) {
    uint32_t maxCodeLen = GetBits(32, encodedData);
    if (!(maxCodeLen == 12 || maxCodeLen == 16)) {
        return 1;
    }
    std::vector<std::string> dict;
    dict.push_back("");
    uint8_t sym = 0x0;
    for (uint32_t i = 0; i <= 0xff;  ++sym, ++i) {   //conversion from char to string
        dict.push_back(std::string(1, sym));
    }

    std::string currDecode;
    uint32_t exitCode = 0;

    uint32_t code = GetBits(GetCodeLen(dict.size()), encodedData);
    std::string prev = dict[code];  //first code separatly
    std::string full = prev;
    while (!Empty(encodedData)) {
        code = GetBits(GetCodeLen(dict.size() + 1), encodedData);
        if (code == exitCode) {
            break;
        }
        if (code == dict.size()) {
            //code that is not in dictionary
            //previous decoded string + its first letter
            currDecode = prev[0];
            full += currDecode;
            dict.push_back(full);
            decodedData.write(prev.data(), prev.length());
            prev = dict.back();
            full = prev;
        } else {
            currDecode = dict[code];
            full += currDecode[0];
            dict.push_back(full);
            decodedData.write(prev.data(), prev.length());
            prev = currDecode;
            full = prev;

        }
        //now one more than currently in dictionary
        if (GetCodeLen(dict.size() + 1) > maxCodeLen) {
            decodedData.write(prev.data(), prev.length());
            dict.resize(0xff + 2);
            prev = dict[GetBits(GetCodeLen(dict.size() + 1),  encodedData)];
            full = prev;
        }
    }
    decodedData.write(prev.data(), prev.length());
    return 0;
}

