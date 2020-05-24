#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  on_zExpression_returnPressed();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_n_points_sliderReleased() {
  int np = ui->n_points->value();
  ui->widget_voronoi->set_n_points(np);

  ui->statusbar->showMessage(QString("lap for %1 points: %2 ms")
                                 .arg(np)
                                 .arg(_c->get_lap()));
}

void MainWindow::on_zExpression_returnPressed() {
  if (_c->compile(ui->zExpression->text())) {
    ui->tabWidget->currentWidget()->findChild<QWidget *>()->repaint();
    ui->statusbar->showMessage(QString("ok, lap: %1 ms").arg(_c->get_lap()));
  } else
    ui->statusbar->showMessage("syntax error");
}
