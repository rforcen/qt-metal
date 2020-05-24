#include "domaincoloringwidget.h"

DomainColoringWidget::DomainColoringWidget(QWidget* parent) : QWidget(parent) {}

void DomainColoringWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  _c->start();

  if (!_c->zc->err) paint(p);

  _c->finished();
}

void DomainColoringWidget::paint(QPainter& p) {
  w = p.device()->width();
  h = p.device()->height();

  VM_Vect<color> pic(w * h);  // pic buffer -> w x h
  VM_Vect<__byte> code(_c->zc->getCode(), _c->zc->codeLength());
  VM_Vect<float> constants(_c->zc->getConsts(), _c->zc->constsLength());
  simd_int4 geo{0, 0, w, h};

  _c->metal->call("domainColorzFunc", w, h, {&pic, &geo, &code, &constants});

  // pic is available now -> draw
  p.drawImage(0, 0, QImage((uchar*)pic.data(), w, h, QImage::Format_ARGB32));
}
