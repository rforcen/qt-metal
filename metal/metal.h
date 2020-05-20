#pragma once

#include <vm.h>

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <iterator>

class metal_device {
 public:
  metal_device();
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

  void set_int(int i, int index) { setIntParam(&i, index); }

 private:
  void* dev = nullptr;
};
