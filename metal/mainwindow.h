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

private slots:
  void on_n_points_sliderReleased();
  void on_zExpression_returnPressed();

private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
