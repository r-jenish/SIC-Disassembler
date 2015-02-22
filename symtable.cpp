#include "symtable.h"

int symtab[65536][2];

/*symtab[][0] = "UNKNOWN";
 *symtab[][1] = "UNDEFINED";
 *symtab[][2] = "DEFINED";
 */

void initialise_symtab ( void ) {
	for ( int i = 0; i < 65536; i++ ) {
		symtab[i][0] = -1;
		symtab[i][1] = 0;
	}
}

void set_symtab ( int addr, int counter ) {
	symtab[addr][0] = counter;
	update_symtab(addr,1);
}

void update_symtab ( int addr, int status ) {
	symtab[addr][1] = status;
}

int get_label ( int addr ) {
	return symtab[addr][0];
}

int get_status ( int addr ) {
	return symtab[addr][1];
}
