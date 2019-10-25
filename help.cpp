##include "help.hpp"

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
    inputFile.close();
    return inputFileName;
}

vector<string> readAndBeautifyInput(const string &fileName){
    vector<string>instrs;

    ifstream instrFile;
    instrFile.open("input.txt");
    string buffer;

    while(!instrFile.eof()){
        getline(instrFile, buffer);
        transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
        //convert the line stored in buffer to lowercase

        buffer.erase(remove(buffer.begin(), buffer.end(), ','), buffer.end());
        buffer.erase(remove(buffer.begin(), buffer.end(), ':'), buffer.end());
        buffer.erase(remove(buffer.begin(), buffer.end(), '('), buffer.end());
        buffer.erase(remove(buffer.begin(), buffer.end(), ')'), buffer.end());
        //erase all of ",:()" from buffer

        instrs.push_back(buffer);
    }
    instrFile.close();
    return instrs;
}

string registerToBinary(const string &regName){
    string regString = regName;
    regString.erase(0, 1);   //delete "r" (first character in string)
    int regInt = stoi(regString);

    return intToBinary(registerNo, REG_LENGTH);
}

string intToBinary(int number, int outputLength){
    bool pos = (number<0)? false : true;
    number = abs(number);
    int binaryNum[outputLength];
    int len = outputLength;
    string binaryString;

    for (int k = 0; k < outputLength; k++) {
        binaryNum[k] = 0;
    }

    while (number > 0) {
        binaryNum[outputLength - 1] = number % 2; //take the remainder
        number = number / 2;              //then divide by 2
        outputLength--; //then move the counter from the last digit to the one before it
    }

    for (int k = 0; k<len; k++) {
        //this is what stores it into a string
        binaryString.push_back(binaryNum[k] + '0');
    }

    return (pos ? binaryString : negToBinary(binaryString));
}

string negToBinary(string posBinary){
    int n = posBinary.length();
    int i;
    string ones, twos;
    ones = twos = "";

    for (i = 0; i < n; i++) {
        ones += flip(posBinary[i]);
    }
    // for ones complement flip every bit

    twos = ones;
    for (i = n-1; i >= 0; i--){
        twos[i] = (ones[i]=='1') ? '0' : '1';
        if (ones[i]==0) break;
    }
    // for two's complement go from right to left in
    // ones complement and if we get 1 make, we make
    // them 0 and keep going left when we get first
    // 0, make that 1 and go out of loop

    if (i == -1) {
        twos = '1' + twos;
    }
    // If No break : all are 1 as in 111 or 11111;
    // in such case, add extra 1 at beginning

    return twos;
}

vector<string> getLabels(vector<string> &codeLines){
    vector<string> labels(codeLines.size(), "");
    int currLine = 0;
    for(string& x: codeLines){
        istringstream tempStr(x);

        // Read a word (the first of the string)
        string word;
        tempStr >> word;
        auto opCheck = Opcode.find(word);       //iterator for Opcode map
        auto funcCheck = AluFunc.find(word);    //iterator for AluFunc map

        //if the first word isn't in either map, store it as a label
        if (opCheck == Opcode.end()){
            if (funcCheck == AluFunc.end()){
                labels[currLine] = word;
                x.erase(0, word.length()+1);        //remove the label and following space from codeLines
                //codeLines[currLine] = x;
            }
        }
        currLine++;
    }
    return labels;
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
                out.emplace_back(instr.finalInstr);
                instrFile << instr.finalInstr << endl;
            }
            else if (((tempValue >= 8) && (tempValue <= 14)) || ((tempValue >= 24) && (tempValue <= 29))
                || (tempValue >= 48)){
                I_Math_Instr instr(x);
                out.emplace_back(instr.finalInstr);
                instrFile << instr.finalInstr << endl;
            }
            else if (tempValue == 15){
                I_LHI_Instr instr(x);
                out.emplace_back(instr.finalInstr);
                instrFile << instr.finalInstr << endl;
            }
            else if ((tempValue == 18) || (tempValue == 19)){
                I_Jump_Instr instr(x);
                out.emplace_back(instr.finalInstr);
                instrFile << instr.finalInstr << endl;
            }
            else if ((tempValue >= 32) && (tempValue <= 37)){
                I_Load_Store_Instr instr(x);
                out.emplace_back(instr.finalInstr);
                instrFile << instr.finalInstr << endl;
            }
            else if ((tempValue >= 40) && (tempValue <= 43)){
                I_Load_Store_Instr instr(x);
                out.emplace_back(instr.finalInstr);
                instrFile << instr.finalInstr << endl;
            }
        }
        else if (funcCodeCheck != AluFunc.end()){
            R_Instr instr(x);
            out.emplace_back(instr.finalInstr);
            instrFile << instr.finalInstr << endl;
        }
        instrLine++;
    }
    instrFile.close();
    return out;
}
