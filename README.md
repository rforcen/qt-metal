# qt-metal
qt metal c++ interface

use apple metal resources in a convenient qt wrapper, 

virtual memory manager is implemented in a VM_Vect vector alike class,

interfacing with qt widget is simple using VM_Vect's and metal parametric call:

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

i've also added a metal.xml sytax highlighting file.
obviously this is only for macos.

inspired in Nikolay's mtlpp:  https://github.com/naleksiev/mtlpp
