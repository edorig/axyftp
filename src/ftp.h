/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef FTP_H
#define FTP_H

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

typedef struct _connect_data {
  /* control connection */
  struct sockaddr_in saddr; /* server */
  struct sockaddr_in laddr; /* local */
  int ctrl;

  /* data connection */
  int passive; /* flag whether passive supported */
  struct sockaddr_in daddr;
  int data;

  /* last response */
  char* lastline;
} connect_data;

typedef int (*ftp_check_proc)();

struct in_addr* ftp_gethosts(char*,ftp_check_proc);
int ftp_syst(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_rename(char*,char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_site(char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_mkdir(char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_delete(char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_rmdir(char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_chdir(char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_command(char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_read_response(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_disconnect(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_connect(char*,int,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_login(char*,char*,char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_pwd(char**,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_size(long*,char,char*,connect_data*,FILE*,ftp_check_proc);
int ftp_passive_init(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_sendport_init(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_data_init(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_passive_connect(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_sendport_connect(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_data_connect(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_list(char*,char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_get(char,char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_resume(char,long,char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_put(char,char*,struct _connect_data*,FILE*,ftp_check_proc);
int ftp_close_data(struct _connect_data*,FILE*,ftp_check_proc);
int ftp_abort_data(struct _connect_data*,FILE*,ftp_check_proc);

#endif /* FTP_H */
