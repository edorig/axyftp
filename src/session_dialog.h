/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef SESSION_DIALOG_H
#define SESSION_DIALOG_H

#define MAX_VISIBLE_ITEMS 10
#include"axyftp.h"
#include"session_data.h"

WXwidget create_session_dialog(WXwidget);

void fetch_session_data(WXwidget,session_data*);
void put_session_data(WXwidget,session_data*,char);
void update_session_dialog(int,char);
void set_profile_strings(WXwidget);

#endif /* SESSION_DIALOG_H */
