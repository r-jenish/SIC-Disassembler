#include "disassembler.h"
#include "convertor.h"
#include "symtable.h"
#include "optable.h"
#include "util.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

string programname;
int start_address;
int exe_address;
int prog_length;
int header_record[19];
bool recordflag[2] = {false,false};
int mem[65540][4];						 /*mem[][0] = hexvalue mem[][1] = 1 =>inst and mem[][1] = 2 =>data mem[][2] = 0 => simple addressing else index based mem[][3] => visited or not*/
int end_record[7];
int vcounter = 0;
int icounter = 0;

void clearmem () {
	for ( int i = 0; i < 65540; i++ ) {
		mem[i][0] = -1;
		mem[i][1] = -1;
		mem[i][2] = 0;
		mem[i][3] = 0;
	}
}

void readinput ( char* input ) {
	clearmem();
	string line;
	string hex = "00";
	ifstream infile;
	infile.open(input,ifstream::in);
	if ( !infile.is_open() ) {
		fatalerror("Unable to open input file.");
	}
	infileopened();
	while ( getline(infile,line) ) {
		if (line == "" ) continue;
		switch ( line.at(0) ) {
			case 'H':
				if ( line.length() == 19 ) {
					header_record[0] = 'H';
					programname = "000000";
					if ( !recordflag[0] ) {
						for ( int i = 0; i < 6; i++ ) {
							header_record[i+1] = line.at(i+1);
							programname.at(i) = line.at(i+1);
						}
						hex = "";
						for ( int i = 7; i < 13; i++ ) {
							header_record[i] = line.at(i);
							hex += line.at(i);
						}
						start_address = hex_to_int6(hex);
						hex = "";
						for ( int i = 13; i < 19; i++ ) {
							header_record[i] = line.at(i);
							hex += line.at(i);
						}
						prog_length = hex_to_int6(hex);
						recordflag[0] = true;
					} else {
						fatalerror("MORE THAN ONE HEADER RECORD.");
					}
				} else {
					fatalerror("HEADER RECORD NOT CORRECT.");
				}
				break;
			case 'T':
				int len;
				hex = "";
				hex += line.at(7);
				hex += line.at(8);
				len = hex_to_int(hex);
				if ( line.length()-9 == len*2 ) {
					hex = "000000";
					int addr;
					for ( int i = 0; i < 6; i++ ) {
						hex.at(i) = line.at(i+1);
					}
					addr = hex_to_int6(hex);
					for  ( int i = 0; i < len; i++ ) {
						hex = "";
						hex += line.at(2*(i+4)+1);
						hex += line.at(2*(i+5));
						mem[addr][0] = hex_to_int(hex);
						addr++;
					}
				} else {
					fatalerror("TEXT RECORD NOT CORRECT.");
				}
				break;
			case 'E':
				if ( line.length() == 7 ) {
					end_record[0] = 'E';
					if ( !recordflag[1] ) {
						hex = "";
						for ( int i = 0; i < 6; i++ ) {
							end_record[i+1] = line.at(i+1);
							hex += line.at(i+1);
						}
						exe_address = hex_to_int6(hex);
						recordflag[1] = true;
					} else {
						fatalerror("MORE THAN ONE END RECORD.");
					}
				} else {
					fatalerror("END RECORD NOT CORRECT.");
				}
				break;
		}
	}
	infile.close();
	infileclosed();
	mem[exe_address][1] = 1;
	pass2(exe_address);
}

void pass2 ( int exeaddr ) {
	mem[exeaddr][3] = 1;
	int memvalue = mem[exeaddr][0];
	string mnemonic = get_mnemonic(memvalue);
	int index;
	int effaddr;
	if ( (mem[exeaddr+1][0]&128) == 128 ) {
		index = 1;
		mem[exeaddr][2] = 1;
		effaddr = ((mem[exeaddr+1][0]&(127))*16*16) + mem[exeaddr+2][0];
	} else {
		index = 0;
		mem[exeaddr][2] = 0;
		effaddr = (mem[exeaddr+1][0]*16*16) + mem[exeaddr+2][0];
	}
	//cout << hex << exeaddr << " " << dec <<  mem[exeaddr][1]  <<  " "<< hex << effaddr << dec << endl;
	if ( mem[exeaddr][1] != 2 ) {
		if ( mnemonic == "LDA"  ||
			 mnemonic == "LDX"  ||
			 mnemonic == "LDL"  ||
			 mnemonic == "ADD"  ||
			 mnemonic == "SUB"  ||
			 mnemonic == "MUL"  ||
			 mnemonic == "DIV"  ||
			 mnemonic == "COMP" ||
			 mnemonic == "TIX"  ||
			 mnemonic == "AND"  ||
			 mnemonic == "OR"     ) {
			if ( mem[exeaddr+3][1] == -1) {
				mem[exeaddr+3][1] = 1;
			}
			if ( mem[effaddr][0] == -1 ) {
				mem[effaddr][0] = 0;
			}
			mem[effaddr][1] = 2;
			mem[effaddr][2] = 0;
			if ( get_label(effaddr) == -1 ) {
				set_symtab(effaddr,vcounter,1);
				vcounter++;
			}
		}
		if ( mnemonic == "STA"  ||
			 mnemonic == "STX"  ||
			 mnemonic == "STL"  ||
			 mnemonic == "STSW"   ) {
			if ( mem[exeaddr+3][1] == -1) {
				mem[exeaddr+3][1] = 1;
			}
			if ( mem[effaddr][0] == -1 ) {
				mem[effaddr][0] = 0;
			}
			mem[effaddr][1] = 2;
			mem[effaddr][2] = 0;
			if ( get_label(effaddr) == -1 ) {
				set_symtab(effaddr,vcounter,1);
				set_flag(effaddr);
				vcounter++;
			}
		}
		if ( mnemonic == "JEQ"  ||
			 mnemonic == "JGT"  ||
			 mnemonic == "JLT"  ||
			 mnemonic == "JSUB" ||
			 mnemonic == "J"      ) {
			if ( mem[exeaddr+3][1] == -1) {
				mem[exeaddr+3][1] = 1;
			}
			if ( get_label(effaddr) == -1 ) {
				set_symtab(effaddr,icounter,3);
				icounter++;
			}
			if ( mem[effaddr][1] == -1 && mem[effaddr][3] == 0) {
				mem[effaddr][1] = 1;
				pass2(effaddr);
			}
		}
		if ( mnemonic == "LDCH" ) {
			if ( mem[exeaddr+3][1] == -1) {
				mem[exeaddr+3][1] = 1;
			}
			if ( mem[effaddr][0] == -1 ) {
				mem[effaddr][0] = 0;
			}
			mem[effaddr][1] = 2;
			mem[effaddr][2] = 0;
			if ( get_label(effaddr) == -1 || get_status(effaddr) == 1) {
				set_symtab(effaddr,vcounter,2);
				vcounter++;
			}
		}
		if ( mnemonic == "STCH" ) {
			if ( mem[exeaddr+3][1] == -1) {
				mem[exeaddr+3][1] = 1;
			}
			if ( mem[effaddr][0] == -1 ) {
				mem[effaddr][0] = 0;
			}
			mem[effaddr][1] = 2;
			mem[effaddr][2] = 0;
			if ( get_label(effaddr) == -1 || get_status(effaddr) == 1) {
				set_symtab(effaddr,vcounter,2);
				set_flag(effaddr);
				vcounter++;
			}
		}
		if ( mnemonic == "RD"   ||
			 mnemonic == "WD"   ||
			 mnemonic == "TD"     ) {
			if ( mem[exeaddr+3][1] == -1) {
				mem[exeaddr+3][1] = 1;
			}
			if ( mem[effaddr][0] == -1 ) {
				mem[effaddr][0] = 0;
			}
			mem[effaddr][1] = 2;
			mem[effaddr][2] = 0;
			if ( get_label(effaddr) == -1 ) {
				set_symtab(effaddr,vcounter,2);
				vcounter++;
			}
		}
	}
	if ( mnemonic == "J"    ||
		 mnemonic == "RSUB"   ) {
		 if ( get_label(effaddr) == -1 && mnemonic == "J") {
			set_symtab(effaddr,icounter,3);
			icounter++;
		}
		return;
	}
	if ( mem[exeaddr+3][0] != -1 && mem[exeaddr+3][1] != -1 && mem[exeaddr+3][3] == 0 ) {
		pass2(exeaddr+3);
	}
}

void print () {
	for ( int i = 0; i < 65540; i++ ) {
		if ( mem[i][0] != -1 ) {
			printf("%7X %3d %3d %3d\n",i,mem[i][0],mem[i][1],mem[i][2]);
		}
	}
}

void writeoutput (char * output) {
	ofstream outfile;
	outfile.open(output);
	if ( !outfile.is_open() ) {
		fatalerror("Unable to open output file.");
	}
	outfileopened();
	stringstream stream;
	string label;
	string inst;
	string operand;
	bool flag = true;
	int lastaddress;
	int effaddr;
	inst = "START";
	char fill = ' ';
	outfile << right << setw(4) << setfill('0') << hex << start_address << dec << left << "  " << setw(7) << setfill(fill) << programname << " " << setw(6) << "START" << " " << setw(10) << hex << start_address << dec << endl;
	//printf("%-10s %-6s %-10X\n",programname.c_str(),inst.c_str(),start_address);
	for ( int i = start_address; i < start_address+prog_length; i = getnextaddr(i) ) {
		if ( mem[i][1] == 1) {
			if ( !flag ) {
				outfile << endl;
			}
			if ( mem[i][0] != 76 ) {
				if ( mem[i][2] == 1 ) {
					effaddr = ((mem[i+1][0]&(127))*16*16) + mem[i+2][0];
				} else {
					effaddr = (mem[i+1][0]*16*16) + mem[i+2][0];
				}
				if ( mem[i][2] == 1 ) {
					label = "";
					if ( get_label(i) != -1 ) {
						stream.str(std::string());
						stream << "ins" << get_label(i);
						label = stream.str();
					}
					inst = get_mnemonic(mem[i][0]);
					stream.str(std::string());
					stream << "var" << get_label(effaddr) << ",X";
					operand = stream.str();
					if ( i == exe_address ) label = "FIRST";
					outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << " " << setw(10) << operand << endl;
					//printf("%-10s %-6s %-10s\n", label.c_str(),inst.c_str(),operand.c_str());
				} else {
					label = "";
					if ( get_label(i) != -1 ) {
						stream.str(std::string());
						stream << "ins" << get_label(i);
						label = stream.str();
					}
					inst = get_mnemonic(mem[i][0]);
					stream.str(std::string());
					if ( mem[i][0] == 48 || mem[i][0] == 52 || mem[i][0] == 56 || mem[i][0] == 60 || mem[i][0] == 72 ) {
						stream << "ins" << get_label(effaddr);
					} else {
						stream << "var" << get_label(effaddr);
					}
					operand = stream.str();
					if ( i == exe_address ) label = "FIRST";
					outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << " " << setw(10) << operand << endl;
					//printf("%-10s %-6s %-10s\n", label.c_str(),inst.c_str(),operand.c_str());
				}
			} else {
				label = "";
				inst = "RSUB";
				outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << endl;
				//printf("%-10s %-6s\n\n", label.c_str(),inst.c_str());
			}
			flag = true;
		} else {
			if ( flag ) {
				outfile << endl;
			}
			if ( get_status(i) == 1) {
				if ( get_flag(i) == 0 ) {
					if ( !wordorbyte(i) ) {
						stream.str(std::string());
						stream << "var" << get_label(i);																									//var label
						label = stream.str();
						inst = "WORD";
						outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << " " << setw(10) << getinttoprint(i) << endl;
						//printf("%-10s %-6s %-10d\n", label.c_str(),inst.c_str(),getinttoprint(i));
					} else {
						stream.str(std::string());
						stream << "var" << get_label(i);																									//var label
						label = stream.str();
						inst = "BYTE";
						operand = getbytetoprint(i);
						outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << " " << setw(10) << operand << endl;
					}
				} else {
					stream.str(std::string());
					stream << "var" << get_label(i);																									//var label
					label = stream.str();
					inst = "RESW";
					outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << " " << setw(10) << getnoofresword(i) << endl;
					//printf("%-10s %-6s %-10d\n", label.c_str(),inst.c_str(),getnoofresword(i));
				}
			} else {
				if ( get_flag(i) == 0 ) {
					stream.str(std::string());
					stream << "var" << get_label(i);																									//var label
					label = stream.str();
					inst = "BYTE";
					operand = getbytetoprint(i);
					outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << " " << setw(10) << operand << endl;
					//printf("%-10s %-6s %-10s\n", label.c_str(),inst.c_str(),operand.c_str());
				} else {
					stream.str(std::string());
					stream << "var" << get_label(i);																									//var label
					label = stream.str();
					inst = "RESB";
					outfile << right << setw(4) << setfill('0') << hex << uppercase << i << dec << left << "  " << setw(7) << setfill(fill) << label << " " << setw(6) << inst << " " << setw(10) << getnoofresbyte(i) << endl;
					//printf("%-10s %-6s %-10d\n", label.c_str(),inst.c_str(),getnoofresbyte(i));
				}
			}
			flag = false;
		}
	}
	stream.str(std::string());
	stream << "label" << get_label(exe_address);
	label = "";
	inst = "END";
	operand = stream.str();
	outfile << right << setw(4) << setfill(fill) << "" << left << "  " << setw(7) << label << " " << setw(6) << inst << " " << setw(10) << /*operand*/"FIRST" << endl;
	//printf("%-10s %-6s %-10s\n", label.c_str(),inst.c_str(),operand.c_str());
	outfile.close();
	outfileclosed();
	
}

int getnoofresword ( int addr ) {
	int i = getnextaddr(addr);
	i = i - addr;
	if ( i%3 != 0 ) {
		warningmessage("RESW must be a multiple of 3. Check the input file.");
	}
	i = i/3;
	return i;
}

int getinttoprint ( int addr ) {
	int i = mem[addr][0]*16*16*16*16;
	i += mem[addr+1][0]*16*16;
	i += mem[addr+2][0];
	return i;
}

bool wordorbyte ( int addr ) {
	int i = getnextaddr(addr);
	bool flag = true;
	int z;
	for ( int j = addr; j < i; j++ ) {
		z = mem[j][0];
		if ( !( (z >= 'a' && z <= 'z') || (z >= 'A' && z <= 'Z') || (z >= '0' && z <= '9') ) ) {
			flag = false;
			break;
		}
	}
	return flag;
}

int getnoofresbyte ( int addr ) {
	int i = getnextaddr(addr);
	i = i - addr;
	return i;
}

string getbytetoprint( int addr ) {
	int i = getnextaddr(addr);
	bool flag = true;
	string hex = "";
	int z;
	for ( int j = addr; j < i; j++ ) {
		z = mem[j][0];
		if ( !( (z >= 'a' && z <= 'z') || (z >= 'A' && z <= 'Z') || (z >= '0' && z <= '9') ) ) {
			flag = false;
			break;
		}
	}
	if ( flag ) {
		hex += "C'";
		for ( int j = addr; j < i; j++ ) {
			z = mem[j][0];
			if ( z >= 'a' && z <= 'z' ) {
				hex += z;
			} else if ( z >= 'A' && z <= 'Z' ) {
				hex += z;
			} else if ( z >= '0' && z <= '9' ) {
				hex += z;
			}
		}
		hex +="'";
	} else {
		hex += "X'";
		for ( int j = addr; j < i; j++ ) {
			hex += int_to_hex(mem[j][0]);
		}
		hex +="'";
	}
	return hex;
}

int getnextaddr ( int addr ) {
	int i = addr+1;
	while ( (i < (prog_length+start_address)) && mem[i][1] == -1 ) {
		i++;
	}
	return i;
}
