#include "symtable.h"
#include "iostream"
int symtab[65540][3];

/*symtab[][1] = 0 => "UNKNOWN";
 *symtab[][1] = 1 => "WORD";
 *symtab[][1] = 2 => "BYTE";
 *symtab[][1] = 3 => "INST";
 */

void initialise_symtab ( void ) {
	for ( int i = 0; i < 65540; i++ ) {
		symtab[i][0] = -1;
		symtab[i][1] = 0;
		symtab[i][2] = 0;
	}
}

void set_symtab ( int addr, int counter, int status ) {
	symtab[addr][0] = counter;
	update_symtab(addr,status);
}

void update_symtab ( int addr, int status ) {
	symtab[addr][1] = status;
}

void set_flag ( int addr ) {
	symtab[addr][2] = 1;
}

int get_label ( int addr ) {
	return symtab[addr][0];
}

int get_status ( int addr ) {
	return symtab[addr][1];
}

int get_flag ( int addr ) {
	return symtab[addr][2];
}

void printtable () {
	std::cout << "----------------SYMTAB----------------\n";
	for ( int i = 0; i < 65540; i++ ) {
		if ( symtab[i][0] != -1 ) {
			std::cout << std::hex << i << " ";
			std::cout << std::dec << symtab[i][0] << " " << symtab[i][1] << " " << symtab[i][2] << std::endl;
		}
	}
}
