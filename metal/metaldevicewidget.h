#pragma once

#include <common.h>

#include <QPainter>
#include <QWidget>

class MetalDeviceWidget : public QWidget {
  Q_OBJECT
 public:
  inline color makeColor(int r, int g, int b) {
    return 0xff000000 | (r | (g << 8) | (b << 16));
  }

  void set_n_points(int n_points, bool rep = true) {
    points.resize(n_points);
    first = true;
    gen_points();
    if (rep) repaint();
  }

  void gen_points() {  // in w, h range
                       // generate random points
    if (first)
      for (auto& p : points)
        p = point{
            (rand() % (w - 10)) + 5, (rand() % (h - 10)) + 5,
            makeColor(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50),
            0};
  }

  void fit_points() {
    if (!first)
      for (auto& p : points) {
        p.x *= (float)w / w_old;
        p.y *= (float)h / h_old;
      }
    first = false;
  }

  void get_geo(QPainter& p) {
    w_old = w;
    h_old = h;

    w = p.device()->width();
    h = p.device()->height();
  }

  // pic, points, colors
  VM_Vect<point> points;

  int w = 0, h = 0, w_old, h_old;
  bool first = true;
  int inline wh() { return w * h; }

  MetalDeviceWidget(QWidget* parent = nullptr);
  ~MetalDeviceWidget() {}

  void paint(QPainter& p);

 protected:
  void paintEvent(QPaintEvent*) override;
};
