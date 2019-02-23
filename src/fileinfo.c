/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<ctype.h>

#include"utils.h"
#include"fileinfo.h"

char* get_fileinfo_string(fileinfo* fi){
  char *buf;
  if(!fi)return NULL;
  if(fi->link){
    buf=WXmalloc(strlen(fi->name)+strlen(fi->link)+100);
    sprintf(buf,"%10.10s %3d %8.8s %8.8s %10ld %3.3s %2d %5.5s %s -> %s\n",
	fi->perms,
	fi->inode,
	fi->user,
	fi->group,
	fi->size,
	fi->month,
	fi->day,
	fi->time_year,
	fi->name,
	fi->link);
  } else {
    buf=WXmalloc(strlen(fi->name)+100);
    sprintf(buf,"%10.10s %3d %8.8s %8.8s %10ld %3.3s %2d %5.5s %s\n",
	fi->perms,
	fi->inode,
	fi->user,
	fi->group,
	fi->size,
	fi->month,
	fi->day,
	fi->time_year,
	fi->name);
  }
  return buf;
}

fileinfo* create_dummy_dir(char* name,fileinfo* next){
  fileinfo* fi;
  
  fi=(fileinfo*)WXmalloc(sizeof(fileinfo));
  strcpy(fi->perms,"d?????????");
  fi->inode=0;
  fi->user=WXnewstring("unknown");
  fi->group=WXnewstring("unknown");
  fi->month[0]='\0';
  fi->day=0;
  fi->time_year[0]='\0';
  fi->date[0]='\0';
  fi->size=0;
  fi->name=WXnewstring(name);
  fi->link=NULL;
  fi->next=next;

  return fi;
}

fileinfo* copy_fileinfo(fileinfo* orig){
  fileinfo *fi;

  fi=(fileinfo*)WXmalloc(sizeof(fileinfo));
  strncpy(fi->perms,orig->perms,11);
  fi->inode=orig->inode;
  fi->user=WXnewstring(orig->user);
  fi->group=WXnewstring(orig->group);
  fi->size=orig->size;
  strncpy(fi->month,orig->month,4);
  fi->day=orig->day;
  strncpy(fi->time_year,orig->time_year,6);
  fi->name=WXnewstring(orig->name);
  fi->link=WXnewstring(orig->link);
  strncpy(fi->date,orig->date,9);
  strncpy(fi->size_str,orig->size_str,20);
  strncpy(fi->time,orig->time,6);
  fi->next=NULL;

  return fi;
}

void destroy_fileinfo(fileinfo * fi){
  if(fi==NULL)return;
  free(fi->user);
  free(fi->group);
  free(fi->name);
  free(fi->link);
  destroy_fileinfo(fi->next);
  free(fi);
  return;
}

void* error_out(fileinfo* fi){
  destroy_fileinfo(fi);
  return NULL;
}

char* eatwhite(char* s,char* end){
  while(s<end && isspace(*s))s++;
  return s;
}

char* getnext(char* s,char* end){
  while(s<end && !isspace(*s))s++;
  return s;
}

static char* nt_month[]=
  {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

static int nt_month_index(char* h){
  int d=atoi(h);
  if(d<1 || d>12)return -1;
  else return d-1;
}
  
fileinfo* create_nt_fileinfo(char* head){
  char* end;
  char *tail;
  int len,i;
  fileinfo* fi;

  if((fi=malloc(sizeof(fileinfo)))==NULL){
    perror("create_fileinfo");
    exit(1);
  }
  fi->inode=0;
  fi->user=WXnewstring("unknown");
  fi->group=WXnewstring("unknown");
  fi->link=NULL;
  fi->name=NULL;
  fi->next=NULL;
  fi->time_year[0]='\0';
  end=head+strlen(head);
  tail=head;

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len<8)return error_out(fi);
  if(isdigit(head[0]) && isdigit(head[1]) && head[2]=='-'){
    if((i=nt_month_index(head))<0) return error_out(fi);
    strcpy(fi->month,nt_month[i]);
    strncpy(&fi->date[4],head,2);
  } else return error_out(fi);
  head+=3;
  if(isdigit(head[0]) && isdigit(head[1]) && head[2]=='-'){
    fi->day=atoi(head);
    strncpy(&fi->date[6],head,2);
  } else return error_out(fi);
  head+=3;
  if(isdigit(head[0]) && isdigit(head[1])){
    len-=8;
    if(len==0){
      strncpy(&fi->date[2],head,2);
      if(atoi(head)<50){/*2050 incompliance*/
        strncpy(fi->date,"20",2);
      } else {
	strncpy(fi->date,"19",2);
      }
    } else if(len==2){
      strncpy(fi->date,head,4);
    } else return error_out(fi);
  } else return error_out(fi);
  fi->date[8]='\0';

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;

  if(len!=7 || head[2]!=':' || (head[6]!='m' && head[6]!='M'))
      return error_out(fi);
  if(head[5]=='p' || head[5]=='P'){
    sprintf(fi->time,"%2d",atoi(head)+12);
  } else if(head[5]=='a' || head[5]=='A'){
    strncpy(fi->time,head,2);
  } else return error_out(fi);
  head+=2;
  strncpy(&fi->time[2],head,3);
  fi->time[5]='\0';

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len==5 && (strncmp(head,"<DIR>",5)==0 || strncmp(head,"<dir>",5)==0)){
    strcpy(fi->perms,"d?????????");
    fi->size=0;
  } else {
    for(i=0;i<len;i++){
      if(!isdigit(head[i]))return error_out(fi);
    }
    strcpy(fi->perms,"-?????????");
    fi->size=atoi(head);
  }

  head=eatwhite(tail,end);
  if(*(end-1)=='\n'){
    end--;
    if(*(end-1)=='\r')end--;
  }
  if((len=end-head)<=0) return error_out(fi);
  if((fi->name=malloc(len+1))==NULL){
    perror("create_fileinfo");
    exit(1);
  }
  strncpy(fi->name,head,len);fi->name[len]='\0';

  return fi;
}

fileinfo* create_fileinfo(char* head){
  char* end;
  char *tail,*newhead,*newtail;
  int len;
  fileinfo* fi;

  if((fi=malloc(sizeof(fileinfo)))==NULL){
    perror("create_fileinfo");
    exit(1);
  }
  fi->user=NULL;
  fi->group=NULL;
  fi->name=NULL;
  fi->link=NULL;
  fi->next=NULL;
  fi->date[0]='\0';
  fi->time[0]='\0';
  end=head+strlen(head);
  tail=head;

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len!=10) return error_out(fi);
  strncpy(fi->perms,head,10);fi->perms[10]='\0';

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len==0) return error_out(fi);

  newhead=head;
  newtail=tail;

  newhead=eatwhite(newtail,end);
  newtail=getnext(newhead,end);
  len=newtail-newhead;
  if(len==0)error_out(fi);

  newhead=eatwhite(newtail,end);
  newtail=getnext(newhead,end);
  len=newtail-newhead;
  if(len==0)error_out(fi);

  newhead=eatwhite(newtail,end);
  newtail=getnext(newhead,end);
  len=newtail-newhead;
  if(len==0)error_out(fi);

  if(isdigit(newhead[0])){
    fi->inode=atoi(head);
    head=eatwhite(tail,end);
    tail=getnext(head,end);
    len=tail-head;
  } else {
    fi->inode=0;
  }

  if(len==0) return error_out(fi);
  if((fi->user=malloc(len+1))==NULL){
    perror("create_fileinfo");
    exit(1);
  }
  strncpy(fi->user,head,len);fi->user[len]='\0';

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len==0) return error_out(fi);
  if((fi->group=malloc(len+1))==NULL){
    perror("create_fileinfo");
    exit(1);
  }
  strncpy(fi->group,head,len);fi->group[len]='\0';

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len==0) return error_out(fi);
  if(head[len-1] == ','){
    fi->size=0;
    head=eatwhite(tail,end);
    tail=getnext(head,end);
    len=tail-head;
  } else {
    fi->size=atoi(head);
  }


  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len<1) return error_out(fi);
  if(isdigit(*head)){
    int i=nt_month_index(head);
    if(i<0) return error_out(fi);
    strcpy(fi->month,nt_month[i]);
  } else {
    if(len!=3) return error_out(fi);
    strncpy(fi->month,head,3);fi->month[3]='\0';
  }

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len==0) return error_out(fi);
  fi->day=atoi(head);

  head=eatwhite(tail,end);
  tail=getnext(head,end);
  len=tail-head;
  if(len==0 || len>5) return error_out(fi);
  strncpy(fi->time_year,head,len);fi->time_year[len]='\0';

  head=tail+1;
  if(len<=4 && isspace(*head))head++;
  if(*(end-1)=='\n'){
    end--;
    if(*(end-1)=='\r')end--;
  }
  if((len=end-head)<=0) return error_out(fi);
  if(fi->perms[0]!='l'){
    if((fi->name=malloc(len+1))==NULL){
      perror("create_fileinfo");
      exit(1);
    }
    strncpy(fi->name,head,len);fi->name[len]='\0';
  } else {

    if(fi->size==0){
      char* sym_off=strstr(head," -> ");
      if(sym_off !=NULL){
	fi->size=end-sym_off-4;
      }
    }
    
    if(fi->size<=0 || (len-fi->size-4)<=0 || 
     strncmp(end-fi->size-4," -> ",4)!=0) return error_out(fi);
    if((fi->link=malloc(fi->size+1))==NULL){
      perror("create_fileinfo");
      exit(1);
    }
    strncpy(fi->link,end-fi->size,fi->size);fi->link[fi->size]='\0';
    if((fi->name=malloc(len-fi->size-3))==NULL){
      perror("create_fileinfo");
      exit(1);
    }
    strncpy(fi->name,head,len-fi->size-4);fi->name[len-fi->size-4]='\0';
  }

  return fi;
}

