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
#include "ayudaP1.c"

#define MAX_SIZE_PATH 1024
#define _GNU_SOURCE

int delete(char *tokens[]){
    int i=0;
    
    while(tokens[i]!=NULL){
       if(remove(tokens[i])!=0)
           printf("Delete_Error < %s >: %s\n",tokens[i],strerror(errno));
       
       i++;
    }
    return 0;
}

int create(char *tokens[]){
    
    if(strcmp(tokens[0],"-f")==0){
        if(fopen(tokens[1],"r")!=NULL)
           printf("Create_Error < %s >: %s\n",tokens[1],strerror(errno));
        else{
           if(fopen(tokens[1],"w+")==NULL)
              printf("Create_Error < %s >: %s\n",tokens[1],strerror(errno));   
        }  
    }else{
       if(mkdir(tokens[0], S_IRWXU)==-1)
          printf("Create_Error < %s >: %s\n",tokens[0],strerror(errno));
       }
       
    //S_IRWXU implica que el propietario tiene permisos de lectura, escritura y ejecución 
    //en el directorio.w+ crea un fichero de lectura y escritura.Si se usa r y no existe el 
    //archivo, se devolverá un error (NULL).
    
    return 0;
}

int stat_aux(char *token,int long1,int acc,int link,int list){
   struct tm *tm;
   struct stat sb;
   char fechayhora[100],permisos[12],*buf,*nombre;
   int max;
   
   if(lstat(token, &sb) == -1){
      printf("Stat_Error < %s >: %s\n",token,strerror(errno));
   }else{         
      if(acc==1)
         tm= gmtime(&sb.st_atime);//Fecha último acceso
      else
         tm= gmtime(&sb.st_mtime);//Fecha última modificación   
      
      if(list==1){
         nombre= basename(token);
      }else 
         nombre=token;
           
      if(long1==1){
         ConvierteModo(sb.st_mode, permisos); 
         strftime(fechayhora,100,"%Y/%m/%d-%H:%M",tm);
         struct passwd *propietario= getpwuid(sb.st_uid);
         struct group *grp=getgrgid(sb.st_gid);
         
         printf( "%s %ld ( %ld) %s ", fechayhora,sb.st_nlink,sb.st_ino,propietario->pw_name); 
         printf(" %s %s %lu %s",grp->gr_name,permisos,sb.st_size,nombre);
         
         if(link==1 && ((sb.st_mode & S_IFMT) == S_IFLNK)){
            buf=malloc(MAX_SIZE_PATH);
            
            if((max = readlink(token,buf,MAX_SIZE_PATH)) == -1)
               printf("\nStat_Error < %s >: %s\n",nombre,strerror(errno));
            else{   
               buf[max]='\0';
               
               printf(" ->  %s",buf);
               
               } 
            
            free(buf);     
         }
         
         puts(""); 
      }else
         printf( "  %lu \t\t%s\n", sb.st_size,nombre); 
   }  

   return 0; 
}

int stat_cmd(char *tokens[]){
   int long1=0,acc=0,link=0,i=0,aux;
   
   while(tokens[i]!=NULL){
      if(strcmp(tokens[i],"-long")==0)
         long1=1;
         
      if(strcmp(tokens[i],"-acc")==0)
         acc=1;
         
      if(strcmp(tokens[i],"-link")==0)   
         link=1;
      i++;
   }
   aux=long1+acc+link;
   
   while(tokens[aux]!= NULL){
      stat_aux(tokens[aux],long1,acc,link,0);
      aux++;
   }
   return 0;
}

int remove_aux(char *path){
   struct stat st;
   if(lstat(path, &st) == -1){
      printf("Deltree_Error < %s >: %s\n",path,strerror(errno));
      return 0;
   }
   if((st.st_mode & S_IFMT) == S_IFDIR){
      DIR *d = opendir(path);
      struct dirent *ent;
      
      if(d == NULL){      
         printf("Deltree_Error < %s >: %s\n",path,strerror(errno));
         
         return 0;
      }
      
      while((ent = readdir(d)) != NULL){
      
         char new_path[1024];
         
         if(strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name,"..")==0)
            continue;
            
         sprintf(new_path, "%s/%s",path,ent->d_name);
         remove_aux(new_path);   
      }
      closedir(d);   
   }
   if(remove(path)==-1)
      printf("Deltree_Error < %s >: %s\n",path,strerror(errno));
   
   return 0;
}

int deltree(char *tokens[]){
   int i=0;
   while(tokens[i]!=NULL){
       remove_aux(tokens[i]);
       i++;
   }
   return 0;
}

int list_aux(char *path, int long1,int acc,int recurA,int recurB,int hid,int link){
   struct stat st,sb;

   
   if(lstat(path, &st) == -1){
      printf("List_Error < %s >: %s\n",path,strerror(errno));
      return 0;
   }
   if((st.st_mode & S_IFMT) == S_IFDIR){
      DIR *d;
      struct dirent *ent;
      
      
      if(recurB==1){
         d = opendir(path);
         
         while((ent = readdir(d)) !=NULL){
            char new_path[1024];
            sprintf(new_path, "%s/%s",path,ent->d_name);
         
            if(strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name,"..")==0)
               continue;
 
            if(stat(new_path,&sb) == -1)
               printf("List_Error < %s >: %s\n",path,strerror(errno));
	  
            if(recurB==1){
               if(ent->d_name[0]=='.'){
                  if(hid==1)
                     list_aux(new_path,long1,acc,recurA,recurB,hid,link);   
               }else
                  list_aux(new_path,long1,acc,recurA,recurB,hid,link);  
             }
         }  
        
         printf("************%s\n",path); 
         closedir(d);
      
      }else
         printf("************%s\n",path);
      
      d = opendir(path);
      
      if(d == NULL){      
         printf("List_Error < %s >: %s\n",path,strerror(errno));
         
         return 0;
      }
      
     
      while((ent = readdir(d)) !=NULL){
         char new_path[1024];
         sprintf(new_path, "%s/%s",path,ent->d_name);
         
         if((ent->d_name[0]=='.') && hid==0)
            continue;
 
         if(lstat(new_path,&sb) == -1)
            printf("List_Error < %s >: %s\n",path,strerror(errno));
         
         stat_aux(new_path,long1,acc,link,1);
      }
      
      if(recurA==1){
         d = opendir(path);
     
         while((ent = readdir(d)) != NULL){
      
            char new_path[1024];
         
            if(strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name,"..")==0)
               continue;
 
            sprintf(new_path, "%s/%s",path,ent->d_name);
            if(ent->d_name[0]=='.'){
               if(hid==1)
                  list_aux(new_path,long1,acc,recurA,recurB,hid,link);  
            }else
               list_aux(new_path,long1,acc,recurA,recurB,hid,link);  
              
         }
      closedir(d);   
      }
   }   
   return 0;
}  

int list_cmd(char *tokens[]){
   int long1=0,acc=0,reca=0,recb=0,hid=0,link=0,i=0,aux;
   
   while(tokens[i]!=NULL){
      if(strcmp(tokens[i],"-long")==0)
         long1=1;
         
      if(strcmp(tokens[i],"-acc")==0)
         acc=1;
         
      if(strcmp(tokens[i],"-reca")==0)   
         reca=1;
      
      if(strcmp(tokens[i],"-recb")==0)   
         recb=1;
      
      if(strcmp(tokens[i],"-hid")==0)
         hid=1;
         
      if(strcmp(tokens[i],"-link")==0)
         link=1;   
      i++;
   }
   aux=long1+acc+reca+recb+hid+link;
   
   if(recb==1 && reca==1){
      printf("List_Error: %s\n",strerror(errno));
   }else
      list_aux(tokens[aux],long1,acc,reca,recb,hid,link);
  
   return 0;
}
