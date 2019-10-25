//Functions used at various parts of the project

#pragma once

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

using namespace std;
// Returns '0' for '1' and '1' for '0'
char flip(char c) {return (c == '0')? '1': '0';}

string getFileName();
    /* lets the user specify what file to get instructions from; prompts for a retry if unable to find/open file.
    */
vector<string> readAndBeautifyInput(const string &fileName);
    /* reads in the instruction file (specified by fileName), converts it to lowercase, removes punctuation and parentheses, and returns a vector with each instruction (represented by a line in the file) stored in a string.
    */
string registerToBinary(const string &regName);
    /* takes in a string representing a register (e.g. "r12") and returns a string containing a 5 bit representation of the register's number
    */
string intToBinary(int number, int outputLength);
    /* returns a string containing an [outputLength] binary representation of a signed integer (number)
     * delegates conversions for negative numbers to negToBinary
    */
string negToBinary(string posBinary);
    /* given a binary representation of a number (without considering the sign), returns a string containing the twos complement of that number
    */
vector<string> getLabels(vector<string> &codeLines);
vector<string> convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr);
