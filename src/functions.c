/* Copyright (c) 1998,1999   Alexander Yukhimets. All rights reserved. */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"utils.h"
#include"multi.h"
#include"ftp.h"
#include"ftp_xfer.h"
#include"axyftp.h"
#include"status.h"
#include"read_init.h"
#include"dirinfo.h"
#include"dirlist.h"
#include"dirname.h"
#include"progress_dialog.h"
#include"little_dialogs.h"
#include"viewer.h"
#include"prompt_dialogs.h"
#include"functions.h"

int show_help(int num){
#ifdef AXYFTP_HELP_DIR
  static char* help_loc=AXYFTP_HELP_DIR "/axyftp.html";
#else
  static char* help_loc="/usr/local/share/axyftp/help/axyftp.html";
#endif /*AXYFTP_HELP_DIR*/
  char* buf;
  buf=WXmalloc(strlen(help_loc)*3+80);
  sprintf(buf,
      "netscape -remote \'OpenURL(%s)\' || firefox %s || xterm -e lynx %s &",
      help_loc,help_loc,help_loc);
  system(buf);
  WXfree(buf);
  return 0;
}

struct _file_progress_data {
  WXwidget progress;
  long totalsize;
  time_t lasttime;
  long currentsize;
  float rate;
  long elapsed;
  long left;
  int percent;
};

static int update_file_progress(int len,void* arg){
  struct _file_progress_data *data;
  time_t currenttime;
  long newelapsed;
  long newleft;
  float newrate;
  long newsize;
  int newpercent;

  data=(struct _file_progress_data*)arg;
  
  currenttime=time(NULL);
  newsize=data->currentsize+(long)len;
  if(currenttime==data->lasttime){
    data->currentsize=newsize;
    return check_for_small_interrupt();
  }
  newelapsed=(currenttime-data->lasttime);
  newelapsed+=data->elapsed;
  if(newelapsed)newrate=((float)newsize)/newelapsed/1024;
  else newrate=data->rate;
  if(newelapsed && newsize){
    newleft=((long long)data->totalsize)*((long long)newelapsed)/newsize;
    newleft-=newelapsed;
  } else {
    newleft=data->left;
  }
  data->lasttime=currenttime;
  if(data->totalsize)newpercent=
    ((long long)newsize)*100/data->totalsize;
  else newpercent=data->percent;

  if(newsize==data->currentsize)newsize=-1;else data->currentsize=newsize;
  if(newrate==data->rate)newrate=-1.;else data->rate=newrate;
  if(newelapsed==data->elapsed)newelapsed=-1;else data->elapsed=newelapsed;
  if(newleft==data->left)newleft=-1;else data->left=newleft;
  if(newpercent==data->percent)newpercent=-1;else data->percent=newpercent;
  update_progress_dialog(data->progress,newpercent,newsize,newrate,
      newelapsed,newleft);
  return check_for_small_interrupt();
}

int check_for_interrupt(){
  process_events();
  if(appdata.interrupt){
    appdata.interrupt=0;
    return 1;
  }
  return 0;
}

int rename_local_num(int num){
  char* text;
  char* orig;
  orig=appdata.local.list->files[num+1]->name;
  text=WXmalloc(strlen(orig)+35);
  sprintf(text,"Enter new name for \"%s\" (local):",orig);
  init_rename_dialog(appdata.rename_local,text,orig);
  WXfree(text);
  return 0;
}

int rename_remote_num(int num){
  char* text;
  char* orig;
  orig=appdata.remote.list->files[num+1]->name;
  text=WXmalloc(strlen(orig)+35);
  sprintf(text,"Enter new name for \"%s\" (remote):",orig);
  init_rename_dialog(appdata.rename_remote,text,orig);
  WXfree(text);
  return 0;
}

int rename_local(char* from,char* to){
  return rename(from,to);
}

int rename_remote(char* from,char* to){
  int ret;
  ret=ftp_rename(from,to,&appdata.connect,logfile,check_for_interrupt);
  if(ret==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(NULL);
  } else {
    append_status(appdata.connect.lastline);
  }
  return ret;
}

int exec_local(char* name){
  return system(name);
}
int mkdir_local(char* name){
  return mkdir(name,0755);
}

int exec_remote(char* name){
  int ret;
  ret=ftp_site(name,&appdata.connect,logfile,check_for_interrupt);
  if(ret==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
  } else {
    append_status(appdata.connect.lastline);
  }
  return ret;
}

int mkdir_remote(char* name){
  int ret;
  ret=ftp_mkdir(name,&appdata.connect,logfile,check_for_interrupt);
  if(ret==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
  } else {
    append_status(appdata.connect.lastline);
  }
  return ret;
}

int show_log(){
  int lfd;
  WXwidget viewer;

  fflush(logfile);
  if((lfd=open(log_file,O_RDONLY))<0){
    char* text=WXmalloc(strlen(log_file)+25);
    sprintf(text,"Cannot open log file %s",log_file);
    (void)popup_warning_dialog(toplevel,text);
    WXfree(text);
    return 1;
  }
  viewer=create_viewer(toplevel,"Log Window");

  return fill_viewer(viewer,lfd);
}

int view_local_num(int num){
  char* cmd;

  cmd=WXmalloc(2*strlen(appdata.local.list->files[num+1]->name)+100);
  sprintf(cmd,"${XEDITOR-gvim} %s || xterm -e ${EDITOR-vi} %s &",
      appdata.local.list->files[num+1]->name,
      appdata.local.list->files[num+1]->name);
  system(cmd);
  WXfree(cmd);
  return 0;
/*  int lfd;
  GtkWidget* viewer;
  char* text;

  if((lfd=open(appdata.local.list->files[num+1]->name,O_RDONLY))<0){
    char* text=WXmalloc(strlen(appdata.local.list->files[num+1]->name)+14);
    sprintf(text,"Cannot open %s",appdata.local.list->files[num+1]->name);
    (void)popup_warning_dialog(toplevel,text);
    WXfree(text);
    return 1;
  }
  text=WXmalloc(strlen(appdata.local.list->files[num+1]->name)+8);
  sprintf(text,"Local: %s",appdata.local.list->files[num+1]->name);
  viewer=create_viewer(toplevel,text);
  WXfree(text);

  return fill_viewer(viewer,lfd);
  */
}

int view_remote_num(int num){
  char* text;
  int ret;
  FILE* tmp;
  struct _file_progress_data fpd;
  WXwidget viewer;

  if(appdata.remote.list->files[num+1]->perms[0]!='-'){
    (void)popup_warning_dialog(toplevel,
	"Only \"normal\" files transfers are implemented in this version\n"
	"Sorry...");
    return 1;
  }
  tmp=tmpfile();

  ret=ftp_get('A',appdata.remote.list->files[num+1]->name,&appdata.connect,
      logfile,check_for_interrupt);
  if(ret==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
    fclose(tmp);
    return 1;
  } else {
    append_status(appdata.connect.lastline);
  }
  if(ret)return 2;
  if(appdata.connect.lastline[0]!='1')return 3;

/*  progress=create_progress_dialog(toplevel);*/
  init_progress_dialog(appdata.progress,
                appdata.remote.list->files[num+1]->name,
                appdata.remote.list->files[num+1]->size);

  process_events();

  fpd.progress=appdata.progress;
  fpd.totalsize=appdata.remote.list->files[num+1]->size;
  fpd.lasttime=time(NULL);
  fpd.currentsize=0;
  fpd.elapsed=0;
  fpd.left=-1;
  fpd.rate=-1.;
  fpd.percent=0;

  ret=ftp_xfer_get('A',appdata.connect.data,fileno(tmp),update_file_progress,
      (void*)&fpd);
  hide_progress_dialog(appdata.progress);
  if(appdata.odata->xferbeep)beep();
  if(ret==-1){
    append_status("CANCELLED\n");
    if(!ftp_abort_data(&appdata.connect,logfile,check_for_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else if(ret){
    append_status("CONNECTION FAILED\n");
    if(!ftp_close_data(&appdata.connect,logfile,check_for_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else {
    if(!ftp_close_data(&appdata.connect,logfile,check_for_interrupt)){
      append_status(appdata.connect.lastline);
    }
  }
  lseek(fileno(tmp),0,SEEK_SET);

  text=WXmalloc(strlen(appdata.remote.list->files[num+1]->name)+10);
  sprintf(text,"%s (remote)",appdata.remote.list->files[num+1]->name);
  viewer=create_viewer(toplevel,text);
  WXfree(text);

  return fill_viewer(viewer,fileno(tmp));

}

int show_local_dirinfo(){
  char* text;
  WXwidget viewer;

  text=WXmalloc(strlen(appdata.local.list->dir)+20);
  sprintf(text,"%s/ (local DirInfo)",appdata.local.list->dir);
  viewer=create_viewer(toplevel,text);
  WXfree(text);
  fill_dirinfo(viewer,appdata.local.list);
  return 0;
}

int show_remote_dirinfo(){
  char* text;
  WXwidget viewer;

  text=WXmalloc(strlen(appdata.remote.list->dir)+20);
  sprintf(text,"%s/ (remote DirInfo)",appdata.remote.list->dir);
  viewer=create_viewer(toplevel,text);
  WXfree(text);
  fill_dirinfo(viewer,appdata.remote.list);
  return 0;
}

int delete_local_num(int num){
  if(appdata.local.list->files[num+1]->perms[0]=='d'){
    return rmdir_local(appdata.local.list->files[num+1]->name);
  } else {
    return delete_local(appdata.local.list->files[num+1]->name);
  }
}

int delete_remote_num(int num){
  if(appdata.remote.list->files[num+1]->perms[0]=='d'){
    return rmdir_remote(appdata.remote.list->files[num+1]->name);
  } else {
    return delete_remote(appdata.remote.list->files[num+1]->name);
  }
}

int rmdir_local(char* name){
  return rmdir(name);
}

int delete_local(char* name){
  return unlink(name);
}

int rmdir_remote(char* name){
  int ret;
  ret=ftp_rmdir(name,&appdata.connect,logfile,check_for_interrupt);
  if(ret==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
  } else {
    append_status(appdata.connect.lastline);
  }
  return ret;
}

int delete_remote(char* name){
  int ret;
  ret=ftp_delete(name,&appdata.connect,logfile,check_for_interrupt);
  if(ret==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
  } else {
    append_status(appdata.connect.lastline);
  }
  return ret;
}

int check_for_small_interrupt(){
  process_events();
  if(appdata.interrupt || appdata.small_interrupt){
    appdata.small_interrupt=0;
    return 1;
  }
  return 0;
}

int upload_file(fileinfo* fi,char type){
  int ret;
  int lfd;
  struct _file_progress_data fpd;
  struct stat statbuf;
  int show_progress;

  if((lfd=open(fi->name,O_RDONLY))<0){
    char* b;
    b=WXmalloc(strlen(fi->name)+40);
    sprintf(b,"Cannot open local file %s\n",fi->name);
    append_status(b);
    WXfree(b);
    return 2;
  }
  if(fstat(lfd,&statbuf)){
    char* b;
    b=WXmalloc(strlen(fi->name)+40);
    sprintf(b,"Cannot get size info for %s\n",fi->name);
    append_status(b);
    WXfree(b);
    return 2;
  }
  if(!type)type='I';
  ret=ftp_put(type,fi->name,&appdata.connect,logfile,check_for_small_interrupt);
  if(ret==10){
    /*
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
    */
    close(lfd);
    return 1;
  } else {
    append_status(appdata.connect.lastline);
  }
  if(ret){
    close(lfd);
    return 3;
  }
  if(appdata.connect.lastline[0]!='1'){
    close(lfd);
    return 4;
  }

  show_progress=0;
  if(appdata.odata->show_progress){
    long tres;
    tres=atoi(appdata.odata->progress_treshold);
    if(tres<0)tres=0;
    if(statbuf.st_size/1024>=tres){

      /*progress=create_progress_dialog(toplevel);*/
      if(!appdata.progress_shown){
	show_progress_dialog(appdata.progress);
	appdata.progress_shown=1;
      }
      init_progress_dialog(appdata.progress,fi->name,statbuf.st_size);

      process_events();

      fpd.progress=appdata.progress;
      fpd.totalsize=statbuf.st_size;
      fpd.lasttime=time(NULL);
      fpd.currentsize=0;
      fpd.elapsed=0;
      fpd.left=-1;
      fpd.rate=-1.;
      fpd.percent=0;

      show_progress=1;
      ret=ftp_xfer_put(type,appdata.connect.data,lfd,update_file_progress,
	  (void*)&fpd);
    }
  }
  if(!show_progress){
    ret=ftp_xfer_put(type,appdata.connect.data,lfd,NULL,NULL);
  }

  close(lfd);
  if(show_progress){
    init_progress_dialog(appdata.progress,"",0);
  }
  if(appdata.odata->xferbeep)beep();
  if(ret==-1){
    append_status("CANCELLED\n");
    if(!ftp_abort_data(&appdata.connect,logfile,check_for_small_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else if(ret){
    append_status("CONNECTION FAILED\n");
    if(!ftp_close_data(&appdata.connect,logfile,check_for_small_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else {
    if(!ftp_close_data(&appdata.connect,logfile,check_for_small_interrupt)){
      append_status(appdata.connect.lastline);
    }
  }
  return 0;
}

int download_file(fileinfo* volatile fi,char volatile type){
  volatile int ret;
  volatile int lfd;
  volatile struct _file_progress_data fpd;
  struct stat statbuf;
  long size;
  int show_progress;

  if(!type)type='I';

  ftp_size(&size,type,fi->name,&appdata.connect,logfile,
      check_for_small_interrupt);
  if(size<=0){
    size=fi->size;
  }

  ret=1;
  if(!stat(fi->name,&statbuf)){
    volatile long save_size=size;
    if(S_ISDIR(statbuf.st_mode))return 1;
    if(!(ret=sigsetjmp(jmp_down_env,1))){
      (void)init_xfer_dialog(appdata.xfer_dialog,fi->name);
      LOOP();
    }
    size=save_size;
  }


  if(ret==1){
    if((lfd=open(fi->name,O_WRONLY|O_CREAT|O_TRUNC,0755))<0){
      char* b;
      b=WXmalloc(strlen(fi->name)+40);
      sprintf(b,"Cannot open local file %s\n",fi->name);
      append_status(b);
      WXfree(b);
      return 2;
    }
    ret=ftp_get(type,fi->name,&appdata.connect,logfile,
	check_for_small_interrupt);
    fpd.totalsize=size;
  } else if(ret==2){
    if((lfd=open(fi->name,O_WRONLY|O_APPEND,0755))<0){
      char* b;
      b=WXmalloc(strlen(fi->name)+40);
      sprintf(b,"Cannot open local file %s\n",fi->name);
      append_status(b);
      WXfree(b);
      return 2;
    }
    ret=ftp_resume(type,statbuf.st_size,fi->name,&appdata.connect,logfile,
	check_for_small_interrupt);
    fpd.totalsize=size-statbuf.st_size;
    if(fpd.totalsize<0)fpd.totalsize=0;
  } else if(ret==3){
    return 0;
  }

  if(ret==10){
    return 10;
  } else {
    append_status(appdata.connect.lastline);
  }
  if(ret)return 4;
  if(appdata.connect.lastline[0]!='1')return 5;

  show_progress=0;
  if(appdata.odata->show_progress){
    long tres;
    tres=atoi(appdata.odata->progress_treshold);
    if(tres<0)tres=0;
    if(size/1024>=tres || (fi->perms[0] == 'l' && size==fi->size)){

      /*progress=create_progress_dialog(toplevel);*/
      if(!appdata.progress_shown){
	show_progress_dialog(appdata.progress);
	appdata.progress_shown=1;
      }
      init_progress_dialog(appdata.progress,fi->name,size);

      process_events();

      fpd.progress=appdata.progress;
      fpd.currentsize=0;
      fpd.lasttime=time(NULL);
      fpd.elapsed=0;
      fpd.left=-1;
      fpd.rate=-1.;
      fpd.percent=0;

      show_progress=1;
      ret=ftp_xfer_get(type,appdata.connect.data,lfd,update_file_progress,
	  (void*)&fpd);
    } 
  }
  if(!show_progress){
    ret=ftp_xfer_get(type,appdata.connect.data,lfd,NULL,NULL);
  }


  close(lfd);
  if(show_progress){
    init_progress_dialog(appdata.progress,"",0);
  }
  if(appdata.odata->xferbeep)beep();
  if(ret==-1){
    append_status("CANCELLED\n");
    if(!ftp_abort_data(&appdata.connect,logfile,check_for_small_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else if(ret){
    append_status("CONNECTION FAILED\n");
    if(!ftp_close_data(&appdata.connect,logfile,check_for_small_interrupt)){
      append_status(appdata.connect.lastline);
    }
  } else {
    if(!ftp_close_data(&appdata.connect,logfile,check_for_small_interrupt)){
      append_status(appdata.connect.lastline);
    }
  }
  return 0;
}

int chg_remote_dir(int num){
  return chg_remote_dirname(appdata.remote.list->files[num+1]->name);
}

int chg_remote_dirname(char* name){
  int retval;
  retval=ftp_chdir(name,&appdata.connect,logfile,check_for_interrupt);
  if(retval==10){
    append_status("CONNECTION LOST\n");
    appdata.connected=0;
    (void)popup_reconnect_dialog(toplevel);
  } else {
    append_status(appdata.connect.lastline);
  }
  if(retval){
    (void)popup_error_dialog(toplevel,"Could not change to remote directory");
  }
  return retval;
}


int chg_local_dir(int num){
  return chg_local_dirname(appdata.local.list->files[num+1]->name);
}

int chg_local_dirname(char* name){
  if(chdir(name)==-1){
    (void)popup_error_dialog(toplevel,"Could not change to local directory");
    return 1;
  }
  return 0;
}


void end_session(){
  if(!ftp_disconnect(&appdata.connect,logfile,check_for_interrupt)){
    append_status(appdata.connect.lastline);
  } else {
    append_status("DISCONNECTED\n");
  }
  appdata.connected=0;
  if(appdata.remote.list){
    destroy_dirinfo(appdata.remote.list);
    appdata.remote.list=NULL;
  }
}


void start_session(session_data* sd,char* mask){
  int good;
  char* p;
  char* cmd;

  if(check_for_interrupt())return;

  appdata.connect.passive=1;

  good=ftp_connect(sd->host,atoi(sd->port),
      &appdata.connect,logfile,check_for_interrupt);
  if(good){
    append_status("CONNECT FAILED\n");
    return;
  }
  append_status(appdata.connect.lastline);
  good=ftp_login(sd->user,sd->pass,sd->account,&appdata.connect,NULL,
      check_for_interrupt);
  if(good){
    append_status("LOGIN FAILED\n");
    return;
  }
  append_status(appdata.connect.lastline);
  if((good=ftp_syst(&appdata.connect,logfile,check_for_interrupt))==0 || good==2){
    append_status(appdata.connect.lastline);
  } else {
    append_status("CONNECT FAILED\n");
    return;
  }
  appdata.connected=1;
  if(*(sd->locdir)){
    if(!chg_local_dirname(sd->locdir)){
      update_local(sd->locmask);
    }
  } else if(*(sd->locmask)){
    update_local(sd->locmask);
  }
  p=sd->initcom;
  for(;;){
    while(*p && isspace(*p))p++;
    if(*p=='\0')break;
    good=0;while(p[good]!='\0' && p[good]!=';')good++;
    cmd=WXmalloc(good+3);
    strncpy(cmd,p,good);
    strcpy(&cmd[good],"\r\n");
    if(ftp_command(cmd,&appdata.connect,logfile,check_for_interrupt)==10){
      appdata.connected=0;
      append_status("CONNECTION LOST\n");
      return;
    }
    if(*(p=&p[good])=='\0')break;
    p++;
  }
  if(*(sd->remdir)){
    good=chg_remote_dirname(sd->remdir);
    if(good==10){
      appdata.connected=0;
      append_status("CONNECTION LOST\n");
      return;
    }
  }
  update_remote(mask);
}

int update_remote(char* mask){
  dirinfo *di;

  di=create_remote_dirinfo(mask);
  
  if(di==NULL)return 1;
  destroy_dirinfo(appdata.remote.list);
  appdata.remote.list=di;
  update_dirname(appdata.remote.combo,appdata.remote.list);
  update_dirlist(appdata.remote.table,appdata.remote.list);
  return 0;
}

int update_local(char* mask){

  destroy_dirinfo(appdata.local.list);

  appdata.local.list=create_local_dirinfo(mask);
  
  if(appdata.local.list==NULL)return 1;

  update_dirname(appdata.local.combo,appdata.local.list);
  update_dirlist(appdata.local.table,appdata.local.list);


  return 0;
}

