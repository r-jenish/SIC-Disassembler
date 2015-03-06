#include <iostream>
#include <string>
#include "optable.h"
#include "convertor.h"
#include "symtable.h"
#include "disassembler.h"
#include "util.h"

int main (int argc, char *argv[]) {
	displaytitle();
	if ( argc == 3 ) {
		startingdisassembling();
		initialise_optab();
		initialise_symtab();
		readinput(argv[1]);
		writeoutput(argv[2]);
		finishdisassembling();
		displaylastmessage();
	} else {
		fatalerror("TOO LESS ARGUMENTS.\nUsage:\t./sicdisassembler inputfile outputfile");
	}
	return 0;
}
