#include <common.h>

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  _c = new Common;

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
