# **dlx-assembler**
an assembler that converts dlx-format instructions to 32-bit binaries. done using C++ and compiled on JetBrains CLion
  the program reads a dlx-format instruction file and converts each line to its equivalent assembly language format. the assembly instructions get written to a file (instr.txt)--this can be changed in "void convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr)" ~line 546.
  
###  *my approach:*
  * read the input file (specified by the user in string getFileName())
  * convert to lowercase and store in a new file (lower.txt)
  * remove commas, colons, and parentheses from the lower.txt file, and store in a new tokenated file (tk.txt)
  * read the first word of each instruction and check if that word is in either Opcode OR ALUFunc map; if not, we identify it as a label
  finally, run void convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr) to generate the binaries for each instruction
  
**NOTE:**   dlx_assembler.cpp is fairly large (~600 lines), but everything called in main() is either a defined class or a defined function  so this code can readily be divided into multiple source files
  
### *files included:*
  * [dlx_handouts.pdf](dlx_handouts.pdf)    - dlx instruction format and examples
  * [dlx_assembler.cpp](dlx_assembler.cpp)    - the main file for the project
  * [tempMap.h](tempMap.h)                    - a header file that goes with dlx_assembler.cpp
  * [CMakeLists.txt](CMakeLists.txt)
  * [input.txt](input.txt)                    - sample input file
