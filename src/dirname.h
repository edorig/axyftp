/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef DIRNAME_H
#define DIRNAME_H

#include"axyftp.h"
#include"dirinfo.h"

WXwidget create_dirname(WXwidget,int);
void update_dirname(WXwidget,dirinfo *);
void clear_dirname(WXwidget);

#endif /* DIRNAME_H */
