/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "ftp.h"
#include"session_data.h"
#include"fileinfo.h"

int show_help(int);
int show_log();
int check_for_interrupt();
int check_for_small_interrupt();
int update_local(char*);
int update_remote(char*);
int download_file(fileinfo* volatile,char volatile);
int upload_file(fileinfo*,char);
void end_session();
void start_session(session_data *,char*);
int chg_local_dir(int);
int chg_remote_dir(int);
int chg_remote_dirname(char*);
int chg_local_dirname(char*);
int delete_local_num(int);
int delete_local(char*);
int rmdir_local(char*);
int view_remote_num(int);
int view_local_num(int);
int delete_remote_num(int);
int delete_remote(char*);
int rmdir_remote(char*);
int mkdir_local(char*);
int mkdir_remote(char*);
int exec_local(char*);
int exec_remote(char*);
int rename_local_num(int);
int rename_local(char*,char*);
int rename_remote_num(int);
int rename_remote(char*,char*);
int show_local_dirinfo();
int show_remote_dirinfo();

#endif /* FUNCTIONS_H */
