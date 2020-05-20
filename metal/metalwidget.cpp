#include "metalwidget.h"

MetalWidget::MetalWidget(QWidget* parent) : QWidget(parent) {
  metal = new Metal;
  metal->compile_func("Voronoi");
  metal->compile_func("setPointBox");

  points.resize(n_points);
  points.create_buffer(metal);
}

void MetalWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  paint(p);
}

void MetalWidget::paint(QPainter& p) {
  w = p.device()->width();
  h = p.device()->height();

  // generate random points
  for (int i = 0; i < n_points; i++)
    points[i] = point{
        (rand() % (w - 10)) + 5, (rand() % (h - 10)) + 5,
        makeColor(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50), 0};

  // pic buffer -> w x h
  pic.resize(w * h);
  pic.create_buffer(metal);

  // color tiles
  metal->set_func("Voronoi");
  pic.set_buffer(0);  // kernel func parameters(pic, points, n_points)
  points.set_buffer(1);
  metal->set_int(n_points, 2);

  metal->run(w, h);

  // points
  metal->set_func("setPointBox");
  pic.set_buffer(0);
  points.set_buffer(1);
  metal->set_int(w, 2);

  metal->run(n_points);

  // pic is available now
  p.drawImage(0, 0, QImage((uchar*)pic.data(), w, h, QImage::Format_ARGB32));
}
