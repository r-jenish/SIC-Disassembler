#include <iostream>
#include <string>
#include "optable.h"
#include "convertor.h"
#include "symtable.h"
#include "disassembler.h"
#include "error.h"

int main (int argc, char *argv[]) {
	initialise_optab();
	initialise_symtab();
	
	readinput(argv[1]);
	print();
	printtable();
	return 0;
}
