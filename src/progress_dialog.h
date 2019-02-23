/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include"axyftp.h"
WXwidget create_progress_dialog(WXwidget);
void init_progress_dialog(WXwidget,char*,long);
void update_progress_dialog(WXwidget,int,long,float,long,long);
void hide_progress_dialog(WXwidget);
void show_progress_dialog(WXwidget);

#endif /* PROGRESS_DIALOG_H */
