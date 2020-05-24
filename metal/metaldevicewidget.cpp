#include "metaldevicewidget.h"

#include <memory>

MetalDeviceWidget::MetalDeviceWidget(QWidget* parent) : QWidget(parent) {
  points.resize(1500);
}

void MetalDeviceWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  _c->start();

  paint(p);

  _c->finished();
}

void MetalDeviceWidget::paint(QPainter& p) {
  get_geo(p);

  gen_points();  // new set as h,w may have changed
  fit_points();

  // pic buffer -> w x h
  VM_Vect<color> pic(wh());

  _c->metal->call("Voronoi", w, h, {&pic, &points, points.size()});
  _c->metal->call("setPointBox", points.size(), 1, {&pic, &points, w});

  // pic is available now -> draw
  p.drawImage(0, 0, QImage(pic.udata(), w, h, QImage::Format_ARGB32));
}
