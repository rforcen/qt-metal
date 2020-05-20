#include "metaldevicewidget.h"

MetalDeviceWidget::MetalDeviceWidget(QWidget* parent) : QWidget(parent) {
  metal = new metal_device;

  points.resize(1500);
}

void MetalDeviceWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  time.start();

  paint(p);

  lap = time.elapsed();
}

void MetalDeviceWidget::paint(QPainter& p) {
  w = p.device()->width();
  h = p.device()->height();

  // generate random points
  for (int i = 0; i < points.size(); i++)
    points[i] = point{
        (rand() % (w - 10)) + 5, (rand() % (h - 10)) + 5,
        makeColor(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50), 0};

  // pic buffer -> w x h
  pic.resize(w * h);

  // color tiles
  metal->compileFunc("Voronoi");

  pic.create_copy_buffer(metal);
  points.create_copy_buffer(metal);

  pic.set_buffer(0);
  points.set_buffer(1);
  metal->set_int(points.size(), 2);

  metal->runThreadsWidth(w, h);

  // points
  metal->compileFunc("setPointBox");

  pic.create_copy_buffer(metal);
  points.create_copy_buffer(metal);

  pic.set_buffer(0);
  points.set_buffer(1);
  metal->set_int(w, 2);

  metal->runThreadsWidth(points.size(), 1);

  // pic is available now -> draw
  p.drawImage(0, 0, QImage((uchar*)pic.data(), w, h, QImage::Format_ARGB32));
}
