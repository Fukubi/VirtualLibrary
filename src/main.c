#include "book.h"
#include "book_manager.h"
#include "listbook.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_window.h"

int main(int argc, char *argv[]) {
  GtkApplication *app;
  int status;

  app =
      gtk_application_new("treinaweb.helloworld", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activateMainWindow), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}