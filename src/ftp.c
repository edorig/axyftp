/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"utils.h"

#include<string.h>
#include<signal.h>
#include<ctype.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>

#include<netdb.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<arpa/ftp.h>
#include<arpa/telnet.h>

extern int h_errno;

#include"ftp.h"

#define FTP_MIN_BUF 256
static int interrupt;

void handler(int sig){
  interrupt=sig;
  /*printf("\n\n\n\nGOT SIGNAL!\n\n\n");*/
  return;
}

int ftp_size(long* size,char type,char* name,connect_data* cd,FILE* log,
    ftp_check_proc proc){
  char* cmd;
  int ret;

  if(name==NULL){
    *size=-1;
    return 0;
  }

  cmd=WXmalloc(strlen(name)+9);
  sprintf(cmd,"TYPE %c\r\n",type);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    *size=-1;
    WXfree(cmd);
    return ret;
  }
  if(cd->lastline[0]!='2'){
    *size=-1;
    WXfree(cmd);
    return 1;
  }

  sprintf(cmd,"SIZE %s\r\n",name);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    *size=-1;
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);
  if(cd->lastline[0]!='2' || strlen(cd->lastline)<5){
    *size=-1;
    return 2;
  }
  *size=atol(&cd->lastline[4]);
  return 0;
}

int ftp_rename(char* from,char* to,connect_data* cd,FILE* log,
    ftp_check_proc proc){
  char* cmd;
  int ret;

  if(from==NULL || to==NULL)return 0;

  cmd=WXmalloc(strlen(from)+8);
  sprintf(cmd,"RNFR %s\r\n",from);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);
  if(cd->lastline[0]!='3'){
    return 2;
  }
  cmd=WXmalloc(strlen(to)+8);
  sprintf(cmd,"RNTO %s\r\n",to);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);
  if(cd->lastline[0]!='2'){
    return 2;
  }
  return 0;
}

int ftp_site(char* name,connect_data* cd,FILE* log,ftp_check_proc proc){
  char* cmd;
  int ret;

  if(name==NULL)return 0;

  cmd=WXmalloc(strlen(name)+8);

  sprintf(cmd,"SITE %s\r\n",name);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='2'){
    return 2;
  }
  return 0;
}

int ftp_mkdir(char* name,connect_data* cd,FILE* log,ftp_check_proc proc){
  char* cmd;
  int ret;

  if(name==NULL)return 0;

  cmd=WXmalloc(strlen(name)+8);

  sprintf(cmd,"MKD %s\r\n",name);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='2'){
    return 2;
  }
  return 0;
}

int ftp_syst(connect_data* cd,FILE* log,ftp_check_proc proc){
  int ret;

  if((ret=ftp_command("SYST\r\n",cd,log,proc))!=0){
    return ret;
  }
  if(cd->lastline[0]!='2'){
    return 2;
  }
  return 0;
}

int ftp_rmdir(char* name,connect_data* cd,FILE* log,ftp_check_proc proc){
  char* cmd;
  int ret;

  if(name==NULL)return 0;

  cmd=WXmalloc(strlen(name)+8);

  sprintf(cmd,"RMD %s\r\n",name);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='2'){
    return 2;
  }
  return 0;
}

int ftp_delete(char* name,connect_data* cd,FILE* log,ftp_check_proc proc){
  char* cmd;
  int ret;

  if(name==NULL)return 0;

  cmd=WXmalloc(strlen(name)+8);

  sprintf(cmd,"DELE %s\r\n",name);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='2'){
    return 2;
  }
  return 0;
}

int ftp_disconnect(connect_data* cd,FILE* log,ftp_check_proc proc){
  int ret;
  signal(SIGALRM,handler);
  alarm(10);
  ret=ftp_command("QUIT\r\n",cd,log,proc);
  interrupt=0;
  alarm(5);
  close(cd->ctrl);
  shutdown(cd->ctrl,2);
  signal(SIGALRM,SIG_IGN);
  return ret;
}

int ftp_command(char* cmd,connect_data* cd,FILE* log,ftp_check_proc proc){
  signal(SIGPIPE,handler);
  if(log)fputs(cmd,log);
  if(write(cd->ctrl,cmd,strlen(cmd))<=0 || interrupt){
    signal(SIGPIPE,SIG_IGN);
    interrupt=0;
    return 10;
  }
  if(proc && (*proc)()){
    signal(SIGPIPE,SIG_IGN);
    interrupt=0;
    return -1;
  }
  return ftp_read_response(cd,log,proc);
}

int ftp_read_line(char** retbuf,connect_data* cd,ftp_check_proc proc){
  static char* buf=NULL;
  static int allocated=0;
  int total;
  char iac[4];
  fd_set selset;
  struct timeval seltime;
  int retval;
  int gotI,gotW,gotD;
  
  if(buf==NULL)buf=WXmalloc(allocated=FTP_MIN_BUF);
  
  signal(SIGPIPE,handler);
  interrupt=0;
  
  *retbuf=buf;
  total=0;
  gotI=0;
  gotW=0;
  gotD=0;

  for(;;){
    FD_ZERO(&selset);
    FD_SET(cd->ctrl,&selset);
    seltime.tv_sec=0;
    seltime.tv_usec=100000;
    
    retval=select(cd->ctrl+1,&selset,NULL,NULL,&seltime);

    if(retval==0){
      if((proc && (*proc)()) || interrupt){
INT_EXIT:
	buf[total]='\0';
	signal(SIGPIPE,SIG_IGN);
	if(interrupt){
	  interrupt=0;
	  return 10;
	} else return -1;
      }
    } else if(retval<0){
      (void)(proc && (*proc)());
      buf[total]='\0';
      signal(SIGPIPE,SIG_IGN);
      interrupt=0;
      return 10;
    } else {
      retval=read(cd->ctrl,&buf[total],1);
      if(retval==0){
	/* EOF */
        (void)(proc && (*proc)());
	buf[total]='\0';
	signal(SIGPIPE,SIG_IGN);
	interrupt=0;
	/*printf("\n\n\nEOF EOF EOF!!!!!!!!!!!!\n\n\n");*/
	return 10;
      }
      if(retval<0){
        (void)(proc && (*proc)());
	buf[total]='\0';
	signal(SIGPIPE,SIG_IGN);
	interrupt=0;
	return 10;
      }

      if(gotI){
	if(buf[total]==(char)WILL || buf[total]==(char)WONT){
	  gotW=1;
	  gotI=0;
	} else if(buf[total]==(char)DO || buf[total]==(char)DONT){
	  gotD=1;
	  gotI=0;
	} else {
	  gotI=0;
	  goto NORMAL_CHAR;
	}
      } else if(gotW || gotD){
        sprintf(iac,"%c%c%c",IAC,gotW?DONT:WONT,buf[total]);
	write(cd->ctrl,iac,3);
	if(interrupt)goto INT_EXIT;
	gotW=0;gotD=0;
      } else {
NORMAL_CHAR:
	switch(buf[total]){
	  case '\0':
	    break;
	  case '\n':
	    (void)(proc && (*proc)());
	    signal(SIGPIPE,SIG_IGN);
	    interrupt=0;
	    buf[++total]='\0';
	    return 0;
          default:
	    total++;
	    if(allocated-total<3){
	      *retbuf=buf=WXrealloc(buf,allocated+=FTP_MIN_BUF);
	    }
	    break;
	}
      }
    }
  }
}


  

int ftp_read_response(connect_data* cd,FILE* log,ftp_check_proc proc){
  int r,len;
  int first;
  char code[4];
  char* p;

  first=1;

  for(;;){
    r=ftp_read_line(&p,cd,proc);
    cd->lastline=p;
    if(log)fputs(p,log);
    if(r){
      if(log)fputc('\n',log);
      return r;
    }
    len=strlen(p);
    if(first){
      if(len<4)return 2;
      if(p[3]=='-'){
	strncpy(code,p,3);
	code[3]=' ';
	first=0;
      } else {
	return 0;
      }
    } else {
      if(len >=4 && strncmp(code,p,4)==0){
	return 0;
      }
    }
  }
}

struct in_addr* ftp_gethosts(char* host,ftp_check_proc proc){
  int fd[2];
  int child;

  if(pipe(fd)<0)return NULL;
  signal(SIGPIPE,handler);
  interrupt=0;
  if((child=fork())==0){
    /* child */
    char** p;
    struct hostent *ht=NULL;

    close(fd[0]);
    ht=gethostbyname(host);
    if(ht==NULL){
      child=-1;
      write(fd[1],&child,sizeof(int));
      close(fd[1]);
      _exit(0);
    }
    p=ht->h_addr_list;
    child=0;
    while(p[0]){
      child++;
      p++;
    }
    p=ht->h_addr_list;
    write(fd[1],&child,sizeof(int));
    while(child){
      write(fd[1],p[0],sizeof(struct sockaddr_in));
      p++;child--;
    }
    close(fd[1]);
    _exit(0);
  } else if(child<0){
    close(fd[0]);
    close(fd[1]);
    return NULL;
  } else {
    /* parent */
    struct timeval tv;
    struct in_addr *buf,*pt;
    fd_set fdset;
    int num;

    close(fd[1]);
    for(;;) {
      FD_ZERO(&fdset);
      FD_SET(fd[0],&fdset);
      tv.tv_sec=0;
      tv.tv_usec=100000;
      if(select(fd[0]+1,&fdset,NULL,NULL,&tv)<=0){
        if((proc && (*proc)()) || interrupt){
	  signal(SIGPIPE,SIG_IGN);
	  kill(child,SIGKILL);
	  interrupt=0;
	  return NULL;
	}
      } else {
        if(read(fd[0],&num,sizeof(int))<=0 || num<=0 || interrupt){
	  close(fd[0]);
	  signal(SIGPIPE,SIG_IGN);
	  kill(child,SIGKILL);
	  interrupt=0;
	  return NULL;
	}
	pt=buf=(struct in_addr*)WXmalloc((num+1)*sizeof(struct in_addr));
	while(num){
	  if(read(fd[0],pt,sizeof(struct in_addr))<=0 || interrupt){
	    close(fd[0]);
	    signal(SIGPIPE,SIG_IGN);
	    kill(child,SIGKILL);
	    interrupt=0;
	    return NULL;
	  }
	  pt++;num--;
	}

	memset(pt,0,sizeof(struct in_addr));
        close(fd[0]);
	signal(SIGPIPE,SIG_IGN);
	kill(child,SIGKILL);
	interrupt=0;
	return buf;
      }
    }
  }
  return NULL;
}

/*#define NOFORK*/

static int myconnect(int sock,struct sockaddr *serv_addr,int addrlen,
    ftp_check_proc proc){
  int p[2];
  int child;
#ifdef NOFORK
  return connect(sock,serv_addr,addrlen)<0;
#else
  signal(SIGPIPE,handler);
  interrupt=0;
  if(pipe(p)==-1)return 1;
  if((child=fork())==-1)return 1;
  if(child==0){
    /* child */
    close(p[0]);
    child=connect(sock,serv_addr,addrlen);
    write(p[1],&child,sizeof(child));
    close(p[1]);
    _exit(0);
  } else {
    /* parent */
    struct timeval tv;
    fd_set fdset;
    int num;

    close(p[1]);
    for(;;) {
      FD_ZERO(&fdset);
      FD_SET(p[0],&fdset);
      tv.tv_sec=0;
      tv.tv_usec=100000;
      if(select(p[0]+1,&fdset,NULL,NULL,&tv)<=0){
        if((proc && (*proc)()) || interrupt){
	  signal(SIGPIPE,SIG_IGN);
	  kill(child,SIGKILL);
	  close(sock);
	  interrupt=0;
	  return 1;
	}
      } else {
        if(read(p[0],&num,sizeof(int))<=0 || num<0 || interrupt){
	  close(p[0]);
	  signal(SIGPIPE,SIG_IGN);
	  kill(child,SIGKILL);
	  close(sock);
	  interrupt=0;
	  return 1;
	}
	return 0;
      }
    }
  }
  return 1;
#endif /* NOFORK */
}

  

int ftp_connect(char* host,int port,connect_data* cd,
    FILE* log,ftp_check_proc proc){
  struct in_addr *ht,*pt;
  int on;
  struct linger lin;
  
  memset(&cd->saddr,(char)0,sizeof(cd->saddr));
  cd->saddr.sin_family=AF_INET;
  cd->saddr.sin_port=htons(port);

  if(proc && (*proc)()){
    interrupt=0;
    return -1;
  }

  pt=ht=NULL;
  /*if(!inet_aton(host,&cd->saddr.sin_addr)){*/
  if((cd->saddr.sin_addr.s_addr=inet_addr(host))==-1){
    pt=ht=ftp_gethosts(host,proc);
    if(ht==NULL){
      return 1;
    }
  }

  if((cd->ctrl=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0 || 
      (proc && (*proc)())){
    WXfree((char*)ht);
    shutdown(cd->ctrl,2);
    interrupt=0;
    return -1;
  }

  on=1;
  setsockopt(cd->ctrl,SOL_SOCKET,SO_REUSEADDR,(void*)&on,sizeof(on));
  if(proc && (*proc)()){
    shutdown(cd->ctrl,2);
    interrupt=0;
    return -1;
  }

  if(ht==NULL){
    if(myconnect(cd->ctrl,(struct sockaddr*)&cd->saddr,sizeof(cd->saddr),
	proc)<0 || (proc && (*proc)())){
      close(cd->ctrl);
      interrupt=0;
      return -1;
    }
  } else {
    while(((char*)pt)[0]){
      memcpy(&cd->saddr.sin_addr,pt,sizeof(struct in_addr));
      if(myconnect(cd->ctrl,(struct sockaddr*)&cd->saddr,sizeof(cd->saddr),
	  proc)>=0){
         goto CONNECTED;
      }
      if(proc && (*proc)()){
	WXfree((char*)ht);
	interrupt=0;
	return -1;
      }
      pt++;
    }
    close(cd->ctrl);
    WXfree((char*)ht);
    interrupt=0;
    return 1;
  }
CONNECTED:
  WXfree((char*)ht);
  setsockopt(cd->ctrl,SOL_SOCKET,SO_OOBINLINE,(void*)&on,sizeof(on));
  setsockopt(cd->ctrl,SOL_SOCKET,SO_KEEPALIVE,(void*)&on,sizeof(on));
  
  lin.l_onoff=0;
  lin.l_linger=0;
  setsockopt(cd->ctrl,SOL_SOCKET,SO_LINGER,(void*)&lin,sizeof(lin));

#ifdef IPTOS_LOWDELAY
  on=IPTOS_LOWDELAY;
  setsockopt(cd->ctrl,IPPROTO_IP,IP_TOS,(void*)&on,sizeof(on));
#endif

  on=sizeof(cd->laddr);

  if(getsockname(cd->ctrl,(struct sockaddr*)&cd->laddr,&on)<0 || 
      (proc && (*proc)())){
    close(cd->ctrl);
    interrupt=0;
    return -1;
  }

  do {
    on=ftp_read_response(cd,log,proc);
    if(on!=0 || (proc && (*proc)())){
      signal(SIGALRM,SIG_IGN);
      interrupt=0;
      return -1;  /*EOF*/
    }
  } while(cd->lastline[0]=='1');
  (void)(proc &&(*proc)());
  interrupt=0;
  if(!cd->lastline[0]=='2')return 1;

  return 0;
}

int ftp_login(char* user,char* pass,char* account,connect_data* cd,FILE* log,
    ftp_check_proc proc){
  char* buf;
  int ret;

  if(proc && (*proc)())return -1;

  if(user==NULL)user="";
  buf=WXmalloc(strlen(user)+8);
  sprintf(buf,"USER %s\r\n",user);
  if((ret=ftp_command(buf,cd,log,proc))!=0 || (proc && (*proc)())){
    WXfree(buf);
    return ret;
  }

  WXfree(buf);
  if(cd->lastline[0]=='2')return 0;
  if(cd->lastline[0]!='3') return 1;

  if(pass==NULL)pass="";
  buf=WXmalloc(strlen(pass)+8);
  sprintf(buf,"PASS %s\r\n",pass);
  if((ret=ftp_command(buf,cd,log,proc))!=0 || (proc && (*proc)())){
    WXfree(buf);
    return ret;
  }
  WXfree(buf);
  if(cd->lastline[0]=='2')return 0;
  if(cd->lastline[0]!='3') return 1;

  if(account==NULL)account="";
  buf=WXmalloc(strlen(account)+8);
  sprintf(buf,"ACCT %s\r\n",account);
  if((ret=ftp_command(buf,cd,log,proc))!=0 || (proc && (*proc)())){
    WXfree(buf);
    return ret;
  }
  WXfree(buf);
  if(cd->lastline[0]=='2')return 0;
  return 1;
}

int ftp_pwd(char** retval,connect_data *cd,FILE* log,ftp_check_proc proc){
  static char* cmd="PWD\r\n";
  char *start,*end;
  int len;

  if((len=ftp_command(cmd,cd,log,proc))!=0){
    return len;
  }
  if(cd->lastline[0]!='2'){
    return 1;
  }
  if((start=strchr(cd->lastline,'\"'))==NULL){
    return 1;
  }
  if((end=strrchr(cd->lastline,'\"'))==NULL || start>=end){
    return 1;
  }
  len=(int)(end-start);
  *retval=WXmalloc(len);
  strncpy(*retval,++start,len-1);
  (*retval)[len-1]='\0';
  return 0;
}

int ftp_sendport_init(connect_data* cd,FILE* log,ftp_check_proc proc){
  int on;
  struct linger lin;
  char cmd[35];

  cd->daddr=cd->laddr;
  cd->daddr.sin_port=0;
  
  if((cd->data=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
    return 1;
  }

  on=1;
  (void)setsockopt(cd->data,SOL_SOCKET,SO_REUSEADDR,(void*)&on,sizeof(on));

  on=1;
  (void)setsockopt(cd->data,SOL_SOCKET,SO_KEEPALIVE,(void*)&on,sizeof(on));
  
  lin.l_onoff=1;
  lin.l_linger=600;
  (void)setsockopt(cd->data,SOL_SOCKET,SO_LINGER,(void*)&lin,sizeof(lin));

#ifdef IPTOS_THROUGHPUT
  on=IPTOS_THROUGHPUT;
  (void)setsockopt(cd->data,IPPROTO_IP,IP_TOS,(void*)&on,sizeof(on));
#endif

  if(bind(cd->data,(struct sockaddr*)&cd->daddr,sizeof(cd->daddr))<0){
    fprintf(log,"%s\n",strerror(errno));
    return 1;
  }

  on=sizeof(cd->daddr);
  if(getsockname(cd->data, (struct sockaddr *)&cd->daddr,&on)<0){
    fprintf(log,"%s\n",strerror(errno));
    return 1;
  }

  if(listen(cd->data,1)<0){
    fprintf(log,"%s\n",strerror(errno));
    return 1;
  }

  sprintf(cmd,"PORT %d,%d,%d,%d,%d,%d\r\n",
      (int)((char*)&cd->daddr.sin_addr)[0] & 0xFF,
      (int)((char*)&cd->daddr.sin_addr)[1] & 0xFF,
      (int)((char*)&cd->daddr.sin_addr)[2] & 0xFF,
      (int)((char*)&cd->daddr.sin_addr)[3] & 0xFF,
      (int)((char*)&cd->daddr.sin_port)[0] & 0xFF,
      (int)((char*)&cd->daddr.sin_port)[1] & 0xFF);

  if((on=ftp_command(cmd,cd,log,proc))!=0){
    return on;
  }

  if(cd->lastline[0]!='2'){
    return 1;
  }

  return 0;
}

int ftp_passive_init(connect_data* cd,FILE* log,ftp_check_proc proc){
  int pd[6];
  unsigned char ad[6];
  int i,on;
  struct linger lin;
  char* s;
 
  if((on=ftp_command("PASV\r\n",cd,log,proc))!=0){
    return on;
  }

  if(cd->lastline[0]!='2'){
    if(cd->lastline[0]=='5'){
      cd->passive=0;
      return ftp_sendport_init(cd,log,proc);
    } else return 1;
  }

  s=&cd->lastline[3];
  while(!isdigit((int)(*s))){
    if(*s=='\0'){
      return 1;
    }
    s++;
  }

  if(sscanf(s,"%d,%d,%d,%d,%d,%d",
      &pd[0],&pd[1],&pd[2],&pd[3],&pd[4],&pd[5])!=6){
    cd->passive=0;
    return ftp_sendport_init(cd,log,proc);
  }

  for(i=0;i<6;i++){
    ad[i]=(unsigned char)(pd[i] & 0xFF);
  }

  memset(&cd->daddr,(char)0,sizeof(cd->daddr));

  memcpy(&cd->daddr.sin_addr,&ad[0],4);
  memcpy(&cd->daddr.sin_port,&ad[4],2);

  cd->daddr.sin_family=AF_INET;

  if((cd->data=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
    return 1;
  }

  on=1;
  setsockopt(cd->data,SOL_SOCKET,SO_REUSEADDR,(void*)&on,sizeof(on));

  if(connect(cd->data,(struct sockaddr*)&cd->daddr,sizeof(cd->daddr))<0){
    cd->passive=0;
    close(cd->data);
    shutdown(cd->data,2);
    return ftp_sendport_init(cd,log,proc);
  }

  setsockopt(cd->data,SOL_SOCKET,SO_KEEPALIVE,(void*)&on,sizeof(on));
  
  lin.l_onoff=1;
  lin.l_linger=600;
  setsockopt(cd->data,SOL_SOCKET,SO_LINGER,(void*)&lin,sizeof(lin));

#ifdef IPTOS_THROUGHPUT
  on=IPTOS_THROUGHPUT;
  setsockopt(cd->data,IPPROTO_IP,IP_TOS,(void*)&on,sizeof(on));
#endif

  return 0;
}

int ftp_data_init(connect_data* cd,FILE* log,ftp_check_proc proc){
  if(cd->passive) return ftp_passive_init(cd,log,proc);
  else return ftp_sendport_init(cd,log,proc);
}

int ftp_passive_connect(connect_data* cd,FILE* log,ftp_check_proc proc){
  return 0;
}

int ftp_sendport_connect(connect_data* cd,FILE* log,ftp_check_proc proc){
  int on,fd;

  on=sizeof(struct sockaddr); 

  if((fd=accept(cd->data,(struct sockaddr*)&cd->daddr,&on))<0){ 
      fprintf(log,"%s\n",strerror(errno));
    return 1;
  }
  close(cd->data);
  cd->data=fd;
  return 0;
}

int ftp_data_connect(connect_data* cd,FILE* log,ftp_check_proc proc){
  if(cd->passive) return ftp_passive_connect(cd,log,proc);
  else return ftp_sendport_connect(cd,log,proc);
}

int ftp_list(char* opt,char* mask,connect_data* cd,FILE* log,
    ftp_check_proc proc){
  char* cmd;
  int ret;

  if((ret=ftp_command("TYPE A\r\n",cd,log,proc))!=0){
    return ret;
  }

  if(cd->lastline[0]!='2'){
    return 1;
  }

  if((ret=ftp_data_init(cd,log,proc))!=0){
    return ret;
  }

  if(opt==NULL)opt="";
  if(mask==NULL)mask="";

  cmd=WXmalloc(strlen(opt)+strlen(mask)+9);

  if(strlen(opt)==0){
    if(strlen(mask)==0){
      sprintf(cmd,"LIST\r\n");
    } else {
      sprintf(cmd,"LIST %s\r\n",mask);
    }
  } else {
    if(strlen(mask)==0){
      sprintf(cmd,"LIST %s\r\n",opt);
    } else {
      sprintf(cmd,"LIST %s %s\r\n",opt,mask);
    }
  }

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='1'){
    return 1;
  }

  return ftp_data_connect(cd,log,proc);
}

int ftp_put(char type,char* local,connect_data* cd,FILE* log,
    ftp_check_proc proc){
  char* cmd;
  int ret;

  if(local==NULL){
    return 0;
  }

  if((ret=ftp_data_init(cd,log,proc))!=0){
    return ret;
  }
  ret=4;

  cmd=WXmalloc(strlen(local)+9);

  sprintf(cmd,"TYPE %c\r\n",type);

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }

  if(cd->lastline[0]!='2'){
    WXfree(cmd);
    return 1;
  }

  sprintf(cmd,"STOR %s\r\n",local);

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='1'){
    return 1;
  }

  return ftp_data_connect(cd,log,proc);
}

int ftp_get(char type,char* remote,connect_data* cd,FILE* log,
    ftp_check_proc proc){
  char* cmd;
  int ret;

  if(remote==NULL){
    return 0;
  }

  if((ret=ftp_data_init(cd,log,proc))!=0){
    return ret;
  }

  cmd=WXmalloc(strlen(remote)+9);

  sprintf(cmd,"TYPE %c\r\n",type);

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }

  if(cd->lastline[0]!='2'){
    WXfree(cmd);
    return 1;
  }

  sprintf(cmd,"RETR %s\r\n",remote);

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='1'){
    return 1;
  }

  return ftp_data_connect(cd,log,proc);
}

int ftp_resume(char type,long size,char* remote,connect_data* cd,FILE* log,
    ftp_check_proc proc){
  char* cmd;
  int ret;

  if(remote==NULL){
    return 0;
  }

  if((ret=ftp_data_init(cd,log,proc))!=0){
    return ret;
  }

  cmd=WXmalloc(strlen(remote)+40);

  sprintf(cmd,"TYPE %c\r\n",type);

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }

  if(cd->lastline[0]!='2'){
    WXfree(cmd);
    return 1;
  }

  sprintf(cmd,"REST %ld\r\n",size);

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }

  if(cd->lastline[0]!='3'){
    return 1;
  }

  sprintf(cmd,"RETR %s\r\n",remote);

  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='1'){
    return 1;
  }

  return ftp_data_connect(cd,log,proc);
}

int ftp_abort_data(connect_data* cd,FILE* log,ftp_check_proc proc){
  char buf[3];
  int ret;

  signal(SIGPIPE,handler);
  signal(SIGALRM,handler);

  sprintf(buf,"%c%c",IAC,IP);
  if(write(cd->ctrl,buf,2)<=0 || interrupt){
    shutdown(cd->data,2);
    signal(SIGPIPE,SIG_IGN);
    signal(SIGALRM,SIG_IGN);
    interrupt=0;
    return 10;
  }

  sprintf(buf,"%c%c",IAC,DM);
  if(send(cd->ctrl,buf,2,MSG_OOB)<=0 || interrupt){
    shutdown(cd->data,2);
    signal(SIGPIPE,SIG_IGN);
    signal(SIGALRM,SIG_IGN);
    interrupt=0;
    return 10;
  }

  ret=ftp_command("ABOR\r\n",cd,log,proc);
  shutdown(cd->data,2);
  close(cd->data);
  signal(SIGPIPE,SIG_IGN);
  signal(SIGALRM,SIG_IGN);
  interrupt=0;
  return ftp_read_response(cd,log,proc);
}

int ftp_close_data(connect_data* cd,FILE* log,ftp_check_proc proc){
  shutdown(cd->data,2);
  close(cd->data);
  return ftp_read_response(cd,log,proc);
}

int ftp_chdir(char* dir,connect_data* cd,FILE* log,ftp_check_proc proc){
  char* cmd;
  int ret;

  if(dir==NULL)return 0;

  cmd=WXmalloc(strlen(dir)+7);

  sprintf(cmd,"CWD %s\r\n",dir);
  if((ret=ftp_command(cmd,cd,log,proc))!=0){
    WXfree(cmd);
    return ret;
  }
  WXfree(cmd);

  if(cd->lastline[0]!='2'){
    return 2;
  }
  return 0;
}
