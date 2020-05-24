#pragma once

#include <common.h>

#include <QPainter>
#include <QTime>
#include <QWidget>

class MandelbrotWidget : public QWidget {
  Q_OBJECT

 public:
  explicit MandelbrotWidget(QWidget *parent = nullptr);

 private:
  int w, h, iters = 256;

  void paint(QPainter &p);

 protected:
  void paintEvent(QPaintEvent *) override;
 signals:
};
