/*
 * Author:  Ehiremen Ekore
 * Started: 03/2019
 */

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>

#include "tempMap.h"

#define OPCODE_LENGTH 6
#define FUNC_LENGTH 11
#define IMM_LENGTH 16
#define OFFSET_LENGTH 26

// Returns '0' for '1' and '1' for '0'
char flip(char c) {return (c == '0')? '1': '0';}

string getFileName();
void convertToLowerCaseAndClean(const string &fileName);
string registerToBinary(const string &regName);
string intToBinary(int numberOfBits, int convertInt);
string immToBinary(const string &immediate);
string negativeToBinary(int numberOfBits, int convertInt);
void splitString(string inputString, string* regString, string* immString);
void sortingLabels(const string &iFile, vector<string> &vecOfCodeLine, vector<string> &vecOfLabelsAdr);
void convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr);

class R_Instr{  //for all R-type ALU instr
public:
    const int opCode = 0;
    string opcodeStr = intToBinary(OPCODE_LENGTH, opCode);
    string rs1, rs2, rd, func, finalInstr;

    //code comes: func rd rs1 rs2; e.g. add r2 r3 r4 = r2 <- r3 ADD r4
    explicit R_Instr(string &strng){
        istringstream instrString(strng);
        int tempIndex = 0;
        int funcInt;

        do {
            // Read a word
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0){
                auto funcCode = AluFunc.find(tempWord);    //iterator for AluFunc map
                funcInt = funcCode->second;                 //get the number mapped to the funcCode
                func = intToBinary(FUNC_LENGTH, funcInt);
            }
            else if (tempIndex == 1){
                rd = registerToBinary(tempWord);
            }
            else if (tempIndex == 2){
                rs1 = registerToBinary(tempWord);
            }
            else if (tempIndex == 3){
                rs2 = registerToBinary(tempWord);
            }
            tempIndex++;

            // While there is more to read
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + rd + func;
    }
};

class I_LHI_Instr{      //for LHI (load high immediate) instr
public:
    int opcodeValue;
    string opcodeStr, rs1, immStr, finalInstr;
    const string rs2 = "00000";

    explicit I_LHI_Instr(string &strng){
        istringstream instrString(strng);
        int tempIndex = 0;

        //instr format: opcode rs1 imm
        do {
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0){
                auto opCode = Opcode.find(tempWord);    //iterator for Opcode map
                opcodeValue = opCode->second;           //get the number mapped to the opCode
                opcodeStr = intToBinary(OPCODE_LENGTH, opcodeValue);
            }
            else if (tempIndex == 1){
                rs1 = registerToBinary(tempWord);
            }
            else if (tempIndex == 2){
                immStr = immToBinary(tempWord);
            }
            tempIndex++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }
};

class I_Load_Instr{     //for LOAD instrs in format: [instr] [rs2] [imm]([rs1])
public:
    int opcodeValue;
    string opcodeStr;
    string rs1, rs2, immStr, finalInstr;

    explicit I_Load_Instr(string &strng){
        istringstream instrString(strng);
        int tempIndex = 0;
        string immWithReg, immTemp, rs1Temp;

        //instr format: opcode rs2 immRs1
        do {
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0){
                auto opCode = Opcode.find(tempWord);    //iterator for Opcode map
                opcodeValue = opCode->second;           //get the number mapped to the opCode
                opcodeStr = intToBinary(OPCODE_LENGTH, opcodeValue);
            }
            else if (tempIndex == 1){
                rs2 = registerToBinary(tempWord);
            }
            else if (tempIndex == 2){
                immWithReg = tempWord;
                splitString(immWithReg, &rs1Temp, &immTemp);
                rs1 = registerToBinary(rs1Temp);
                immStr = immToBinary(immTemp);
            }
            tempIndex++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }
};

class I_Store_Instr{    //for STORE instr
public:
    int opcodeValue;
    string opcodeStr;
    string rs1, rs2, immStr, finalInstr;

    explicit I_Store_Instr(string &strng){
        istringstream instrString(strng);
        int tempIndex = 0;
        string immWithReg, immTemp, rs1Temp;

        //instr format: opcode immRs1 rs2
        do {
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0){
                auto opCode = Opcode.find(tempWord);    //iterator for Opcode map
                opcodeValue = opCode->second;           //get the number mapped to the opCode
                opcodeStr = intToBinary(OPCODE_LENGTH, opcodeValue);
            }
            else if (tempIndex == 1){
                immWithReg = tempWord;
                splitString(immWithReg, &rs1Temp, &immTemp);
                rs1 = registerToBinary(rs1Temp);
                immStr = immToBinary(immTemp);
            }
            else if (tempIndex == 2){
                rs2 = registerToBinary(tempWord);
            }
            tempIndex++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }
};

class I_Math_Instr{ //for I-type ALU instructions
public:
    int opcodeValue;
    string opcodeStr, rs1, rs2, immStr, finalInstr;

    explicit I_Math_Instr(string &strng){
        istringstream instrString(strng);
        int tempIndex = 0;

        //instr format: opcode rs2 rs1 imm
        do {
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0){
                auto opCode = Opcode.find(tempWord);        //iterator for Opcode map
                opcodeValue = opCode->second;               //get the number mapped to the opCode
                opcodeStr = intToBinary(OPCODE_LENGTH, opcodeValue);
            }
            else if (tempIndex == 1){
                rs2 = registerToBinary(tempWord);
            }
            else if (tempIndex == 2){
                rs1 = registerToBinary(tempWord);
            }
            else if (tempIndex == 3){
                immStr = immToBinary(tempWord);
            }
            tempIndex++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }
};

class I_Jump_Instr{ //for JR and JALR instructions
public:
    int opcodeValue;
    string opcodeStr, rs1, finalInstr;
    const string rs2 = "00000";
    const string immStr = immToBinary("0");

    explicit I_Jump_Instr(string &strng){
        istringstream instrString(strng);
        int tempIndex = 0;

        //instr format: opcode rs2 rs1 imm
        do {
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0){
                auto opCode = Opcode.find(tempWord);    //iterator for Opcode map
                opcodeValue = opCode->second;                 //get the number mapped to the opCode
                opcodeStr = intToBinary(OPCODE_LENGTH, opcodeValue);
            }
            else if (tempIndex == 1){
                rs1 = registerToBinary(tempWord);
            }
            tempIndex++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }
};

class J_OR_Branch_Instr{    //for regular Jump and Branch instr
public:
    int opcode;
    int offset;
    string rs1;
    string rs2 = "00000";
    string labelName, offsetStr, opcodeStr, finalInstr;

    explicit J_OR_Branch_Instr(string &strng, int currentInstrLine, vector<string> &vecOfLabelsAdr) {
        istringstream instrString(strng);
        int tempIndex = 0;
        string word1, word2;

        do {
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0) {
                auto opCode = Opcode.find(tempWord);    //iterator for Opcode map
                opcode = opCode->second;                 //get the number mapped to the opCode
                opcodeStr = intToBinary(OPCODE_LENGTH, opcode);
            } else if (tempIndex == 1) {
                word1 = tempWord;
            } else if (tempIndex == 2) {
                word2 = tempWord;
            }
            tempIndex++;

            // While there is more to read
        } while (instrString);

        if (tempIndex == 3) {//J-type
            labelName = word1;
            auto iteratorJ = find(vecOfLabelsAdr.begin(), vecOfLabelsAdr.end(), labelName);
            if (iteratorJ != vecOfLabelsAdr.end()){
                int tempInt = (iteratorJ - vecOfLabelsAdr.begin());
                offset = 4*(tempInt - currentInstrLine - 1);
            }
            if (offset < 0){
                offsetStr = negativeToBinary(OFFSET_LENGTH, offset);
            }
            else{
                offsetStr = intToBinary(OFFSET_LENGTH, offset);
            }

            finalInstr = opcodeStr + offsetStr;
        }
        if (tempIndex == 4) {//I-type: Branch
            rs1 = registerToBinary(word1);

            labelName = word2;
            auto iteratorJ = find(vecOfLabelsAdr.begin(), vecOfLabelsAdr.end(), labelName);
            if (iteratorJ != vecOfLabelsAdr.end()) {
                int tempInt = (iteratorJ - vecOfLabelsAdr.begin());
                offset = 4 * (tempInt - currentInstrLine - 1);
            }
            if (offset < 0) {
                offsetStr = negativeToBinary(IMM_LENGTH, offset);
            } else {
                offsetStr = intToBinary(IMM_LENGTH, offset);
            }
            finalInstr = opcodeStr + rs1 + rs2 + offsetStr;
        }
    }
};

//Everything is broken into classes and functions, so the project can readily be split into smaller source files
int main() {
    vector<string> vecOfCodeLine;
    vector<string> vecOfLabelsAdr;
    string vecFile = "tk.txt";

    string inputFileName = getFileName();
    convertToLowerCaseAndClean(inputFileName);
    sortingLabels(vecFile, vecOfCodeLine, vecOfLabelsAdr);
    convertAndWriteInstr(vecOfCodeLine, vecOfLabelsAdr);

    return 0;
}

string getFileName(){
    string inputFileName;

    while(true){
        cout << "Please input the name of your file, including the extension" << endl;
        cout << "If the file is not in the same folder as this program's executable--or if you're not sure--\n"
                "you are free to use an explicit file name" << endl;

        ifstream inputFile;
        getline(cin, inputFileName);
        inputFile.open(inputFileName);
        if (inputFile){
            //checks if the file exists
            break;
        }
        else {
            cout << "FILE: " << inputFileName << " NOT FOUND\n" << endl;
            continue;
        }
    }
    return inputFileName;
}

void convertToLowerCaseAndClean(const string &fileName){
    char inputBuffer;
    char* inputString = (char*) calloc(20, sizeof(char));
    FILE *bufferFile = fopen("lower.txt", "w"); //lowercase file
    FILE *tkFile = fopen("tk.txt", "w");
    ifstream minifyThisFile (fileName);

    while ((inputBuffer = minifyThisFile.get()) != EOF) {
        inputBuffer = tolower(inputBuffer);
        fprintf(bufferFile, "%c", inputBuffer);
    }
    fclose(bufferFile);
    FILE* bufferFile2 = fopen("lower.txt", "r");

    int index = 0;
    while ((inputBuffer = getc(bufferFile2)) != EOF) {
        inputString[index] = inputBuffer;
        index++;

        if ((inputBuffer == '\n') || (EOF)){
            char* token = strtok(inputString, ",:()");
            while (token){
                fprintf(tkFile, "%s", token);
                token = strtok(NULL, ",:()");
            }
            index = 0;
            realloc(inputString, 20);
        }
    }
    fclose(tkFile);
    minifyThisFile.close();
}

string registerToBinary(const string &regName){
    string registerName = regName;
    string registerBinaryString;
    registerName.erase(0, 1);   //delete "r"
    int registerNo = stoi(registerName);

    int binaryNum[5] = {0, 0, 0, 0, 0};
    int i = 5;
    while (registerNo > 0) {
        // storing remainder in binary array
        binaryNum[i-1] = registerNo % 2;
        registerNo = registerNo / 2;
        i--;
    }

    for(auto x : binaryNum){
        registerBinaryString.push_back(x + '0');
    }

    return registerBinaryString;
}

string intToBinary(const int numberOfBits, const int convertInt){
    string binaryString;
    int tempDecimal = convertInt;
    int* binaryNum = (int*) calloc(numberOfBits, sizeof(int));  //use calloc to initialize each bit to '0'
    int i = numberOfBits;

    while (tempDecimal > 0) {
        binaryNum[i-1] = tempDecimal % 2;
        tempDecimal = tempDecimal / 2;
        i--;
    }

    for(int x=0; x<numberOfBits; x++){
        binaryString.push_back(binaryNum[x] + '0');
    }

    return binaryString;
}

string negativeToBinary(int numberOfBits, int convertInt){
    string negativeString, tempBinary;
    string onesComplement, twosComplement;
    int tempDecimal = abs(convertInt);
    tempBinary = intToBinary(numberOfBits, tempDecimal);

    int i;
    for (i = 0; i < numberOfBits; i++){
        onesComplement += flip(tempBinary[i]);
    }

    // for two's complement go from right to left in
    // ones complement and if we get 1 , we make
    // them 0 and keep going left when we get first
    // 0, make that 1 and go out of loop
    twosComplement = onesComplement;
    for (i = numberOfBits - 1; i >= 0; i--)
    {
        if (onesComplement[i] == '1')
            twosComplement[i] = '0';
        else
        {
            twosComplement[i] = '1';
            break;
        }
    }

    // If No break : all are 1 as in 111 or 11111;
    // in such case, add extra 1 at beginning
    if (i == -1){
        twosComplement = "";
        for(int index=0; index <numberOfBits; index++){
            twosComplement += '1';
        }
    }

    negativeString = twosComplement;

    return negativeString;
}

string immToBinary(const string &immediate){
    string immName = immediate;
    string immBinaryString;
    int immDecimal = stoi(immName);

    if (immDecimal < 0){
        immBinaryString = negativeToBinary(IMM_LENGTH, immDecimal);
    }
    else {
        immBinaryString = intToBinary(IMM_LENGTH, immDecimal);
    }

    return immBinaryString;
}

void splitString(string inputString, string* regString, string* immString){
    //this separates [imm] from [reg] when register direct with displacement is used
    int lengthOfString = inputString.length();
    int i=0, tempTest=0;    //tempTest is used to know when the imm ends
    string tempImmStr, tempRegStr;

    while (i < lengthOfString){
        if (isalpha(inputString[i]) != 0) tempTest = 1;
        if ((isalpha(inputString[i]) != 1) && (tempTest == 0)){
            tempImmStr.push_back(inputString[i]);
        }
        else{
            tempRegStr.push_back(inputString[i]);
        }
        i++;
    }
    *regString = tempRegStr;
    *immString = tempImmStr;
}

void sortingLabels(const string &iFile, vector<string> &vecOfCodeLine, vector<string> &vecOfLabelsAdr){
    int index = 0;
    char inputBuffer;
    char* inputString = (char*) calloc(20, sizeof(char));
    ifstream unsortedLabels (iFile);
    string tempString;

    while (getline(unsortedLabels, tempString)) {
        vecOfCodeLine.emplace_back(tempString);
    }

    vector<string> tempVecLabels(vecOfCodeLine.size(), "");
    int instrLine = 0;
    for(auto x: vecOfCodeLine){
        istringstream tempStr(x);

        // Read a word (the first line of the string)
        string word;
        tempStr >> word;
        auto opcodeCheck = Opcode.find(word);       //iterator for Opcode map
        auto funcCodeCheck = AluFunc.find(word);    //iterator for AluFunc map
        //if the first word isn't in either map, store it as a label
        if (opcodeCheck == Opcode.end()){
            if (funcCodeCheck == AluFunc.end()){
                tempVecLabels.at(instrLine) = word;
                x.erase(0, word.length()+1);        //remove the label and following space from the stored instr
                vecOfCodeLine.at(instrLine) = x;
            }
        }
        instrLine++;
    }
    vecOfLabelsAdr = tempVecLabels;
}

void convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr){
    int instrLine = 0;
    int tempValue;
    ofstream instrFile("instr.txt");

    for(auto x: vecOfCodeLine){
        istringstream tempStr(x);

        // Read a word (the first word of the string)
        string word;
        tempStr >> word;
        auto opcodeCheck = Opcode.find(word);       //iterator for Opcode map
        auto funcCodeCheck = AluFunc.find(word);    //iterator for AluFunc map
        if (opcodeCheck != Opcode.end()){
            tempValue = opcodeCheck->second;
            if ((tempValue >= 2) && (tempValue <= 5)){
                J_OR_Branch_Instr testBranchInstr(x, instrLine, vecOfLabelsAdr);
                instrFile << testBranchInstr.finalInstr << endl;
            }
            else if (((tempValue >= 8) && (tempValue <= 14)) || ((tempValue >= 24) && (tempValue <= 29))
                || (tempValue >= 48)){
                I_Math_Instr testMathInstr(x);
                instrFile << testMathInstr.finalInstr << endl;
            }
            else if (tempValue == 15){
                I_LHI_Instr testLhiInstr(x);
                instrFile << testLhiInstr.finalInstr << endl;
            }
            else if ((tempValue == 18) || (tempValue == 19)){
                I_Jump_Instr testJumpInstr(x);
                instrFile << testJumpInstr.finalInstr << endl;
            }
            else if ((tempValue >= 32) && (tempValue <= 37)){
                I_Load_Instr testLoadInstr(x);
                instrFile << testLoadInstr.finalInstr << endl;
            }
            else if ((tempValue >= 40) && (tempValue <= 43)){
                I_Store_Instr testStoreInstr(x);
                instrFile << testStoreInstr.finalInstr << endl;
            }
        }
        else if (funcCodeCheck != AluFunc.end()){
            R_Instr testRInstr(x);
            instrFile << testRInstr.finalInstr << endl;
        }
        instrLine++;
    }
    instrFile.close();
}