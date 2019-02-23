/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<ctype.h>
#include<errno.h>

extern int errno;

#define SIZE 1024

#include"status.h"

#include"utils.h"
#include"ftp.h"
#include"ftp_xfer.h"
#include"dirinfo.h"
#include"functions.h"
#include"read_init.h"
#include"little_dialogs.h"
#include"axyftp.h"

extern void print_fileinfo(fileinfo*);
fileinfo** create_file_table(FILE*);

dirinfo* select_dirinfo(dirinfo* orig,int* sel){
  dirinfo *di;
  fileinfo *top,*fi;
  int i;
  
  di=(dirinfo*)WXmalloc(sizeof(dirinfo));
  di->dir=WXnewstring(orig->dir);
  top=fi=(fileinfo*)WXmalloc(sizeof(fileinfo));
  fi->next=NULL;
  fi->name=NULL;
  fi->user=NULL;
  fi->group=NULL;
  fi->link=NULL;
  for(di->total=0;sel[di->total]>=0;di->total++){
    fi->next=copy_fileinfo(orig->files[sel[di->total]+1]);
    fi=fi->next;
    fi->next=NULL;
  }
  di->files=(fileinfo**)WXmalloc(sizeof(dirinfo*)*(di->total+1));
  fi=top;
  for(i=0;i<=di->total;i++){
    di->files[i]=fi;
    fi=fi->next;
  }
  return di;
}

  
void destroy_dirinfo(dirinfo* di){
  if(di==NULL)return;
  destroy_fileinfo(di->files[0]);
  WXfree((char*)di->dir);
  WXfree((char*)di);
}

struct _list_progress_data {
  WXwidget label;
  long got;
};
  
static int update_list_progress(int len,void* arg){
  static char buf[50];
  struct _list_progress_data *data;

  data=(struct _list_progress_data*)arg;

  if(len){
    sprintf(buf,"%ld ",data->got+=(long)len);
    WXsetLabel(data->label,buf);
  }
  return check_for_interrupt();
}

int read_remote_list(FILE* ls){
  int ret;

  struct _list_progress_data lpd;

  lpd.got=0;
  lpd.label=appdata.listprogress;

  ret= ftp_xfer_get('A',appdata.connect.data,fileno(ls),
      update_list_progress,(void*)&lpd);
  
  WXsetLabel(lpd.label,"          ");

  return ret;
}


dirinfo* create_remote_dirinfo(char* mask){
  dirinfo* current;
  FILE * ls;
  fileinfo* f;
  int result;
  
  current=(dirinfo*)WXmalloc(sizeof(dirinfo));

  if((result=ftp_pwd(&current->dir,&appdata.connect,logfile,check_for_interrupt))!=0){
    WXfree((char*)current);
    if(result==10){
      append_status("CONNECTION LOST\n");
      appdata.connected=0;
      (void)popup_reconnect_dialog(toplevel);
    }
    return NULL;
  }
  append_status(appdata.connect.lastline);

  if(mask==NULL || strlen(mask)==0){
    result=ftp_list(NULL,NULL,&appdata.connect,logfile,check_for_interrupt);
  } else {
    result=ftp_list("-ald",mask,&appdata.connect,logfile,check_for_interrupt);
  }

  if(result==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
  } else {
    append_status(appdata.connect.lastline);
  }

  if(result){
    WXfree((char*)current);
    return NULL;
  }

  ls=tmpfile();

  result=read_remote_list(ls);

  if(result==-1){
    append_status("CANCELLED\n");
    if(!ftp_abort_data(&appdata.connect,logfile,check_for_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else if(result){
    append_status("CONNECTION FAILED\n");
    if(!ftp_close_data(&appdata.connect,logfile,check_for_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else {
    if(!ftp_close_data(&appdata.connect,logfile,check_for_interrupt)){
      append_status(appdata.connect.lastline);
    }
  }

  lseek(fileno(ls),0,SEEK_SET);
  current->files=create_file_table(ls);
  fclose(ls);

  current->total=0;
  f=current->files[0];
  while(f->next!=NULL){
    current->total++;
    f=f->next;
  }

  return current;

}

dirinfo* create_local_dirinfo(char* mask){
  dirinfo* current;
  FILE * ls;
  fileinfo* f;
  static char* command="LANG=C /bin/ls -ald ";
  char *buf;
  int size;
  int len;
  
  current=(dirinfo*)WXmalloc(sizeof(dirinfo));

  size=SIZE;
  while(1) {
    buf=WXmalloc(size);
    if(getcwd(buf,size)==NULL) 
      if(errno==ERANGE){
	WXfree(buf);
	size*=2;
       } else {
	 WXfree(buf);
	 WXfree((char*)current);
	 return NULL;
       }
    else break;
  }
  current->dir=WXmalloc(strlen(buf)+1);
  strcpy(current->dir,buf);
  WXfree(buf);
  
  if(!mask)mask="";
  len=strlen(mask);
  if(len==0){
    buf=WXnewstring("LANG=C /bin/ls -al");
  } else {
    buf=WXmalloc(strlen(mask)+strlen(command)+1);
    strcpy(buf,command);
    strcat(buf,mask);
  }
  if((ls=popen(buf,"r"))==NULL){
    perror("create_local_dirinfo");
    exit(1);
  }
  WXfree(buf);
  
  current->files=create_file_table(ls);
  pclose(ls);

  current->total=0;
  f=current->files[0];
  while(f->next!=NULL){
    current->total++;
    f=f->next;
  }

  return current;

}


int convert_month(char *month){
  if(strcmp(month,"Jan")==0) return 1;
  if(strcmp(month,"Feb")==0) return 2;
  if(strcmp(month,"Mar")==0) return 3;
  if(strcmp(month,"Apr")==0) return 4;
  if(strcmp(month,"May")==0) return 5;
  if(strcmp(month,"Jun")==0) return 6;
  if(strcmp(month,"Jul")==0) return 7;
  if(strcmp(month,"Aug")==0) return 8;
  if(strcmp(month,"Sep")==0) return 9;
  if(strcmp(month,"Oct")==0) return 10;
  if(strcmp(month,"Nov")==0) return 11;
  if(strcmp(month,"Dec")==0) return 12;
  return 0;
}

int inthefuture(struct tm *t,int month,fileinfo *fi){
  if(month > (t->tm_mon+1)) return 1;
  return 0;
}

void set_fields(fileinfo** retval,int total){
  char year[5];
  int i,month;
  time_t bigseconds;
  struct tm *t;

  bigseconds=time(NULL);
  t=localtime(&bigseconds);
  sprintf(year,"%.4d",1900+t->tm_year);

  for(i=1;i<=total;i++){
    /* SIZE */ 
    sprintf(retval[i]->size_str,"%ld",retval[i]->size);
    /* DATE */
    if(retval[i]->date[0]=='\0'){
      month=convert_month(retval[i]->month);   
      if(month==0){
	retval[i]->date[0]='\0';
	retval[i]->time[0]='\0';
      } else {
	if(strchr(retval[i]->time_year,':')!=NULL){
	  if(inthefuture(t,month,retval[i])){
	    sprintf(retval[i]->date,"%.4d%.2d%.2d",
		t->tm_year+1899,month,retval[i]->day);
	  } else {
	    sprintf(retval[i]->date,"%.4d%.2d%.2d",
		t->tm_year+1900,month,retval[i]->day);
	  } 
	  if(strlen(retval[i]->time_year)>=5)
	    strcpy(retval[i]->time,retval[i]->time_year);
	  else 
	    sprintf(retval[i]->time,"0%s",retval[i]->time_year);
	} else {
	  sprintf(retval[i]->date,"%s%.2d%.2d",
	      retval[i]->time_year,month,retval[i]->day);
	  sprintf(retval[i]->time,"00:00");
	}
      }
    } else {
      if(strncmp(year,retval[i]->date,4)==0){
	strcpy(retval[i]->time_year,retval[i]->time);
      } else {
	strncpy(retval[i]->time_year,retval[i]->date,4);
	retval[i]->time_year[4]='\0';
      }
    }
  }
  return;
}

int check_presence(char* name, fileinfo* top){
  while((top=top->next)!=NULL){
    if(strcmp(top->name,name)==0)return 1;
  }
  return 0;
}

fileinfo** create_file_table(FILE *ls){
  fileinfo** retval;
  fileinfo  *top, *current;
  char buf[SIZE];
  char *line, *s,*result;
  int len,num,i;
  
  current=top=(fileinfo*)WXmalloc(sizeof(fileinfo));
  current->next=NULL;
  current->name=NULL;
  current->user=NULL;
  current->group=NULL;
  current->link=NULL;
  num=1;

  
  do {
    line=WXmalloc(1);
    line[0]='\0';
    do {
      if((result=fgets(buf,SIZE,ls))==NULL) break;
      len=strlen(buf);
      s=WXmalloc(strlen(line)+len+1);
      strcpy(s,line);
      strcat(s,buf); 
      WXfree(line);
      line=s;
    } while(len==SIZE-1 && buf[SIZE-2]!='\n');
    current->next=create_fileinfo(line);
    if(current->next==NULL)current->next=create_nt_fileinfo(line);
    if(current->next!=NULL){
      current=current->next;
      num++;
    }
    WXfree(line);
  } while(result!=NULL);

  if(!check_presence("..",top)){
    top->next=create_dummy_dir("..",top->next);
    num++;
  }
  
  if(!check_presence(".",top)){
    top->next=create_dummy_dir(".",top->next);
    num++;
  }
  
  retval=(fileinfo**)WXmalloc(num*sizeof(fileinfo*));
  current=top;
  for(i=0;i<num;i++){
    retval[i]=current;
    current=current->next;
  }

  set_fields(retval,num-1);

  return retval;

}
