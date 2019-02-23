/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

#include"read_init.h"

#include"session_data.h"
#include"options_data.h"
#include"axyftp.h"

#define INIT_DIR ".axyftp"
#define OLD_INIT_DIR ".wxftp"
#define SESSION_FILE "sessions"
#define OPTIONS_FILE "options"
#define LOG_FILE "log"
FILE* logfile;

char* session_file;
char* options_file;
char* log_file;
char* anonymous_password="user@host.net";

int read_init(){
  char* home;
  char* initdir,*oldinitdir;
  char* buf;
  struct stat statbuf;
  int ret;

  home=getenv("HOME");

  if(home==NULL){
    fprintf(stderr,"Cannot figure out user home directory\n");
    session_file=NULL;
    return 1;
  }

  initdir=WXmalloc(strlen(home)+strlen(INIT_DIR)+2);
  sprintf(initdir,"%s/%s",home,INIT_DIR);

  oldinitdir=WXmalloc(strlen(home)+strlen(INIT_DIR)+2);
  sprintf(oldinitdir,"%s/%s",home,OLD_INIT_DIR);

  if(!stat(oldinitdir,&statbuf) && stat(initdir,&statbuf)){
    rename(oldinitdir,initdir);
  }
  WXfree(oldinitdir);

  if(stat(initdir,&statbuf)){
    if(mkdir(initdir,0755)){
      fprintf(stderr,"Cannot create init directory %s\n",initdir);
      WXfree(initdir);
      session_file=NULL;
      return 2;
    }
  } else {
    if(!S_ISDIR(statbuf.st_mode)){
      fprintf(stderr,"%s is not a directory! Please fix it.\n",initdir);
      WXfree(initdir);
      session_file=NULL;
      return 3;
    }
  }

  log_file=WXmalloc(strlen(initdir)+strlen(LOG_FILE)+2);
  sprintf(log_file,"%s/%s",initdir,LOG_FILE);
  if(!stat(log_file,&statbuf) && statbuf.st_size){
    buf=WXmalloc(strlen(log_file)+6);
    sprintf(buf,"%s.last",log_file);
    (void)rename(log_file,buf);
    WXfree(buf);
  }
  if((logfile=fopen(log_file,"w"))==NULL){
    fprintf(stderr,"Cannot open log file %s for writing\n",log_file);
  }

  session_file=WXmalloc(strlen(initdir)+strlen(SESSION_FILE)+2);
  sprintf(session_file,"%s/%s",initdir,SESSION_FILE);
  appdata.sdata=create_session_data(NULL);
  options_file=WXmalloc(strlen(initdir)+strlen(OPTIONS_FILE)+2);
  sprintf(options_file,"%s/%s",initdir,OPTIONS_FILE);
  
  switch(read_options_data(options_file,&appdata.odata)){
    case 1:
    case 2:
    case 3:
      appdata.odata=create_options_data(NULL);
      empty_options_data(appdata.odata);
      buf=WXmalloc(strlen(options_file)+10);
      sprintf(buf,"%s.bak",options_file);
      rename(options_file,buf);
      WXfree(buf);
      write_options_data(options_file,appdata.odata);
      break;
    case 0:
      ret=0;
      break;
    default:
      ret=1;
      break;
  }
  switch(read_session_data(session_file,appdata.sdata)){
    case 1:
    case 2:
    case 3:
      buf=WXmalloc(strlen(session_file)+10);
      sprintf(buf,"%s.bak",session_file);
      rename(session_file,buf);
      WXfree(buf);
      ret=write_session_data(session_file,appdata.sdata);
      break;
    case 0:
      ret=0;
      break;
    default:
      ret=1;
      break;
  }
  return ret;
}
