#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

void initialise_symtab(void);
void set_symtab(int,int);
void update_symtab(int,int);
int get_label(int);
int get_status(int);

#endif
