//David Cuervo Pallin d.cuervo.pallin@udc.es

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h" 
#include "process_shell.c"

#define MAX_SIZE 256
#define MAX_TOKENS 30
#define Salir_Shell 1
#define Vaciar_List 2

list free_list(list l){
   struct celda *h;
   pos p;
   
   for(p= first(l ) ; !at_end(l,p ) ; p= next(l, p )){
      h= get(l,p);
      free(h->cmd_histo);
      free(h );  
         }
   l= create_list();               
   return l;
}

int main(int argc,char *argv[]){
   char *tokens[MAX_TOKENS],input[MAX_SIZE];
   int end=0;
   list histor;
   histor=create_list();
   mem_l=create_list();
   
   struct celda *h;
   
   while(end!=Salir_Shell){
     printf("-> ");
     fgets(input, MAX_SIZE, stdin);
     h=malloc(sizeof(struct celda));
     h->cmd_histo=strdup(input);
     insert(histor,h);
     Split_String(input,tokens);
     end=proc(tokens, histor);
     if(end==Vaciar_List)
         histor=free_list(histor);
   }
   histor=free_list(histor);
   return 0;
}
