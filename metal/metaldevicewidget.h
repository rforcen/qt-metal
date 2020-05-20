#pragma once

#include <VM_Vect.h>
#include <metal.h>

#include <QPainter>
#include <QWidget>
#include <QTime>

class MetalDeviceWidget : public QWidget {
  Q_OBJECT
 public:
  typedef uint32_t color;
  typedef struct {
    int x, y;
    color color;
    int _align_to_uint4;
  } point;

  inline color makeColor(int r, int g, int b) {
    return 0xff000000 | (r | (g << 8) | (b << 16));
  }

  void set_n_points(int n_points) {
    points.resize(n_points);
    repaint();
  }

  int get_lap() { return lap;}

  // pic, points, colors
  VM_Vect<point> points;
  VM_Vect<color> pic;

  int w, h;

  MetalDeviceWidget(QWidget* parent = nullptr);
  ~MetalDeviceWidget() {}

  void paint(QPainter& p);

  metal_device* metal = nullptr;
  QTime time;
  int lap=0;

 protected:
  void paintEvent(QPaintEvent*) override;
};
