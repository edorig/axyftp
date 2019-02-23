/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef FILEINFO_H
#define FILEINFO_H

typedef struct _fileinfo {
  char perms[11];
  int inode;
  char *user;
  char *group;
  long size;
  char month[4];
  int day;
  char time_year[6];
  char *name;
  char *link;
  char date[9];
  char size_str[20];
  char time[6];
  struct _fileinfo * next;
} fileinfo;

char* get_fileinfo_string(fileinfo*);
fileinfo* create_fileinfo(char*);
fileinfo* create_nt_fileinfo(char*);
fileinfo* create_dummy_dir(char*,fileinfo*);
void destroy_fileinfo(fileinfo*);
fileinfo* copy_fileinfo(fileinfo*);

#endif /* FILEINFO_H */
