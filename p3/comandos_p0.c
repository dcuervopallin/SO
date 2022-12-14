#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <libgen.h>
#include "list.h"

#define MAX_SIZE 256
#define MAX_TOKENS 30
#define Salir_Shell 1
#define Vaciar_List 2

int autores(char *tokens[]){
   if(tokens[0]==NULL)
       printf("d.cuervo.pallin@udc.es\nDavid Cuervo Pallin\n");
   else if(strcmp(tokens[0],"-l")==0)
       printf("d.cuervo.pallin@udc.es\n");
   else if(strcmp(tokens[0],"-n")==0)
       printf("David Cuervo Pallin\n");    
   else printf("Comando no encontrado\n");

   return 0;
}

int pid(char *tokens[]){
   if(tokens[0]==NULL)
      printf("Proceso %d\n",getpid());
   else if(strcmp(tokens[0],"-p")==0)
      printf("Proceso %d\n",getppid());
   else   
      printf("Comando no encontrado\n");
      
   return 0;
}

int fin(char *tokens[]){return Salir_Shell;}

int fecha(char *tokens[]){
   time_t t;
   struct tm *tm;
   char fechayhora[100];
   
   t=time(NULL);
   tm=localtime(&t);
   
   if(tokens[0]==NULL)
       strftime(fechayhora,100,"%d/%m/%Y\n%H:%M:%S",tm); 
   else if(strcmp(tokens[0],"-h"))
      strftime(fechayhora,100,"%d/%m/%Y",tm); 
   else if(strcmp(tokens[0],"-d"))
      strftime(fechayhora,100,"%H:%M:%S",tm);
   else
      printf("Comando no encontrado\n");
      
   if(fechayhora != NULL)   
      printf("%s\n",fechayhora);      
   
   return 0;
}
int carpeta(char *tokens[]){
   char s[100];
   int z;
   
   if(tokens[0]==NULL)
      printf("%s\n",getcwd(s, 100));
   else {
      z=chdir(tokens[0]);
      if(z<0)
         printf("Carpeta_Error < %s >: %s\n",tokens[0],strerror(errno));
      else
         printf("Cambio satisfatorio, directorio actual: %s\n",getcwd(s, 100));    
   }
      
   return 0;      
}

int infosis(char *tokens[]){
   struct utsname a;
   uname(&a);
   printf("\n\tSysname:    %s\n", a.sysname);
   printf("\tNodename:   %s\n", a.nodename);
   printf("\tRelease:    %s\n", a.release);
   printf("\tVersion:    %s\n", a.version);
   printf("\tMachine:    %s\n", a.machine);
   
   return 0;
}
