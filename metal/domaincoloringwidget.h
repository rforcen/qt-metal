#pragma once

#include <common.h>

#include <QPainter>
#include <QWidget>

class DomainColoringWidget : public QWidget {
  Q_OBJECT
 public:
  explicit DomainColoringWidget(QWidget *parent = nullptr);

  ~DomainColoringWidget() { }


 private:
  int w, h, iters = 256;

  void paint(QPainter &p);

 protected:
  void paintEvent(QPaintEvent *) override;
 signals:
};
