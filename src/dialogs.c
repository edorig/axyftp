/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"options_dialog.h"
#include"session_dialog.h"
#include"about_dialog.h"
#include"progress_dialog.h"
#include"prompt_dialogs.h"
#include"axyftp.h"


void make_dialogs(WXwidget parent){
  appdata.session=create_session_dialog(parent);
  appdata.options=create_options_dialog(parent);
 
  /*appdata.progress=create_progress_dialog(parent);*/
  appdata.about=create_about_dialog(parent);
  appdata.mkdir_remote=create_mkdir_dialog(parent,REMOTE);
  appdata.mkdir_local=create_mkdir_dialog(parent,LOCAL);
  appdata.exec_remote=create_exec_dialog(parent,REMOTE);
  appdata.exec_local=create_exec_dialog(parent,LOCAL);
  appdata.rename_remote=create_rename_dialog(parent,REMOTE);
  appdata.rename_local=create_rename_dialog(parent,LOCAL);
  /*appdata.delete_remote=create_delete_dialog(parent,REMOTE);
  appdata.delete_local=create_delete_dialog(parent,LOCAL);*/
  appdata.delete_dialog=create_delete_dialog(parent);
  appdata.xfer_dialog=create_xfer_dialog(parent);
  appdata.progress=create_progress_dialog(parent);
  appdata.progress_shown=0; 
}
