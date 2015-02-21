#include <string>
#include "optable.h"

std::string optab[256];

void initialise_optab ( void ) {
	for ( int i = 0; i < 256; i++ ) {
		optab[i] = "NULL";
	}
	optab[0] = "LDA";
	optab[4] = "LDX";
	optab[8] = "LDL";
	optab[12] = "STA";
	optab[16] = "STX";
	optab[20] = "STL";
	optab[24] = "ADD";
	optab[28] = "SUB";
	optab[32] = "MUL";
	optab[36] = "DIV";
	optab[40] = "COMP";
	optab[44] = "TIX";
	optab[48] = "JEQ";
	optab[52] = "JGT";
	optab[56] = "JLT";
	optab[60] = "J";
	optab[64] = "AND";
	optab[68] = "OR";
	optab[72] = "JSUB";
	optab[76] = "RSUB";
	optab[80] = "LDCH";
	optab[84] = "STCH";
	optab[216] = "RD";
	optab[220] = "WD";
	optab[224] = "TD";
	optab[232] = "STSW";	
}

std::string get_mnemonic ( int opcode ) {
	return optab[opcode];
}
