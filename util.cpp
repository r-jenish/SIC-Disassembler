#include "util.h"
#include <string>
#include <iostream>
#include <cstdlib>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[1;31m"
#define KGRN  "\x1B[1;32m"
#define KYEL  "\x1B[1;33m"
#define KBLU  "\x1B[1;34m"
#define KMAG  "\x1B[1;35m"
#define KCYN  "\x1B[1;36m"
#define KWHT  "\x1B[1;37m"

void fatalerror(std::string err) {
	std::cout << KRED << "- " << err << std::endl;
	displaylastmessage();
	exit(-1);
}

void warningmessage(std::string war) {
	std::cout << KYEL << "! " << war << std::endl;
}

void infileopened() {
	std::cout << KBLU <<  "+ Input object file successfully opened." << std::endl;
}

void infileclosed() {
	std::cout << KGRN << "+ Input object file successfully closed." << std::endl;
}

void startingdisassembling() {
	std::cout << KBLU << "+ Starting disassembling." << std::endl;
}

void finishdisassembling() {
	std::cout << KGRN << "+ Finished disassembling and output has be written in ouputfile specified." << std::endl;
}

void outfileopened() {
	std::cout << KBLU << "+ Output file successfully opened." << std::endl;
}

void outfileclosed() {
	std::cout << KGRN << "+ Output file successfully closed." << std::endl;
}

void displaylastmessage() {
	std::cout << KCYN << "=======Copyright (c) 2015 Rakholiya Jenish=======" << KNRM << std::endl;
}
