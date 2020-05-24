#pragma once

#include <VM_Vect.h>
#include <metal.h>
#include <parameter.h>
#include <zcompiler/zCompiler.h>

#include <QTime>

class Common {
 public:
  Common() {
    metal = new metal_device;
    metal->load_metallib(
        ":/metal source/qtmetal.metallib");  // in resource file
    zc = new zCompiler;
  }
  ~Common() {
    delete metal;
    delete zc;
  }

  bool compile(QString expr) { return !zc->Compile(expr.toStdString()); }
  int get_lap() { return lap; }
  void start() { time.start(); }
  void finished() { lap = time.elapsed();}

  metal_device *metal;
  zCompiler *zc;
  QTime time;
  int lap = 0;
};

extern Common *_c;
