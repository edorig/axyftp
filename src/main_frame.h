/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include"axyftp.h"
WXwidget create_main_frame(WXwidget);

#ifdef BUILD_MOTIF
int adjust_main_frame(WXwidget);
#endif

#endif /* MAIN_FRAME_H */
