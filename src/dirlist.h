/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef DIRLIST_H
#define DIRLIST_H

#include"axyftp.h"
#include"dirinfo.h"

WXwidget create_dirlist(WXwidget,int);
void clear_dirlist(WXwidget);
void update_dirlist(WXwidget,dirinfo*);

int* get_selected_rows(WXwidget);
int get_selected_row(WXwidget);
#define NONE -1
#define MANY -2

void cancel_table_update(WXwidget);
void restore_table_update(WXwidget);
#endif /* DIRLIST_H */
