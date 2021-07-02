#pragma once

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

class ARIFM {

protected:
    
    //how many bits will be used for encoding
    const unsigned int codeBits = 32; 

    const unsigned int noOfChar = 256;
    const unsigned int EOF_sybol = noOfChar + 1;
    const unsigned int noOfSymbols = noOfChar + 1;
    

    // swap zone for updating tables freq and cum-freq
    std::vector<unsigned long> charToIndex = std::vector<unsigned long>(noOfChar); 
    std::vector<unsigned char> indexToChar = std::vector<unsigned char>(noOfSymbols + 1);

    // main tables of probability
    std::vector<unsigned long> frequency = std::vector<unsigned long>(noOfSymbols + 1); 
    std::vector<unsigned long> cumFrequency = std::vector<unsigned long>(noOfSymbols + 1);

    // input-output for Encode
    std::vector<unsigned char> inputEncode;

    // input-output for Decode
    std::vector<unsigned short int> inputDecode;
    std::vector<unsigned char> outputDecode;

    unsigned long bufferSize;

    unsigned long l; // left bound
    unsigned long h; // right bound


    unsigned long firstQrt;
    unsigned long half;
    unsigned long thirdQtr;

    unsigned long maxFrequency;

    void CodeSymbol(unsigned long input_ch, std::ostream& outputData);
    unsigned long long DecodeSymbol(std::istream& inputData);

    void UpdateModel(unsigned long input_ch);

    void BitPlusFollow(long bit, std::ostream& outputData);
    
    unsigned short int buffer = 0; // to store bits 
    unsigned long long buffercounter = 0; // for next values to dtore in buffer

    int input_bit(std::istream& inputData); 
    void output_bit(int bit, std::ostream& outputData); 

    unsigned long long bitsToGo;
    unsigned long long ullbufferbits;
    unsigned long long bitsToFollow = 0; // to form output bits
    unsigned long long garbageBits = 0;
    unsigned long value = 0;

public:

    ARIFM();
    
    void EncodeArifm(std::istream& inputData, std::ostream& outputData);
    void DecodeArifm(std::istream& inputData, std::ostream& outputData);

};

ARIFM::ARIFM() {

    bufferSize = 60000;
    inputEncode.resize(bufferSize);
    inputDecode.resize(bufferSize);

    l = 0;
    h = (1UL << codeBits) - 1; //4294967295 if 32   // 2**N - 1 
    maxFrequency = (1UL << (codeBits-2)) - 1;

    firstQrt = (h/4 + 1);
    half = 2*firstQrt;
    thirdQtr = 3*firstQrt;
    
    // tables of recoding symbols
    for (unsigned int i = 0; i < noOfChar; i++) { 
        charToIndex[i] = i+1;     
        indexToChar[i+1] = i;
    }

    int cf = noOfSymbols;
    for(unsigned int i = 0; i <= noOfSymbols; i++) {
       cumFrequency[i] = cf--;
       frequency[i] = 1;
    }
   frequency[0]=0;
}

void ARIFM::EncodeArifm(std::istream& inputData, std::ostream& outputData) {

    bitsToGo = 16;
    buffer = 0;
    bitsToFollow = 0;

    while(inputData) {
        (inputData).read(reinterpret_cast<char*>(&inputEncode[0]), bufferSize*sizeof(unsigned char));
        unsigned long long readSize = (inputData).gcount(); 

        inputEncode.resize(readSize);

        if (readSize) {
            for(unsigned long i = 0; i < inputEncode.size(); i++) {
                
                // std::cout << "input_ch: " << (int)inputEncode[i] << std::endl;
                unsigned long input_ch = charToIndex[inputEncode[i]];
                //std::cout << "symbol to code " << input_ch << std::endl;
        
                CodeSymbol(input_ch, outputData);
                UpdateModel(input_ch);
            }
        } 
    } 
    CodeSymbol(EOF_sybol, outputData);

    // done encoding
    bitsToFollow++;
    if (l < firstQrt) { BitPlusFollow(0, outputData); }
    else { BitPlusFollow(1, outputData); }
    
    // flush buffer
    buffer = buffer >> bitsToGo;
    (outputData).write(reinterpret_cast<char*>(&buffer), sizeof(unsigned short int));
}

void ARIFM::CodeSymbol(unsigned long input_ch, std::ostream& outputData) { 
    unsigned long tmp = l;
    l = tmp + (((h-tmp+1)*cumFrequency[input_ch])/cumFrequency[0]);
    h = tmp + (((h-tmp+1)*cumFrequency[input_ch-1])/cumFrequency[0]) - 1;

    for(;;) {
        if (h < half) {
            BitPlusFollow(0, outputData);
        } else if (l >= half) {
            BitPlusFollow(1, outputData);
            l -= half;
            h -= half;
        } else if (l >= firstQrt && h < thirdQtr) {
            bitsToFollow += 1;
            l -= firstQrt;
            h -= firstQrt;
        } else 
            break;
        l = l*2;
        h = h*2+1;
    }        
}

void ARIFM::UpdateModel(unsigned long symbol) {

    // updating model
    int index = 0;
    if (cumFrequency[0] >= maxFrequency) { 
        int cum = 0;
        for (index = noOfSymbols; index >= 0; index--) {
            frequency[index] = (frequency[index]+1)/2;
            cumFrequency[index] = cum;
            cum += frequency[index];
        }
    }
    
    // finding suitable index
    for (index = symbol; frequency[index]==frequency[index-1]; index--);
    
    if ((unsigned int)index < symbol) {
        unsigned long ch_i = indexToChar[index];
        unsigned long ch_symbol = indexToChar[symbol];
        indexToChar[index] = ch_symbol;
        indexToChar[symbol] = ch_i;
        charToIndex[ch_i] = symbol;
        charToIndex[ch_symbol] = index;
    } 

    // updating tables 
    frequency[index]++; 
    while(index > 0) {
        index -= 1;
        cumFrequency[index] += 1;
    }
}


void ARIFM::DecodeArifm(std::istream& inputData, std::ostream& outputData) {
    
    bitsToGo = 0;
    garbageBits = 0;
    value = 0;

    while(inputData) {
        (inputData).read(reinterpret_cast<char*>(&inputDecode[0]), bufferSize*sizeof(unsigned short int));
        unsigned long long readSize = (inputData).gcount(); 
        // divide by 2, unsigned short 2 bytes
        inputDecode.resize(readSize/2);
        buffercounter = 0;
        
        value = 0;
        for (unsigned int i = 0; i < codeBits; i++){
            value = 2*value + input_bit(inputData);
        }
        if (readSize) {
            for(;;) {
                int input_ch = DecodeSymbol(inputData);
                // std::cout << "input_ch: " << input_ch << std::endl;
                if ((unsigned int)input_ch == EOF_sybol) break;
                unsigned char ch = indexToChar[input_ch];
                (outputData).write(reinterpret_cast<char*>(&ch), sizeof(unsigned char));
                outputDecode.push_back(ch);
                // std::cout << "put |" << (unsigned long)ch << "| in file" << std::endl;

                UpdateModel(input_ch);
            } 
        } 
    } 

}


unsigned long long ARIFM::DecodeSymbol(std::istream& inputData) {
    unsigned long long cum = (((value - l) + 1) * cumFrequency[0] - 1)/((h - l) + 1);
    unsigned long long i = 1; 
    while (cumFrequency[i] > cum) i++;

    unsigned long long tmp = l;
    l = tmp + ((h - tmp + 1)*cumFrequency[i])/cumFrequency[0];
    h = tmp + ((h - tmp + 1)*cumFrequency[i-1])/cumFrequency[0] - 1;

    for(;;) {
        if (h < half) {
            // do nothing
        } else if (l >= half) {
            value -= half;
            l -= half;
            h -= half;
        } else if (l >= firstQrt && h < thirdQtr) {
            value -= firstQrt;
            l -= firstQrt;
            h -= firstQrt;
        } else break;

        l = 2*l;
        h = 2*h+1;
        value = 2*value + input_bit(inputData); 
    }

    return i;
}

int ARIFM::input_bit(std::istream& inputData) {
    int t; // to return
    if (bitsToGo == 0) {
        if (buffercounter == bufferSize){
            (inputData).read(reinterpret_cast<char*>(&inputDecode[0]), bufferSize*sizeof(unsigned short int));
            unsigned long long readSize = (inputData).gcount(); 
            // divide by 2, unsigned short 2 bytes
            inputDecode.resize(readSize/2);
            buffercounter = 0;
        }
        // get new value from buffer
        buffer = inputDecode[buffercounter++];

        if (static_cast<long long>(buffer) == EOF) { 
            garbageBits += 1;
            if (garbageBits > codeBits - 2) {
                std::cerr << "Error in file" << std::endl;;
                return 0;
            }
        }
        bitsToGo = 16;
    }
    t = buffer & 1;
    buffer >>= 1;
    bitsToGo -= 1;
    return t;
    
}

void ARIFM::BitPlusFollow(long bit, std::ostream& outputData) { 
    output_bit(bit, outputData);
    while(bitsToFollow > 0) {
        output_bit(!bit, outputData);
        bitsToFollow--;
    }
}

void ARIFM::output_bit(int bit, std::ostream& outputData) {
    //std::cout << "Out: " << bit << std::endl;
    buffer >>= 1;
    if(bit){ buffer |= 0x8000; }
    /*std::cout << "buffer ";
    std::cout << std::bitset<64>(buffer);
    std::cout << std::endl;
    */
    bitsToGo -= 1;
    if (bitsToGo == 0) {
        (outputData).write(reinterpret_cast<char*>(&buffer), sizeof(unsigned short int));
        bitsToGo = 16;
    }
}