#include "util.h"
#include <string>
#include <iostream>
#include <cstdlib>

void fatalerror(std::string err) {
	std::cout << "- " << err << std::endl;
	displaylastmessage();
	exit(-1);
}

void warningmessage(std::string war) {
	std::cout << "! " << war << std::endl;
}

void infileopened() {
	std::cout << "+ Object file successfully opened." << std::endl;
}

void infileclosed() {
	std::cout << "+ Object file successfully closed." << std::endl;
}

void startingdisassembling() {
	std::cout << "+ Starting disassembling." << std::endl;
}

void finishdisassembling() {
	std::cout << "+ Finished disassembling and output has be written in ouputfile specified." << std::endl;
}

void outfileopened() {
	std::cout << "+ Output file successfully opened." << std::endl;
}

void outfileclosed() {
	std::cout << "+ Output file successfully closed." << std::endl;
}

void displaylastmessage() {
	std::cout << "=======Copyright (c) 2015 Rakholiya Jenish=======" << std::endl;
}
