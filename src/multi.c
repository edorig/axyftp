/* Copyright (c) 1998,1999   Alexander Yukhimets. All rights reserved. */
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/stat.h>
#include<setjmp.h>

#include"multi.h"

#include"axyftp.h"
#include"progress_dialog.h"
#include"read_init.h"
#include"fileinfo.h"
#include"dirinfo.h"
#include"functions.h"
#include"utils.h"
#include"ftp.h"

#include"prompt_dialogs.h"

sigjmp_buf jmp_down_env;

static struct _dirs {
  char* dir;
  struct _dirs *next;
} *dirs;

void destroy_dirs(struct _dirs* d){
  if(d==NULL)return;
  WXfree(d->dir);
  destroy_dirs(d->next);
  WXfree((char*)d);
}

static int append_dirs(char* dir){
  struct _dirs *d;

  if(dirs==NULL){
    dirs=(struct _dirs*)WXmalloc(sizeof(struct _dirs));
    dirs->dir=NULL;
    dirs->next=NULL;
  }

  for(d=dirs;d->next!=NULL;d=d->next){
    if(strcmp(d->next->dir,dir)==0){
      return 1;
    }
  }

  d->next=(struct _dirs*)WXmalloc(sizeof(struct _dirs));
  d=d->next;
  d->next=NULL;
  d->dir=WXnewstring(dir);

  return 0;
}

#ifndef SIZE
#define SIZE 256
#endif

void get_dir_files(dirinfo*,char);
void put_dir_files(dirinfo*,char);

static int process_down_directory(fileinfo* fi,char type){
  dirinfo *di;
  struct stat statbuf;
  char* buf;
  int size;

  if(appdata.interrupt) return -1;

  if(!stat(fi->name,&statbuf)){
    if(!S_ISDIR(statbuf.st_mode)){
      return 1;
    }
  } else {
    if(mkdir(fi->name,0755)==-1)return 2;
  }

  size=SIZE;
  while(1) {
    buf=WXmalloc(size);
    if(getcwd(buf,size)==NULL){
      if(errno==ERANGE){
        WXfree(buf);
        size*=2;
      } else {
        WXfree(buf);
        return 3;
      }
    } else {
      break;
    }
  }


  if(chdir(fi->name)==-1){
    WXfree(buf);
    return 4;
  }

  di=create_remote_dirinfo(NULL);

  if(appdata.interrupt){
    destroy_dirinfo(di);
    return -1;
  }
  
  if(!append_dirs(di->dir))get_dir_files(di,type);
  destroy_dirinfo(di);
  chdir(buf);
  WXfree(buf);
  return 0;
}


void get_dir_files(dirinfo* di, char type){
  fileinfo *fi;

  if(appdata.interrupt) return;
  /*printf("%s\n",di->dir);*/

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]=='-'){
      download_file(fi,type);
      /*{
        char* p;
	p=get_fileinfo_string(fi);
	printf("%d: %s",ret,p);
	WXfree(p);
      }*/
      if(appdata.interrupt) return;
    }
  }

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]=='d'){
      if(strcmp(fi->name,".") && strcmp(fi->name,"..") &&
	  !ftp_chdir(fi->name,&appdata.connect,logfile,check_for_interrupt)){
        process_down_directory(fi,type);
	ftp_chdir(di->dir,&appdata.connect,logfile,check_for_interrupt);
	if(appdata.interrupt) return;
      }
    }
  }

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]=='l'){
      if(!ftp_chdir(fi->name,&appdata.connect,logfile,check_for_interrupt)){
        process_down_directory(fi,type);
	ftp_chdir(di->dir,&appdata.connect,logfile,check_for_interrupt);
	if(appdata.interrupt) return;
      } else {
	download_file(fi,type);
	/*{
	  char* p;
	  p=get_fileinfo_string(fi);
	  printf("%d: %s",ret,p);
	  WXfree(p);
	}*/
	if(appdata.interrupt) return;
      }
    }
  }
}

int get_files(int* list,char type){
  dirinfo *di;
  
  dirs=NULL;

  di=select_dirinfo(appdata.remote.list,list);
  append_dirs(di->dir);

  get_dir_files(di,type);

  /*
  printf("%s %d\n",di->dir,di->total);
  for(i=0;i<di->total;i++){
    char* p;
    p=get_fileinfo_string(di->files[i+1]);
    printf("%s",p);
    WXfree(p);
  }
  printf("\n");
  */

  destroy_dirinfo(di);
  destroy_dirs(dirs);

  
  if(appdata.progress_shown){
    hide_progress_dialog(appdata.progress);
    appdata.progress_shown=0;
  }

  appdata.interrupt=0;

  return 0;
}

static int process_up_directory(fileinfo* fi,char type){
  dirinfo *di;
  char* buf;

  if(appdata.interrupt) return -1;

  di=create_local_dirinfo(NULL);
  if(append_dirs(di->dir)){
    destroy_dirinfo(di);
    return 1;
  }
  if(ftp_pwd(&buf,&appdata.connect,logfile,check_for_small_interrupt)){
    return 2;
  }
  if(appdata.interrupt){
    destroy_dirinfo(di);
    return -1;
  }
  ftp_mkdir(fi->name,&appdata.connect,logfile,check_for_small_interrupt);
  if(appdata.interrupt){
    destroy_dirinfo(di);
    return -1;
  }
  if(ftp_chdir(fi->name,&appdata.connect,logfile,check_for_small_interrupt)){
    destroy_dirinfo(di);
    return 1;
  }
  put_dir_files(di,type);
  destroy_dirinfo(di);
  ftp_chdir(buf,&appdata.connect,logfile,check_for_small_interrupt);
  WXfree(buf);

  return 0;
}

void put_dir_files(dirinfo* di, char type){
  fileinfo *fi;

  if(appdata.interrupt) return;
  /*printf("%s\n",di->dir);*/

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]=='-'){
      upload_file(fi,type);
      /*{
        char* p;
	p=get_fileinfo_string(fi);
	printf("%d: %s",ret,p);
	WXfree(p);
      }*/
      if(appdata.interrupt) return;
    }
  }

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]=='d'){
      if(strcmp(fi->name,".") && strcmp(fi->name,"..") && chdir(fi->name)!=-1){
        process_up_directory(fi,type);
	chdir(di->dir);
	if(appdata.interrupt) return;
      }
    }
  }

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]=='l'){
      if(chdir(fi->name)!=-1){
        process_up_directory(fi,type);
	chdir(di->dir);
	if(appdata.interrupt) return;
      } else {
	upload_file(fi,type);
	/*{
	  char* p;
	  p=get_fileinfo_string(fi);
	  printf("%d: %s",ret,p);
	  WXfree(p);
	}*/
	if(appdata.interrupt) return;
      }
    }
  }
}


int put_files(int* list,char type){
  dirinfo *di;
  
  dirs=NULL;

  di=select_dirinfo(appdata.local.list,list);
  append_dirs(di->dir);

  put_dir_files(di,type);

  destroy_dirinfo(di);
  destroy_dirs(dirs);

  if(appdata.progress_shown){
    hide_progress_dialog(appdata.progress);
    appdata.progress_shown=0;
  }

  appdata.interrupt=0;

  return 0;
}

static int verify_deletions;
static int verify_local_deletions;

static void delete_remote_link(fileinfo *fi){
  delete_remote(fi->name);
  if(appdata.connect.lastline[0]=='5'){
    ftp_put('I',fi->link,&appdata.connect,logfile,check_for_interrupt);
    ftp_close_data(&appdata.connect,logfile,check_for_interrupt);
    delete_remote(fi->name);
    delete_remote(fi->link);
  }
}


int delete_remote_dir_files(dirinfo* volatile di){
  dirinfo * volatile di_new;
  fileinfo * volatile fi;
  int volatile ret;

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]=='l'){
      if(verify_deletions){
	if(!(ret=sigsetjmp(jmp_down_env,1))){
	  (void)init_delete_dialog(appdata.delete_dialog,fi->name);
	  LOOP();
	}
	if(ret==1){
	  delete_remote_link(fi);
	} else if(ret==2){
	  verify_deletions=0;
	  delete_remote_link(fi);
	}
      } else {
	delete_remote_link(fi);
      }
    }
  }
  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]!='d'){
      if(verify_deletions){
	if(!(ret=sigsetjmp(jmp_down_env,1))){
	  (void)init_delete_dialog(appdata.delete_dialog,fi->name);
	  LOOP();
	}
	if(ret==1){
	  delete_remote(fi->name);
	} else if(ret==2){
	  verify_deletions=0;
	  delete_remote(fi->name);
	}
      } else {
	delete_remote(fi->name);
      }
    } else if(strcmp(".",fi->name) && strcmp("..",fi->name)){
      if( appdata.odata->recurse_del &&
	  !ftp_chdir(fi->name,&appdata.connect,logfile,check_for_interrupt)){
	di_new=create_remote_dirinfo(".* *");
	delete_remote_dir_files(di_new);
	destroy_dirinfo(di_new);
	ftp_chdir(di->dir,&appdata.connect,logfile,check_for_interrupt);
      }
      if(verify_deletions){
	if(!(ret=sigsetjmp(jmp_down_env,1))){
	  (void)init_delete_dialog(appdata.delete_dialog,fi->name);
	  LOOP();
	}
	if(ret==1){
	  rmdir_remote(fi->name);
	} else if(ret==2){
	  verify_deletions=0;
	  rmdir_remote(fi->name);
	}
      } else {
	rmdir_remote(fi->name);
      }
    }
  }

  return 0;
}


int delete_remote_files(int* list){

  dirinfo *di;
  
  dirs=NULL;

  di=select_dirinfo(appdata.remote.list,list);
  verify_deletions=appdata.odata->deletions;

  delete_remote_dir_files(di);
  destroy_dirinfo(di);

  return 0;
}

int delete_local_dir_files(dirinfo* volatile di){
  dirinfo * volatile di_new;
  fileinfo * volatile fi;
  int volatile ret;

  fi=di->files[0];
  while(fi->next!=NULL){
    fi=fi->next;
    if(fi->perms[0]!='d'){
      if(verify_local_deletions){
	if(!(ret=sigsetjmp(jmp_down_env,1))){
	  (void)init_delete_dialog(appdata.delete_dialog,fi->name);
	  LOOP();
	}
	if(ret==1){
	  delete_local(fi->name);
	} else if(ret==2){
	  verify_local_deletions=0;
	  delete_local(fi->name);
	}
      } else {
	delete_local(fi->name);
      }
    } else if(strcmp(".",fi->name) && strcmp("..",fi->name)){
      if( appdata.odata->recurse_del &&
	  !chdir(fi->name)){
	di_new=create_local_dirinfo(".* *");
	delete_local_dir_files(di_new);
	destroy_dirinfo(di_new);
	chdir(di->dir);
      }
      if(verify_local_deletions){
	if(!(ret=sigsetjmp(jmp_down_env,1))){
	  (void)init_delete_dialog(appdata.delete_dialog,fi->name);
	  LOOP();
	}
	if(ret==1){
	  rmdir_local(fi->name);
	} else if(ret==2){
	  verify_local_deletions=0;
	  rmdir_local(fi->name);
	}
      } else {
	rmdir_local(fi->name);
      }
    }
  }

  return 0;
}


int delete_local_files(int* list){

  dirinfo *di;
  
  dirs=NULL;

  di=select_dirinfo(appdata.local.list,list);
  verify_local_deletions=appdata.odata->deletions;

  delete_local_dir_files(di);
  destroy_dirinfo(di);

  return 0;
}
