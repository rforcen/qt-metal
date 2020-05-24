#pragma once

#include <simd/simd.h>
#include <vm.h>

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <iterator>

typedef uint32_t color;
typedef simd_float4 range;
typedef struct {
  int x, y;
  color color;
  int _align_to_uint4;
} point;

class Parameter;

class metal_device {
 public:
  metal_device();
  bool load_library(QString lib_name);
  bool load_library_from_data(QByteArray data);
  bool load_metallib(QString file_name);
  void compileFunc(QString func);
  void prepFunc(QString func);
  void genEncoder(), run();
  void* createBuffer(void* data, size_t len);
  size_t sz2Page(size_t sz, size_t szof);
  int sz2Page(size_t sz);
  void* newData(size_t size);
  void deleteData(size_t size, void* data);

  size_t memoryUsage();

  void* createGPUPrivateBuffer(void* data, size_t len);
  void* copyBuffer(void* data, size_t len);
  void runThreadsWidth(size_t width, size_t height);
  void runThreads3d(size_t width, size_t height, size_t depth);

  void copyContentsOn(void* data, void* buffer, size_t length);
  void setBufferParam(void* buffer, uint index);
  void setBytesParam(void* data, uint length, int index);
  void setIntParam(void* data, int index);
  void setFloatParam(void* data, int index);

  void set_int(int i, int index) { setIntParam(&i, index); }
  void set_float(float f, int index) { setFloatParam(&f, index); }

  void call(QString func, int w, int h, QList<Parameter> pl);

 private:
  void* dev = nullptr;
};
