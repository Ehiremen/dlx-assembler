/*
 * Author:      Ehiremen Ekore
 * Started:     03/2019
 * Modified:    10/2019
 */


#include "tempMap.h"
#include "help.hpp"

#define OPCODE_LENGTH 6
#define FUNC_LENGTH 11
#define IMM_LENGTH 16
#define OFFSET_LENGTH 26

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
    vector<string> codeLines;
    vector<string> labels;
    vector<string> output;

    string inputFileName = getFileName();
    codeLines = readAndBeautifyInput(inputFileName);
    labels = getLabels(codeLines);
    output = convertAndWriteInstr(codeLines, vecOfLabelsAdr);

    return 0;
}
