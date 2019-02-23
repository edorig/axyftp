/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef PROMPT_DIALOGS_H
#define PROMPT_DIALOGS_H

#include"axyftp.h"
void init_xfer_dialog(WXwidget,char*);
WXwidget create_xfer_dialog(WXwidget);
void init_delete_dialog(WXwidget,char*);
WXwidget create_delete_dialog(WXwidget);
void init_rename_dialog(WXwidget,char*,char*);
WXwidget create_rename_dialog(WXwidget,int);
WXwidget create_exec_dialog(WXwidget,int);
WXwidget create_mkdir_dialog(WXwidget,int);

#endif /* PROMPT_DIALOGS_H */
