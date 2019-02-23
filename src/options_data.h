/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef OPTIONS_DATA_H
#define OPTIONS_DATA_H

#define OPTIONS_DATA_VERSION_LENGTH 3

typedef struct _options_data {
  /* General */
  char* anonpass;
  char deletions;
  char recurse_del;
  /* Display */
  char show_session;
  char columnadjustment;
  char* maxwidth;
  char show_progress;
  char* progress_treshold;
  /* Sound */
  char xferbeep;
  /* Advanced */
  char* redial;
  char* delay;
} options_data;

options_data* create_options_data(options_data*);
void destroy_options_data(options_data*);
void empty_options_data(options_data*);
int read_options_data(char*,options_data**);
int write_options_data(char*,options_data*);

#endif /* OPTIONS_DATA_H */
