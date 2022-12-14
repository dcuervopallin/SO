#include <stdlib.h>
#include <stdio.h>

struct Nodo{
   void *data;//Comodin comodin
   struct Nodo *next;
};

typedef struct Nodo * list;

#include "list.h"

int at_end(list l,pos p){
   return p->next==NULL;
}

list create_list(){
   struct Nodo *node=malloc(sizeof(struct Nodo));
   node->next=NULL;
   node->data=NULL;
   return node;
}

void insert(list l, void *data){
   struct Nodo *pos=l;
   if(!at_end(l, pos)){
      while(pos->next !=NULL)
         pos=pos->next;  
   }
    
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

void *get(list l,pos p){
    if(p->next !=NULL)
        return p->next->data;
    return NULL;    
}

void *removeList(list l,void *d){
   struct Nodo *aux=l,*pos=NULL;
   void *data;
   
   while((aux!=NULL) && (aux->data != d)){
      pos=aux;
      aux=aux->next;
   }
   if(aux==NULL)
      return NULL;
   else if(pos==NULL){
      l=l->next;
      data=aux->data;
      free(aux);
   }else{
      pos->next=aux->next;
      data=aux->data;
      free(aux);
   }
   return data;
      
}
