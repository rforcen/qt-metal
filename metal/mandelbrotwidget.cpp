#include "mandelbrotwidget.h"

MandelbrotWidget::MandelbrotWidget(QWidget* parent) : QWidget(parent) {}

void MandelbrotWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  _c->start();

  if (!_c->zc->err) paint(p);

  _c->finished();
}

void MandelbrotWidget::paint(QPainter& p) {
  w = p.device()->width();
  h = p.device()->height();

  VM_Vect<color> pic(w * h);  // pic buffer -> w x h
  VM_Vect<__byte> code(_c->zc->getCode(), _c->zc->codeLength());
  VM_Vect<float> constants(_c->zc->getConsts(), _c->zc->constsLength());

  range _range{-2, -2, 2, 2};

  _c->metal->call("MandelbrotzFunc", w, h,
                  {&pic, &_range, w, h, iters, &code, &constants});

  // pic is available now -> draw
  p.drawImage(0, 0, QImage((uchar*)pic.data(), w, h, QImage::Format_ARGB32));
}
