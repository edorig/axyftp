/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef DIRINFO_H
#define DIRINFO_H

#include"fileinfo.h"

typedef struct _dirinfo {
  char* dir;
  int total;
  struct _fileinfo** files;
} dirinfo;

dirinfo* create_remote_dirinfo(char*);
dirinfo* create_local_dirinfo(char*);
void destroy_dirinfo(dirinfo*);
dirinfo* select_dirinfo(dirinfo*,int*);

#endif /* DIRINFO_H */
