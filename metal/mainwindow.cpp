#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_n_points_sliderReleased() {
  int np = ui->n_points->value();

  ui->widget->set_n_points(np);
  ui->n_points->setToolTip(QString::number(np));

  ui->statusbar->showMessage(
      QString("lap for %1 points: %2 ms").arg(np).arg(ui->widget->get_lap()));
}
