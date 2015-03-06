#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>

void displaytitle(void);
void fatalerror(std::string);
void warningmessage(std::string);
void displaylastmessage(void);
void infileopened(void);
void infileclosed(void);
void startingdisassembling(void);
void finishdisassembling(void);
void outfileopened(void);
void outfileclosed(void);

#endif
