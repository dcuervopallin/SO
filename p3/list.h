#ifndef __LIST_H__
#define __LIST_H__

typedef struct Nodo *list;
typedef struct Nodo *pos;

list create_list();
void insert(list l, void *d);
pos first(list l);
pos next(list l, pos p);
int at_end(list l, pos p);
void *get(list l, pos p);
pos search(list l,void *d);
void *removeList(list l,void *d);
   
#endif
