/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef FTP_XFER_H
#define FTP_XFER_H

typedef int (*ftp_xfer_proc)(int,void*);

int ftp_xfer_get(char,int,int,ftp_xfer_proc,void*);
int ftp_xfer_put(char,int,int,ftp_xfer_proc,void*);
#endif /* FTP_XFER_H */
