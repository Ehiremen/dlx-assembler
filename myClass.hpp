#pragma once

#include "help.hpp"


class R_Instr{  //for all R-type ALU instr
private:
    const int opCode = 0;
    string opcodeStr = intToBinary(opCode, OPCODE_LENGTH);
    string rs1, rs2, rd, func, finalInstr;

public:
    //code comes: func rd rs1 rs2; e.g. add r2 r3 r4 = r2 <- r3 ADD r4
    R_Instr(string &input){
        istringstream instrString(input);
        int index = 0;
        int funcInt;

        do {
            // Read a word
            string tempWord;
            instrString >> tempWord;

            if (index == 0){
                auto funcCode = AluFunc.find(tempWord);    //iterator for AluFunc map
                funcInt = funcCode->second;                 //get the number mapped to the funcCode
                func = intToBinary(funcInt, FUNC_LENGTH);
            }
            else if (index == 1){
                rd = registerToBinary(tempWord);
            }
            else if (index == 2){
                rs1 = registerToBinary(tempWord);
            }
            else if (index == 3){
                rs2 = registerToBinary(tempWord);
            }
            index++;

            // While there is more to read
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + rd + func;
    }

    string getInstr(){
        return finalInstr;
    }
};

class I_LHI_Instr{      //for LHI (load high immediate) instr
private:
    int opcodeValue;
    string opcodeStr, rs1, immStr, finalInstr;
    const string rs2 = "00000";

public:
    explicit I_LHI_Instr(string &input){
        istringstream instrString(input);
        int index = 0;

        //instr format: opcode rs1 imm
        do {
            string tempWord;
            instrString >> tempWord;

            if (index == 0){
                auto opCode = Opcode.find(tempWord);    //iterator for Opcode map
                opcodeValue = opCode->second;           //get the number mapped to the opCode
                opcodeStr = intToBinary(opcodeValue, OPCODE_LENGTH);
            }
            else if (index == 1){
                rs1 = registerToBinary(tempWord);
            }
            else if (index == 2){
                immStr = intToBinary(stoi(tempWord), IMM_LENGTH);
            }
            index++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }

    string getInstr(){
        return finalInstr;
    }
};

class I_Load_Store_Instr {
private:
    string opcode, rs1, rs2, imm, finalInstr;

    void split(string word) {
        int rIndex;
        for (int i = 0; i < word.length(); i++){
            if (word[i] != 'r') {
                imm += word[i];
            } else {
                rIndex = i;
                break;
            }
        }
        for (int i = rIndex; i < word.length(); i++){
            rs1 += word[i];
        }
    }

public:
    I_Load_Store_Instr (string input ) {
        istringstream tempStr(input);
        int count = 0;

        string word;
        tempStr >> word; //reads first word of temp string (opcode)

        auto funcCode = Opcode.find(word);
        int opInt = funcCode->second;
        opcode = intToBinary(opInt, OPCODE_LENGTH);

        if (32 <= opInt && opInt <= 37) { //checks if its load
            tempStr >> word;
            rs2 = registerToBinary(word);
            tempStr >> word;
            split(word);
            rs1 = registerToBinary(rs1);
            int conv = stoi(imm);
            imm = intToBinary(conv, IMM_LENGTH);
        }
        else { //for store
            tempStr >> word;
            split(word); //gets imm and rs1
            rs1 = registerToBinary(rs1);
            int conv = stoi(imm);
            imm = intToBinary(conv, IMM_LENGTH);
            tempStr >> word;
            rs2 = registerToBinary(word);
        }
        finalInstr = opcode + rs1 + rs2 + imm;
    }

    string getInstr(){
        return finalInstr;
    }
};

class I_Math_Instr{ //for I-type ALU instructions
private:
    int opcodeValue;
    string opcodeStr, rs1, rs2, immStr, finalInstr;

public:
    explicit I_Math_Instr(string &input){
        istringstream instrString(input);
        int index = 0;

        //instr format: opcode rs2 rs1 imm
        do {
            string tempWord;
            instrString >> tempWord;

            if (index == 0){
                auto opCode = Opcode.find(tempWord);        //iterator for Opcode map
                opcodeValue = opCode->second;               //get the number mapped to the opCode
                opcodeStr = intToBinary(opcodeValue, OPCODE_LENGTH);
            }
            else if (index == 1){
                rs2 = registerToBinary(tempWord);
            }
            else if (index == 2){
                rs1 = registerToBinary(tempWord);
            }
            else if (index == 3){
                immStr = intToBinary(stoi(tempWord), IMM_LENGTH);
            }
            index++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }

    string getInstr(){
        return finalInstr;
    }
};

class I_Jump_Instr{ //for JR and JALR instructions
private:
    int opcodeValue;
    string opcodeStr, rs1, finalInstr;
    const string rs2 = "00000";
    const string immStr = intToBinary(0, IMM_LENGTH);

public:
    explicit I_Jump_Instr(string &input){
        istringstream instrString(input);
        int index = 0;

        //instr format: opcode rs2 rs1 imm
        do {
            string word;
            instrString >> word;

            if (index == 0){
                auto opCode = Opcode.find(word);    //iterator for Opcode map
                opcodeValue = opCode->second;                 //get the number mapped to the opCode
                opcodeStr = intToBinary(opcodeValue, OPCODE_LENGTH);
            }
            else if (index == 1){
                rs1 = registerToBinary(word);
            }
            index++;
        } while (instrString);

        finalInstr = opcodeStr + rs1 + rs2 + immStr;
    }
    string getInstr(){
        return finalInstr;
    }
};

class J_OR_Branch_Instr{    //for regular Jump and Branch instr
private:
    int opcode;
    int offset;
    string rs1;
    string rs2 = "00000";
    string labelName, offsetStr, opcodeStr, finalInstr;

public:
    J_OR_Branch_Instr(string input, int currentInstrLine, vector<string> labels) {
        istringstream instrString(input);
        int tempIndex = 0;
        string word1, word2;

        do {
            string tempWord;
            instrString >> tempWord;

            if (tempIndex == 0) {
                auto opCode = Opcode.find(tempWord);    //iterator for Opcode map
                opcode = opCode->second;                 //get the number mapped to the opCode
                opcodeStr = intToBinary(opcode, OPCODE_LENGTH);
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
            auto iteratorJ = find(labels.begin(), labels.end(), labelName);
            if (iteratorJ != labels.end()){
                int tempInt = (iteratorJ - labels.begin());
                offset = 4*(tempInt - currentInstrLine - 1);
            }
            offsetStr = intToBinary(offset, OFFSET_LENGTH);
            finalInstr = opcodeStr + offsetStr;
        }
        if (tempIndex == 4) {//I-type: Branch
            rs1 = registerToBinary(word1);

            labelName = word2;
            auto iteratorJ = find(labels.begin(), labels.end(), labelName);
            if (iteratorJ != labels.end()) {
                int tempInt = (iteratorJ - labels.begin());
                offset = 4 * (tempInt - currentInstrLine - 1);
            }
            offsetStr = intToBinary(offset, IMM_LENGTH);
            finalInstr = opcodeStr + rs1 + rs2 + offsetStr;
        }
    }

    string getInstr(){
        return finalInstr;
    }
};
