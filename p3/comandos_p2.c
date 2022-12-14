#include <stdio.h>
#include <stdlib.h>
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
#include "ayudaP2.c"
#include "list.h"

#define mem_type_malloc 11
#define mem_type_shared 12
#define mem_type_mmap 13

struct mem_celda{
   void *mem_ptr;
   size_t size;
   char *time;
   int type;
   int info;
   char *fich;
};
list mem_l;

int recurse(char *tokens[]){
   Recursiva(atoi(tokens[0]));
   return 0;
}

int io(char *tokens[]){
   
   if(tokens[0]==NULL || tokens[1]==NULL || tokens[2]==NULL || tokens[3]==NULL){
      printf("i-o_Error: %s \n",strerror(errno));
   }else if(strcmp(tokens[0],"read")==0){
      do_I_O_read(tokens+1);
   
   }else if(strcmp(tokens[0],"write")==0){
      if(strcmp(tokens[1],"-o")==0){
         ssize_t size;
         void *p =(void *) strtoul(tokens[3], NULL, 16);
         
         if((size=EscribirFichero(tokens[2],p,atoi(tokens[4]),1))==-1)
            printf("i-o_Error < %s>: %s\n",tokens[2],strerror(errno));
         
         else
            printf("Escritos %ld bytes en %s desde %p\n",size,tokens[2],p);
         
      }else {
         ssize_t size;
         void *p =(void *) strtoul(tokens[2], NULL, 16);
         
         if((size=EscribirFichero(tokens[1],p,atoi(tokens[3]),0))==-1)
            printf("i-o_Error < %s>: %s\n",tokens[1],strerror(errno));
            
         else
            printf("Escritos %ld bytes en %s desde %p\n",size,tokens[1],p);
      
      }
   
   }else printf("i-o_Error: %s\n",strerror(errno));
   
   
   return 0;
}

void listar(list l){
   pos p;
   printf("******Lista de bloques asignados para el proceso %d\n",getpid());
   
   for(p=first(l); !at_end(l,p);p=next(l,p)){
       struct mem_celda *g=get(l,p);
       printf("\t%p\t\t%ld\t%s",g->mem_ptr,g->size,g->time);

       if(g->type==mem_type_malloc)
          printf(" malloc\n");
       else if(g->type==mem_type_shared)
          printf(" shared (key %d)\n",g->info); 
       else if(g->type==mem_type_mmap)
          printf(" (%s)  (descriptor %d)\n",g->fich,g->info);        
          
   }
}

int memdump(char *tokens[]){
   
   if(tokens[0]== NULL){
      printf("Memdump_Error: %s\n",strerror(errno));
   }else {
      char *pointer=strdup(tokens[0]);
      unsigned long pos=strtoul(tokens[0],NULL,16);
      unsigned char *pc= (unsigned char *) pos;
      size_t i,cont=atoi(tokens[1]);
      int print_aux=20;
      printf("Volcando %ld bytes desde la direccion %s\n",cont,pointer);
      
      for(i=0;i<cont;i++){
         if(i%print_aux==0){ 
            int j=0;
            puts("");
            for(j=0;j<print_aux;j++){
               printf("%2c  ",pc[i+j]);
            }
            puts("");
            for(j=0;j<print_aux;j++){
               printf("%01x  ", pc[i+j]);
            }
         }      
      } 
      
      puts("");
   }
   
   return 0;
}

int memfill(char *tokens[]){
   if(tokens[0]==NULL){
      printf("Memfill_Error: %s\n",strerror(errno));
   }else{
      char *p=strdup(tokens[0]);
      size_t size;
      unsigned char a;
      
      if(tokens[1]==NULL)
         size=128;
      else
         size= atoi(tokens[1]);
      if(tokens[2]==NULL)   
         a=65;
      else   
         a=strtoul(tokens[2],NULL,16);
      
      if(LlenarMemoria(tokens[0],size,a))
         printf("Llenando %ld bytes de memoria con el byte %c(%02x) a partir de la direccion %s\n",size,a,a,p);
      else 
         printf("Memfill_Error: %s\n",strerror(errno));
   }
   
   return 0;
}

int deallocate(char *tokens[]){
   struct mem_celda *h;
   pos p;
   
   if(strcmp(tokens[0],"-malloc")==0){
      size_t size=atoi(tokens[1]);
      
      for(p=first(mem_l);!at_end(mem_l,p);p=next(mem_l,p)){
          h=get(mem_l,p);
   
          if((h->size==size) && (h->type==mem_type_malloc)){
             h=removeList(mem_l,h);
             free(h->mem_ptr);
             free(h);
             return 0;
          }
      }
      printf("Deallocate_Error < %ld bytes>: %s\n",size,strerror(errno));
   
   } else if(strcmp(tokens[0],"-shared")==0){
      int key=atoi(tokens[1]);
      
      for(p=first(mem_l);!at_end(mem_l,p);p=next(mem_l,p)){
          h=get(mem_l,p);
   
          if((h->info==key) && (h->type==mem_type_shared)){
             h=removeList(mem_l,h);             
             do_DeallocateDelkey (tokens[1]);            
             free(h);
             return 0;
          }
      }

      printf("Deallocate_Error < key %d>: %s\n",key,strerror(errno)); 
   }else if(strcmp(tokens[0],"-delkey")==0){
      int id,key=atoi(tokens[1]);
      struct shmid_ds s;
      id=shmget(key,0,0);
      if(shmctl (id,IPC_RMID,&s)==-1)
         printf("Deallocate_Error: %s\n",strerror(errno));
   }else if(strcmp(tokens[0],"-mmap")==0){
      if(tokens[1]==NULL)
         printf("Deallocate_Error: %s\n",strerror(errno));
      else{
         
         for(p=first(mem_l);!at_end(mem_l,p);p=next(mem_l,p)){
            h=get(mem_l,p);
          
            if(h->type==mem_type_mmap){
                if(strcmp(tokens[1],h->fich)==0){
                   if(munmap(h->mem_ptr,h->size)==0){
                      h=removeList(mem_l,h);
                      free(h);                  
                   }else
                      printf("Deallocate_Error: %s\n",strerror(errno));
                }
            }
         }
      }      
   }else {
      for(p=first(mem_l);!at_end(mem_l,p);p=next(mem_l,p)){
         h=get(mem_l,p);
         if(strcmp(h->mem_ptr,tokens[0])){
            char out[90];
            switch(h->type){
               case mem_type_malloc:
                  free(h->mem_ptr);
                  break;
               case mem_type_shared:
                  sprintf(out,"%d",h->info);
                  do_DeallocateDelkey (out);            
                  break;
               case mem_type_mmap:
                  if(munmap(h->mem_ptr,h->size)!=0){
                     printf("Deallocate_Error: %s\n",strerror(errno)); 
                     return 0;
                  }     
                  break;   
            }
            h=removeList(mem_l,h);
            free(h);
         }    
      }   
   }
      
   return 0;
}

void allocate_mmap_aux(char * fichero, int protection,char *time)
{
    void *p;
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    
    if (protection&PROT_WRITE)
          modo=O_RDWR;
      
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
          return;
    
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED){
          printf("Allocate_Error: %s\n",strerror(errno));
          return;
    }else {      
       printf("fichero %s mapeado en %p\n",fichero,p);
       struct mem_celda *h=malloc(sizeof (struct mem_celda));
       h->mem_ptr=p;
       h->size=s.st_size;
       h->time=strdup(time);
       h->type=mem_type_mmap;
       h->info=df;
       h->fich=fichero;
       insert(mem_l,h);
       }
    return;
}

int allocate(char *tokens[]){
   struct mem_celda *h;
   time_t t;
   struct tm *tm;
   char fechayhora[100],*aux[2];
   
   t=time(NULL);
   tm=localtime(&t);
   
   if(strcmp(tokens[0],"-malloc")==0){
      h=malloc(sizeof (struct mem_celda));
      size_t size= atoi(tokens[1]);
      h->mem_ptr=malloc(size);
      
      if(h->mem_ptr!=NULL){
         h->size=size;
         h->type=mem_type_malloc;
         strftime(fechayhora,100,"%b  %d  %H:%M",tm);
         h->time=strdup(fechayhora);
         insert(mem_l,h);
         printf("Asignados %ld bytes en %p\n",size,h->mem_ptr);
      }else
         printf("Allocate_Error: %s\n",strerror(errno));
   } else if(strcmp(tokens[0],"-createshared")==0){
         if(tokens[1]==NULL || tokens[2]==NULL){
            printf("Allocate_Error: %s\n",strerror(errno));
            return 0;
         }   
         h=malloc(sizeof (struct mem_celda));
         h->size=(size_t) atoi(tokens[2]);
         h->info=atoi(tokens[1]);
         h->type=mem_type_shared;
         strftime(fechayhora,100,"%b  %d  %H:%M",tm);
         h->time=strdup(fechayhora);
         aux[0]=strdup(tokens[1]);
         aux[1]=strdup(tokens[2]);
         
   	 h->mem_ptr=do_AllocateCreateshared(aux);
   	 
   	 if(h->mem_ptr!=NULL)
   	    insert(mem_l,h);
   
   } else if(strcmp(tokens[0],"-shared")==0){ 
      int id,key=atoi(tokens[1]),flags=0777;
      void *d;
      size_t size=0;
      struct shmid_ds s;
    
      if(tokens[1]==NULL){
         printf("Allocate_Error: %s\n",strerror(errno));
         return 0;
      }else{
         h=malloc(sizeof (struct mem_celda));
         id=shmget(key,size,0);          
         if((d=shmat(id,NULL,flags))!=-1){
            shmctl (id,IPC_STAT,&s);
            h->size=s.shm_segsz;
            h->mem_ptr=d;
            h->info=key;
            h->type=mem_type_shared;
            strftime(fechayhora,100,"%b  %d  %H:%M",tm);
            h->time=strdup(fechayhora);
            printf("Memoria compartida de clave %d en %p (%ld)\n",key,d,h->size);
            insert(mem_l,h);
         }else
            printf("Allocate_Error: %s\n",strerror(errno));
      }
   }else if(strcmp(tokens[0],"-mmap")==0){
      if(tokens[1]==NULL || tokens[2]==NULL)
          printf("Allocate_Error: %s\n",strerror(errno));
      else {
         char *fich=strdup(tokens[1]);
         int permisos=atoi(tokens[2]);
         strftime(fechayhora,100,"%b  %d  %H:%M",tm);
         allocate_mmap_aux(fich, permisos,fechayhora);
      }
        
   } else printf("Allocate_Error: %s\n",strerror(errno));
  
   return 0;
}

int valGl1,valGl2,valGl3;

int memory_cmd(char *tokens[]){
   int static valSt1,valSt2,valSt3;
   int valLo1,valLo2,valLo3;

   if(tokens[0]==NULL || (strcmp(tokens[0],"-all")==0)){
      printf("Variables locales:\t%p,\t%p,\t%p\n",&valLo1,&valLo2,&valLo3);
      printf("Variables globales:\t%p,\t%p,\t%p\n",&valGl1,&valGl2,&valGl3);
      printf("Variables estaticas:\t%p,\t%p,\t%p\n",&valSt1,&valSt2,&valSt3);
      printf("Funciones programa:\t%p,\t%p,\t%p\n",memfill,deallocate,allocate);
      printf("Funciones libreria:\t%p,\t%p,\t%p\n",strcmp,strdup,localtime);
      listar(mem_l);
   }else if(strcmp(tokens[0],"-funcs")==0){
      printf("Funciones programa:\t%p,\t%p,\t%p\n",memfill,deallocate,allocate);
      printf("Funciones libreria:\t%p,\t%p,\t%p\n",strcmp,strdup,localtime);
   }else if(strcmp(tokens[0],"-vars")==0){
      printf("Variables locales:\t%p,\t%p,\t%p\n",&valLo1,&valLo2,&valLo3);
      printf("Variables globales:\t%p,\t%p,\t%p\n",&valGl1,&valGl2,&valGl3);
      printf("Variables estaticas:\t%p,\t%p,\t%p\n",&valSt1,&valSt2,&valSt3);
   }else if(strcmp(tokens[0],"-blocks")==0){
      listar(mem_l);
   }else if(strcmp(tokens[0],"-pmap")==0){
      Do_pmap();
   }else printf("Memory_Error: %s\n",strerror(errno));
   return 0;
}
