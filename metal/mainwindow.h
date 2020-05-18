#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <metal.h>

#include <QGraphicsPixmapItem>
#include <QMainWindow>
#include <QResizeEvent>
#include <algorithm>
#include <mtlpp/mtlpp.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
