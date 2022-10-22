#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h" 
#include "comandos_p0.c"
#include "comandos_p1.c"

int autores(char *tokens[]);
int pid(char *tokens[]);
int fin(char *tokens[]);
int fecha(char *tokens[]);
int carpeta(char *tokens[]);
int infosis(char *tokens[]);
int proc(char *tokens[],list histor);
int ayuda(char *tokens[]);
int create(char *tokens[]);
int delete(char *tokens[]);
int stat_cmd(char *toknes[]);
int deltree(char *tokens[]);
int list_cmd(char *tokens[]);

struct celda{
   char *cmd_histo;
};


int Split_String(char * cadena, char * trozos[]){ 
   int i=1;
   if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
     return 0;
   while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
     i++;
   return i;
}

int process_comando(char *tokens[],list histor){
   char *tok[MAX_TOKENS];
   int h,j=0;
   pos p;
   
   if(strcmp(tokens[0],"hist")==0){
      if(tokens[1]==NULL){
         for(p=first(histor) ; !at_end(histor,p) ; p=next(histor,p)){
            struct celda *t=get(histor,p);
            printf("%d) %s",j++,t->cmd_histo);
            }
         puts("");   
         }
      else if(strcmp(tokens[1],"-c")==0){
         return Vaciar_List;
      }else { 
         h=-atoi(tokens[1]);
         for(p=first(histor) ; !at_end(histor,p) ; p=next(histor,p)){
            struct celda *t=get(histor,p);
            
            printf("%d) %s",j++,t->cmd_histo);
            if(j==h)  {
                  puts("");                
                  return 0; 
               }     
         }
         puts("");   
       }
              
   }else if(strcmp(tokens[0],"comando")==0){
           h=atoi(tokens[1]);
           
           for(p=first(histor) ; !at_end(histor,p) ; p=next(histor,p)){
           if(j==h){
              struct celda *t=get(histor,p);
              printf("*) %s\n",t->cmd_histo);
              Split_String(t->cmd_histo,tok);
              proc(tok,histor);
                   }
           j++;
           }
         } 
   return 0;       
}

struct cmd{
   char *cmd_name;
   int (*cmd_fun) (char *tokens[]);
} cmds[] = {
   {"autores",autores},
   {"pid",pid},
   {"carpeta",carpeta},
   {"fecha",fecha},
   {"infosis",infosis},
   {"fin",fin},
   {"salir",fin},
   {"bye",fin},
   {"ayuda",ayuda},
   {"create",create},
   {"delete",delete},
   {"deltree",deltree},
   {"stat",stat_cmd},
   {"list",list_cmd},
   {NULL,NULL}
};

int ayuda(char *tokens[]){
   int i;
     
   if(tokens[0]==NULL){
       puts("Lista de todos los comandos disponibles:");
       for(i= 0 ; cmds[i].cmd_name!= NULL ; i++)
          printf("%s ",cmds[i].cmd_name);   
       printf("comando hist\n");//Añadido manualmente ya que no se encuentran en la lista de comandos
   }else {
      char *out; 
      if(strcmp(tokens[0],"ayuda")==0){
          out="ayuda[cmd]\tMuestra ayuda sobre los comandos\n\tcmd:Informacion sobre el comando 'cmd'";
      }else if(strcmp(tokens[0],"autores")==0){
          out="autores [-n|-l]\tMuestra el login del usuario y/o el nombre del autor.";
      }else if(strcmp(tokens[0],"pid")==0){
      	  out="pid [-p]\tMuestra el pid del shell o de su proceso padre.";
      }else if(strcmp(tokens[0],"carpeta")==0){	  
         out="carpeta [dir]\tMuestra el directorio actual del shell o lo cambia a 'dir'.";
      }else if(strcmp(tokens[0],"fecha")==0){
         out="fecha [-d|-h]\tMuestra la fecha y/o hora actual.";
      }else if(strcmp(tokens[0],"infosis")==0){  
         out="infosis\tMuestra informacion de la maquina donde corre el shell.";
      }else if(strcmp(tokens[0],"fin")==0){
          out="fin\tTermina la ejecucion del shell.";
      }else if(strcmp(tokens[0],"salir")==0){
          out="salir\tTermina la ejecucion del shell.";    
      }else if(strcmp(tokens[0],"bye")==0){
          out="bye\tTermina la ejecucion del shell."; 
      }else if(strcmp(tokens[0],"create")==0){
          out="create [-f] name\tSe crea un directorio o un fichero(-f) de nombre 'name'.";
      }else if(strcmp(tokens[0],"delete")==0){
          out="delete [n1 n2 ...]\tBorra ficheros o directorios vacios.";  
      }else if(strcmp(tokens[0],"deltree")==0){
          out="deltree [n1 n2 ...]\tBorra ficheros o directorios no vacios recursivamente.";
      }else if(strcmp(tokens[0],"stat")==0){  
          out="stat [-long][-link][-acc] n1 n2 \tMuestra información de los archivos indicados.\n\t-long:Listado largo, con mayor informacion.\n\t-acc: Combinado con '-long' nos muestra el acesstime.\n\t-link: Si se hace refencia a un enlace simbolico, el path contenido.";
      }else if(strcmp(tokens[0],"list")==0){      
          out="list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2\tLista el contenido de directorios\n\t-hid:Incluye los ficheros ocultos.\n\t-reca: Recursividad (antes).\n\t-recb: Recursividad (despues).\n\tResto de paramatros como stat."; 
      }else if(strcmp(tokens[0],"hist")==0){
         out="hist [-c|-N]\tMuestra el historico de comandos del shell.\n\t-c: Limpia el historial.\n\t-N: printea el comando numero 'N' del histórico.";
      }else if(strcmp(tokens[0],"comando")==0){
         out="comando N\tRepito el comando numero 'N' del histórico de comandos.";                
      }else out="No se ha encontrado informacion sobre el comando.";
   
      printf("%s\n",out);
   }

   return 0;
}

int process(char *tokens[]){
   int i;
   
   for(i= 0 ; cmds[i].cmd_name!= NULL ; i++){
       if(strcmp(tokens[0],cmds[i].cmd_name)== 0){
          return cmds[i].cmd_fun(tokens + 1);
       }
   }
   printf("Comando no encontrado\n");
   return 0;
}

int proc(char *tokens[],list histor){
   if(strcmp(tokens[0],"comando")==0 || strcmp(tokens[0],"hist")==0)
        return process_comando(tokens, histor); 
     else    
        return process(tokens);
}
