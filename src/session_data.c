/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"utils.h"
#include"read_init.h"

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include"session_data.h"
#define SD_BUFSIZ 256

typedef int (*rw_entry_func)(int,session_data*);

typedef struct _sd_ver {
  char* ver;
  rw_entry_func readfunc;
} sd_ver;


static sd_ver sd_ver_list[]={
  {"\x000\x000\x000",(rw_entry_func)NULL},
  {"\x000\x000\x001",(rw_entry_func)NULL},
  {(char*)NULL,(rw_entry_func)NULL}
};
static int init_done=0;
static int last_ver;

static int read_session_data_entry_0(int,session_data*);
static int read_session_data_entry_1(int,session_data*);

static void init_ver_list(){
  sd_ver_list[0].readfunc=read_session_data_entry_0;
  sd_ver_list[1].readfunc=read_session_data_entry_1;

  last_ver=1;
  init_done=1;
}

static char rotate(char in){
  if(in<'A')return in;
  if(in<'N')return (in+13);
  if(in<='Z')return (in-13);
  if(in<'a')return in;
  if(in<'n')return (in+13);
  if(in<='z')return (in-13);
  return in;
}

static void rotate_str(char* str){
  while(*str!='\0'){
    *str=rotate(*str);
    str++;
  }
}

void print_session_data(session_data*);

static int read_field(int fd,char** bp){
  char* buf;
  int alloc;
  int len;
  char* ptr;

  alloc=SD_BUFSIZ;
  buf=WXmalloc(alloc);
  ptr=buf;

  while((len=read(fd,ptr,1))>0){
    if(*ptr++=='\0'){
      *bp=WXrealloc(buf,ptr-buf);
      return 0;
    }
    if((ptr-buf)>=alloc){
      buf=WXrealloc(buf,alloc+=SD_BUFSIZ);
    }
  }
  WXfree(buf);
  return 7;
}

void empty_session_data(session_data *sd){
  if(sd==NULL)return;
  WXfree(sd->profile);
  WXfree(sd->host);
  WXfree(sd->user);
  WXfree(sd->pass);
  WXfree(sd->account);
  WXfree(sd->comment);
  WXfree(sd->remdir);
  WXfree(sd->locdir);
  WXfree(sd->initcom);
  WXfree(sd->locmask);
  WXfree(sd->remmask);

  sd->profile=strdup("");
  sd->host=strdup("");
  sd->user=strdup("");
  sd->pass=strdup("");
  sd->account=strdup("");
  sd->comment=strdup("");
  sd->anon='\0';
  sd->save='\0';

  sd->remdir=strdup("");
  sd->locdir=strdup("");
  sd->initcom=strdup("");
  sd->locmask=strdup("");
  sd->remmask=strdup("");

  sd->port=strdup("21");
}

void destroy_session_data(session_data *sd){
  if(sd==NULL)return;
  WXfree(sd->profile);
  WXfree(sd->host);
  WXfree(sd->user);
  WXfree(sd->pass);
  WXfree(sd->account);
  WXfree(sd->comment);
  WXfree(sd->remdir);
  WXfree(sd->locdir);
  WXfree(sd->initcom);
  WXfree(sd->locmask);
  WXfree(sd->remmask);
  WXfree(sd->port);

  destroy_session_data(sd->next);
  WXfree((char*)sd);
}

session_data* create_session_data(session_data* next){
  session_data* sd;
  sd=(session_data*)WXmalloc(sizeof(session_data));
  sd->profile=NULL;
  sd->host=NULL;
  sd->user=NULL;
  sd->pass=NULL;
  sd->account=NULL;
  sd->comment=NULL;
  sd->anon='\0';
  sd->save='\0';
  sd->remdir=NULL;
  sd->locdir=NULL;
  sd->initcom=NULL;
  sd->locmask=NULL;
  sd->remmask=NULL;
  sd->port=NULL;

  sd->next=next;
  return sd;
}

static int read_session_data_entry_0(int fd,session_data* top){
  session_data * sd;
  int len;

  sd=top->next=create_session_data(NULL);
  
  if((len=read_field(fd,&sd->profile))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return (len==7)?1:(len-1);
  }
  if((len=read_field(fd,&sd->host))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->user))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }

  if((len=read_field(fd,&sd->pass))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }

  if((len=read_field(fd,&sd->account))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->comment))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }

  if((len=read(fd,&sd->anon,1))<0){
    destroy_session_data(sd);
    top->next=NULL;
    return 5;
  }
  if((len=read(fd,&sd->save,1))<0){
    destroy_session_data(sd);
    top->next=NULL;
    return 5;
  }

  if((len=read_field(fd,&sd->remdir))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->locdir))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->initcom))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->locmask))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->remmask))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  sd->port=strdup("21");

  if((!sd->save) && sd->anon){
    WXfree(sd->pass);
    sd->pass=WXnewstring(anonymous_password);
  }
  return 0;
}

static int read_session_data_entry_1(int fd,session_data* top){
  session_data * sd;
  int len;

  sd=top->next=create_session_data(NULL);
  
  if((len=read_field(fd,&sd->profile))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return (len==7)?1:(len-1);
  }
  if((len=read_field(fd,&sd->host))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->user))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->pass))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  rotate_str(sd->pass);
  if((len=read_field(fd,&sd->account))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->comment))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }

  if((len=read(fd,&sd->anon,1))<0){
    destroy_session_data(sd);
    top->next=NULL;
    return 5;
  }
  if((len=read(fd,&sd->save,1))<0){
    destroy_session_data(sd);
    top->next=NULL;
    return 5;
  }

  if((len=read_field(fd,&sd->remdir))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->locdir))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->initcom))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->locmask))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->remmask))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((len=read_field(fd,&sd->port))!=0){
    destroy_session_data(sd);
    top->next=NULL;
    return len-1;
  }
  if((!sd->save) && sd->anon){
    WXfree(sd->pass);
    sd->pass=WXnewstring(anonymous_password);
  }
  return 0;
}

static int find_ver_index(char* buf,char* file){
  int index;

  if(!init_done)init_ver_list();

  for(index=0;sd_ver_list[index].ver!=NULL;index++){
    if(memcmp(buf,sd_ver_list[index].ver,SESSION_DATA_VERSION_LENGTH+1)==0)
      break;
  }
  

  if(index!=last_ver){
    char* command;

    command=WXmalloc(2*strlen(file)+20);
    sprintf(command,"/bin/cp -f %s %s.old",file,file);

    (void)system(command);
    WXfree(command);
  }
  if(last_ver<index)return -1;
  else return index;
}


int read_session_data(char* file,session_data* top){
  int fd,len;
  char buf[4];
  int index=-1;

  if((fd=open(file,O_RDONLY))<0){
    fprintf(stderr,"Cannot open session data file %s for reading\n",file);
    return 1;
  }

  if((len=read(fd,buf,SESSION_DATA_VERSION_LENGTH+1))
      < SESSION_DATA_VERSION_LENGTH+1){
    fprintf(stderr,"Cannot read the version of session data file %s\n",file);
    close(fd);
    return 2;
  }

  if((index=find_ver_index(buf,file))==-1){
    fprintf(stderr,"Unknown version of session data file %s\n",file);
    close(fd);
    return 3;
  }

  while((len=(*sd_ver_list[index].readfunc)(fd,top))==0){
    top=top->next;
  }
  return len-1;
}

int write_session_data(char* file,session_data* top){
  int fd;

  if((fd=open(file,O_WRONLY|O_CREAT|O_TRUNC,0600))<0){
    fprintf(stderr,"Cannot open session data file %s for writing\n",file);
    return 1;
  }

  if(!init_done)init_ver_list();
  if(write(fd,sd_ver_list[last_ver].ver,SESSION_DATA_VERSION_LENGTH+1)<0){
    fprintf(stderr,"Cannot write to session data file %s\n",file);
    close(fd);
    return 2;
  }

  while(top->next!=NULL){
    top=top->next;
    if(write(fd,top->profile,strlen(top->profile)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->host,strlen(top->host)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->user,strlen(top->user)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }

    if(top->save){
      rotate_str(top->pass);
      if(write(fd,top->pass,strlen(top->pass)+1)<0){
	fprintf(stderr,"Cannot write to session data file %s\n",file);
	rotate_str(top->pass);
	close(fd);
	return 2;
      }
      rotate_str(top->pass);
    } else {
      if(write(fd,"",1)<0){
	fprintf(stderr,"Cannot write to session data file %s\n",file);
	close(fd);
	return 2;
      }
    }

    if(write(fd,top->account,strlen(top->account)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->comment,strlen(top->comment)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,&top->anon,sizeof(top->anon))<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,&top->save,sizeof(top->save))<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->remdir,strlen(top->remdir)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->locdir,strlen(top->locdir)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->initcom,strlen(top->initcom)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->locmask,strlen(top->locmask)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->remmask,strlen(top->remmask)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
    if(write(fd,top->port,strlen(top->port)+1)<0){
      fprintf(stderr,"Cannot write to session data file %s\n",file);
      close(fd);
      return 2;
    }
  }
  close(fd);
  return 0;
}

void print_session_data(session_data* sd){
  printf("\nProfile: %s|\nHost: %s\nUser: %s\nPassowrd: %s\nAccount: %s\n"
      "Comment: %s\nAnonymous: %s\nSave Passowrd: %s\nRemDir: %s\n"
      "LocDir: %s\nInit Command: %s\nLocMask: %s\nRemMask: %s\nPort: %s\n",
      sd->profile,sd->host,sd->user,sd->pass,sd->account,
      sd->comment,sd->anon?"YES":"NO",sd->save?"YES":"NO",sd->remdir,
      sd->locdir,sd->initcom,sd->locmask,sd->remmask,sd->port);
}
