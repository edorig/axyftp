/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef LITTLE_DIALOGS_H
#define LITTLE_DIALOGS_H

#include"axyftp.h"
WXwidget popup_prompt_dialog(WXwidget,char*);
WXwidget popup_error_dialog(WXwidget,char*);
WXwidget popup_warning_dialog(WXwidget,char*);
WXwidget popup_reconnect_dialog(WXwidget);

#endif /* LITTLE_DIALOGS_H */
