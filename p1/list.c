#include <stdlib.h>

struct Nodo{
   void *data;//Comodin comodin
   struct Nodo *next;
};

typedef struct Nodo * list;

#include "list.h"

list create_list(list l){
   struct Nodo *node=malloc(sizeof(struct Nodo));
   node->next=NULL;
   node->data=NULL;
   return node;
}

void insert(list l, void *data){
   struct Nodo *pos=l;
   
   while(pos->next != NULL)
      pos=pos->next;
      
   pos->next=malloc(sizeof(struct Nodo));
   pos->next->next=NULL;
   pos->next->data=data;    
}


pos first(list l){
   return l;
}

pos next(list l,pos p){
   if(p->next != NULL)
      return p->next;
   return p;   
}

int at_end(list l,pos p){
   return p->next==NULL;
}

void *get(list l,pos p){
    if(p->next !=NULL)
        return p->next->data;
    return NULL;    
}

