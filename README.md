# **dlx-assembler**
An assembler that converts dlx-format instructions to 32-bit binaries. Done using C++ and compiled on JetBrains CLion
  The program reads a dlx-format instruction file and converts each line to its equivalent assembly language format. The assembly instructions get written to a file (instr.txt)--this can be changed in "void convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr)" ~line 546.
  
###  *My approach:*
  * read the input file (specified by the user in string getFileName())
  * convert to lowercase and store in a new file (lower.txt)
  * remove commas, spaces, and parentheses from the lower.txt file, and store in a new tokenated file (tk.txt)
  * read the first word of each instruction and check if that word is in either Opcode OR ALUFunc map; if not, we identify it as a label
  finally, run void convertAndWriteInstr(vector<string> vecOfCodeLine, vector<string> vecOfLabelsAdr) to generate the binaries for each instruction
  
