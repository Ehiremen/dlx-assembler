/*
 * Author:      Ehiremen Ekore
 * Started:     03/2019
 * Modified:    10/2019
 */


#include "myClass.hpp"

vector<string> convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr);

int main() {
    vector<string> codeLines;
    vector<string> labels;
    vector<string> output;

    string inputFileName = getFileName();
    codeLines = readAndBeautifyInput(inputFileName);
    labels = getLabels(codeLines);
    output = convertAndWriteInstr(codeLines, labels);

    for(int k=0; k<codeLines.size(); k++){
        cout << codeLines[k] << "*" << labels[k] << endl << output[k] << endl << endl;
    }

    return 0;
}

vector<string> convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr){
    vector<string> out;
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
                J_OR_Branch_Instr instr(x, instrLine, vecOfLabelsAdr);
                out.emplace_back(instr.getInstr());
                instrFile << instr.getInstr() << endl;
            }
            else if (((tempValue >= 8) && (tempValue <= 14)) || ((tempValue >= 24) && (tempValue <= 29)) || (tempValue >= 48)){
                I_Math_Instr instr(x);
                out.emplace_back(instr.getInstr());
                instrFile << instr.getInstr() << endl;
            }
            else if (tempValue == 15){
                I_LHI_Instr instr(x);
                out.emplace_back(instr.getInstr());
                instrFile << instr.getInstr() << endl;
            }
            else if ((tempValue == 18) || (tempValue == 19)){
                I_Jump_Instr instr(x);
                out.emplace_back(instr.getInstr());
                instrFile << instr.getInstr() << endl;
            }
            else if ((tempValue >= 32) && (tempValue <= 37)){
                I_Load_Store_Instr instr(x);
                out.emplace_back(instr.getInstr());
                instrFile << instr.getInstr() << endl;
            }
            else if ((tempValue >= 40) && (tempValue <= 43)){
                I_Load_Store_Instr instr(x);
                out.emplace_back(instr.getInstr());
                instrFile << instr.getInstr() << endl;
            }
        }
        else if (funcCodeCheck != AluFunc.end()){
            R_Instr instr(x);
            out.emplace_back(instr.getInstr());
            instrFile << instr.getInstr() << endl;
        }
        instrLine++;
    }
    instrFile.close();
    return out;
}
