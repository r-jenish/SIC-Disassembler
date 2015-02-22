#include "disassembler.h"
#include "convertor.h"
#include "error.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

string programname;
int start_address;
int exe_address;
int length;
int header_record[19];
bool recordflag[2] = {false,false};
int mem[65536][2];						 /*mem[][0] = code and mem[][1] = data*/
int end_record[7];

void clearmem () {
	for ( int i = 0; i < 65536; i++ ) {
		mem[i][0] = -1;
		mem[i][1] = -1;
	}
}

void readinput ( char* input ) {
	clearmem();
	string line;
	string hex = "00";
	ifstream infile;
	infile.open("input.in",ifstream::in);
	while ( getline(infile,line) ) {
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
						length = hex_to_int6(hex);
						recordflag[0] = true;
					} else {
						//fatalerror("MORE THAN ONE HEADER RECORD.");
						cout << "MORE THAN ONE HEADER RECORD." << endl;
						return;
					}
				} else {
					cout << "HEADER RECORD NOT CORRECT." << endl;
					return;
				}
				break;
			case 'T':
				
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
						//fatalerror("MORE THAN ONE END RECORD.");
						cout << "MORE THAN ONE END RECORD." << endl;
						return;
					}
				} else {
					cout << "END RECORD NOT CORRECT." << endl;
					return;
				}
				break;
		}
	}
}
