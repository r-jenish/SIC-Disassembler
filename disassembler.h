#ifndef _DISASSEMBLER_H_
#define _DISASSEMBLER_H_
#include <string>

void readinput(char *);
void writeoutput(char *);
void pass2(int);
void print(void);
int getnoofresword(int);
int getinttoprint(int);
int getnoofresbyte(int);
std::string getbytetoprint(int);
int getnextaddr(int);

#endif
