//
// Created by ehise on 3/17/2019.
//

#pragma once

using namespace std;

static map<string, int> Opcode = {
        {"alu", 0}, {"j", 2}, {"jal", 3}, {"beqz", 4}, {"bnez", 5}, {"addi", 8}, {"addui", 9}, {"subi", 10},
        {"subui", 11}, {"andi", 12}, {"ori", 13}, {"xori", 14}, {"lhi", 15}, {"jr", 18}, {"jalr", 19}, {"seqi", 24},
        {"snei", 25}, {"slti", 26}, {"sgti", 27}, {"slei", 28}, {"sgei", 29}, {"lb", 32}, {"lh", 33}, {"lw", 35},
        {"lbu", 36}, {"lhu", 37}, {"sb", 40}, {"sh", 41}, {"sw", 43}, {"sequi", 48}, {"sneui", 49},
        {"sltui", 50}, {"sgtui", 51}, {"sleui", 52}, {"sgeui", 53}, {"slli", 54}, {"srli", 55}, {"srai", 56}
};

static map<string, int> AluFunc{
        {"sll", 4}, {"srl", 6}, {"sra", 7}, {"sltu", 18}, {"sgtu", 19}, {"sleu", 20}, {"sgeu", 21},
        {"add", 32}, {"addu", 33}, {"sub", 34}, {"subu", 35}, {"and", 36}, {"or", 37}, {"xor", 38},
        {"seq", 40}, {"sne", 41}, {"slt", 42}, {"sgt", 43}, {"sle", 44}, {"sge", 45}
};