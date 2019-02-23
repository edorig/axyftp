/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef MULTI_H
#define MULTI_H
#include"setjmp.h"

int get_files(int*,char);
int put_files(int*,char);
int delete_remote_files(int*);
int delete_local_files(int*);

extern sigjmp_buf jmp_down_env;

#endif /* MULTI_H */
