/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef SESSION_DATA_H
#define SESSION_DATA_H

#define SESSION_DATA_VERSION_LENGTH 3

typedef struct _session_data {
  char* profile;
  char* host;
  char* user;
  char* pass;
  char* account;
  char* comment;
  char anon;
  char save;

  char* remdir;
  char* locdir;
  char* initcom;
  char* locmask;
  char* remmask;

  char* port;

  struct _session_data* next;
} session_data;

session_data* create_session_data(session_data*);
void destroy_session_data(session_data*);
void empty_session_data(session_data*);
int read_session_data(char*,session_data*);
int write_session_data(char*,session_data*);
void print_session_data(struct _session_data*);

#endif /* SESSION_DATA_H */
