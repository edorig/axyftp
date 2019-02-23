/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"utils.h"

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


#include"read_init.h"
#include"options_data.h"
#define OPT_BUFSIZ 256

typedef int (*rw_option_func)(int,options_data**);

typedef struct _opt_ver {
  char* ver;
  rw_option_func readfunc;
} opt_ver;


static opt_ver opt_ver_list[]={
  {"\x000\x000\x000",(rw_option_func)NULL},
  {"\x000\x000\x001",(rw_option_func)NULL},
  {(char*)NULL,(rw_option_func)NULL}
};
static int init_done=0;
static int last_ver;

static int read_options_data_entry_0(int,options_data**);
static int read_options_data_entry_1(int,options_data**);

static void init_ver_list(){
  opt_ver_list[0].readfunc=read_options_data_entry_0;
  opt_ver_list[1].readfunc=read_options_data_entry_1;

  last_ver=1;
  init_done=1;
}

/*
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
*/

static int read_field(int fd,char** bp){
  char* buf;
  int alloc;
  int len;
  char* ptr;

  alloc=OPT_BUFSIZ;
  buf=WXmalloc(alloc);
  ptr=buf;

  while((len=read(fd,ptr,1))>0){
    if(*ptr++=='\0'){
      *bp=WXrealloc(buf,ptr-buf);
      return 0;
    }
    if((ptr-buf)>=alloc){
      buf=WXrealloc(buf,alloc+=OPT_BUFSIZ);
    }
  }
  WXfree(buf);
  return 7;
}

void empty_options_data(options_data *opt){
  if(opt==NULL)return;
  WXfree(opt->anonpass);
  WXfree(opt->maxwidth);
  WXfree(opt->progress_treshold);
  WXfree(opt->redial);
  WXfree(opt->delay);

  opt->anonpass=strdup(anonymous_password);
  opt->deletions='\1';
  opt->recurse_del='\0';

  opt->show_session='\1';
  opt->columnadjustment='\1';
  opt->maxwidth=strdup("30");
  opt->show_progress='\1';
  opt->progress_treshold=strdup("1");

  opt->xferbeep='\1';

  opt->redial=strdup("10");
  opt->delay=strdup("10");
}

void destroy_options_data(options_data *opt){
  if(opt==NULL)return;
  WXfree(opt->anonpass);
  WXfree(opt->maxwidth);
  WXfree(opt->progress_treshold);
  WXfree(opt->redial);
  WXfree(opt->delay);
  WXfree((char*)opt);
}

options_data* create_options_data(options_data* orig){
  options_data* opt;
  opt=(options_data*)WXmalloc(sizeof(options_data));
  opt->anonpass=NULL;
  opt->deletions='\0';
  opt->recurse_del='\0';

  opt->show_session='\0';
  opt->columnadjustment='\0';
  opt->maxwidth=NULL;
  opt->show_progress='\0';
  opt->progress_treshold=NULL;

  opt->xferbeep='\0';

  opt->redial=NULL;
  opt->delay=NULL;

  if(orig!=NULL){
    opt->anonpass=strdup(orig->anonpass);
    opt->deletions=orig->deletions;
    opt->recurse_del=orig->recurse_del;

    opt->show_session=orig->show_session;
    opt->columnadjustment=orig->columnadjustment;
    opt->maxwidth=strdup(orig->maxwidth);
    opt->show_progress=orig->show_progress;
    opt->progress_treshold=strdup(orig->progress_treshold);
    
    opt->xferbeep=orig->xferbeep;

    opt->redial=strdup(orig->redial);
    opt->delay=strdup(orig->delay);
  }

  return opt;
}

static int read_options_data_entry_1(int fd,options_data** top){
  int len;
  options_data* sd;

  sd=*top=create_options_data(NULL);
  
  if((len=read_field(fd,&sd->anonpass))!=0){
    destroy_options_data(sd);
    *top=NULL;
    return (len==7)?1:(len-1);
  }
  if((len=read(fd,&sd->deletions,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
  if((len=read(fd,&sd->recurse_del,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }

  if((len=read(fd,&sd->show_session,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
  if((len=read(fd,&sd->columnadjustment,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
  if((len=read_field(fd,&sd->maxwidth))!=0){
    destroy_options_data(sd);
    *top=NULL;
    return (len==7)?1:(len-1);
  }
  if((len=read(fd,&sd->show_progress,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
  if((len=read_field(fd,&sd->progress_treshold))!=0){
    destroy_options_data(sd);
    *top=NULL;
    return (len==7)?1:(len-1);
  }

  if((len=read(fd,&sd->xferbeep,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
 
  if((len=read_field(fd,&sd->redial))!=0){
    destroy_options_data(sd);
    *top=NULL;
    return (len==7)?1:(len-1);
  }
  if((len=read_field(fd,&sd->delay))!=0){
    destroy_options_data(sd);
    *top=NULL;
    return (len==7)?1:(len-1);
  }

  return 0;
}

static int read_options_data_entry_0(int fd,options_data** top){
  int len;
  options_data* sd;

  sd=*top=create_options_data(NULL);
  
  if((len=read_field(fd,&sd->anonpass))!=0){
    destroy_options_data(sd);
    *top=NULL;
    return (len==7)?1:(len-1);
  }
  if((len=read(fd,&sd->deletions,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
  if((len=read(fd,&sd->columnadjustment,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
  if((len=read_field(fd,&sd->maxwidth))!=0){
    destroy_options_data(sd);
    *top=NULL;
    return (len==7)?1:(len-1);
  }
  if((len=read(fd,&sd->xferbeep,1))<0){
    destroy_options_data(sd);
    *top=NULL;
    return 5;
  }
  sd->recurse_del='\0';
  sd->show_session='\1';
  sd->show_progress='\1';
  sd->progress_treshold=strdup("1");
  sd->redial=strdup("10");
  sd->delay=strdup("10");
  return 0;
}

static int find_ver_index(char* buf,char* file){
  int index;

  if(!init_done)init_ver_list();

  for(index=0;opt_ver_list[index].ver!=NULL;index++){
    if(memcmp(buf,opt_ver_list[index].ver,OPTIONS_DATA_VERSION_LENGTH+1)==0)
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


int read_options_data(char* file,options_data** top){
  int fd,len;
  char buf[4];
  int index=-1;

  if((fd=open(file,O_RDONLY))<0){
    fprintf(stderr,"Cannot open options data file %s for reading\n",file);
    return 1;
  }

  if((len=read(fd,buf,OPTIONS_DATA_VERSION_LENGTH+1))
      < OPTIONS_DATA_VERSION_LENGTH+1){
    fprintf(stderr,"Cannot read the version of options data file %s\n",file);
    close(fd);
    return 2;
  }

  if((index=find_ver_index(buf,file))==-1){
    fprintf(stderr,"Unknown version of options data file %s\n",file);
    close(fd);
    return 3;
  }

  len=(*opt_ver_list[index].readfunc)(fd,top);
  return len-1;
}

int write_options_data(char* file,options_data* opt){
  int fd;

  if((fd=open(file,O_WRONLY|O_CREAT|O_TRUNC,0600))<0){
    fprintf(stderr,"Cannot open options data file %s for writing\n",file);
    return 1;
  }

  if(!init_done)init_ver_list();
  if(write(fd,opt_ver_list[last_ver].ver,OPTIONS_DATA_VERSION_LENGTH+1)<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }

  if(write(fd,opt->anonpass,strlen(opt->anonpass)+1)<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }
  if(write(fd,&opt->deletions,sizeof(opt->deletions))<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }
  if(write(fd,&opt->recurse_del,sizeof(opt->recurse_del))<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }

  if(write(fd,&opt->show_session,sizeof(opt->show_session))<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }
  if(write(fd,&opt->columnadjustment,sizeof(opt->columnadjustment))<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }
  if(write(fd,opt->maxwidth,strlen(opt->maxwidth)+1)<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }
  if(write(fd,&opt->show_progress,sizeof(opt->show_progress))<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }
  if(write(fd,opt->progress_treshold,strlen(opt->progress_treshold)+1)<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }

  if(write(fd,&opt->xferbeep,sizeof(opt->xferbeep))<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }

  if(write(fd,opt->redial,strlen(opt->redial)+1)<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }
  if(write(fd,opt->delay,strlen(opt->delay)+1)<0){
    fprintf(stderr,"Cannot write to options data file %s\n",file);
    close(fd);
    return 2;
  }

  close(fd);
  return 0;
}

