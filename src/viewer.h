/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef VIEWER_H
#define VIEWER_H

#include"dirinfo.h"
#include"axyftp.h"
WXwidget create_viewer(WXwidget,char*);
int fill_viewer(WXwidget,int);
void fill_dirinfo(WXwidget,dirinfo*);

#endif /* VIEWER_H */
