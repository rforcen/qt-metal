#ifndef METALWIDGET_H
#define METALWIDGET_H

#include <VM_Vect.h>
#include <metal.h>

#include <QPainter>
#include <QWidget>

class MetalWidget : public QWidget {
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

  // pic, points, colors
  VM_Vect<point> points;
  VM_Vect<color> pic;

  int w, h;
  int n_points = 1500;

  MetalWidget(QWidget* parent = nullptr);
  ~MetalWidget() {}

  void paint(QPainter& p);

  Metal* metal = nullptr;

 protected:
  void paintEvent(QPaintEvent*) override;
};

#endif  // METALWIDGET_H
