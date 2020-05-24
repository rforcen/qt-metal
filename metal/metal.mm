// metal device c++ wrapper

#include <MetalDevice.h>
#include <metal.h>
#include <parameter.h>

void metal_device::call(QString func, int w, int h, QList<Parameter> pl) {
  compileFunc(func);

  int index = 0;
  for (auto& p : pl) p.set_buffer(this, index++);

  runThreadsWidth(w, h);
}

#define GenFunc(x) \
  void metal_device::x() { [(__bridge(MetalDevice*) dev) x]; }
#define GenFunc1(x, p0) \
  void metal_device::x() { [(__bridge(MetalDevice*) dev) x:p0]; }

inline NSString* qs2ns(QString s) { return [NSString stringWithUTF8String:s.toUtf8().data()]; }

metal_device::metal_device() { dev = (__bridge void*)[MetalDevice init]; }

bool metal_device::load_library(QString lib_name) {
  return [(__bridge(MetalDevice*) dev) load_library:qs2ns(lib_name)];
}

bool metal_device::load_metallib(QString file_name) {
  bool ok;
  QFile f(file_name);
  if ((ok = f.open(QIODevice::ReadOnly))) load_library_from_data(f.readAll());
  return ok;
}

bool metal_device::load_library_from_data(QByteArray data) {
  return [(__bridge(MetalDevice*) dev) load_library_from_data:data.data() length:data.size()];
}

void metal_device::compileFunc(QString func) {
  [(__bridge(MetalDevice*) dev) compileFunc:qs2ns(func)];
}

void metal_device::prepFunc(QString func) { [(__bridge(MetalDevice*) dev) prepFunc:qs2ns(func)]; }

void metal_device::run() { [(__bridge(MetalDevice*) dev) run]; }

void metal_device::genEncoder() { [(__bridge(MetalDevice*) dev) genEncoder]; }

void* metal_device::createBuffer(void* data, size_t len) {
  return [(__bridge(MetalDevice*) dev) createBuffer:data length:len];
}

size_t metal_device::sz2Page(size_t sz, size_t szof) {
  return [(__bridge(MetalDevice*) dev) sz2Page:sz szof:szof];
}

int metal_device::sz2Page(size_t sz) { return [(__bridge(MetalDevice*) dev) sz2Page:sz]; }

void* metal_device::newData(size_t size) { return [(__bridge(MetalDevice*) dev) newData:size]; }

void metal_device::deleteData(size_t size, void* data) {
  [(__bridge(MetalDevice*) dev) deleteData:size data:data];
}

size_t metal_device::memoryUsage() { return (size_t)[MetalDevice memoryUsage]; }

void* metal_device::createGPUPrivateBuffer(void* data, size_t len) {
  return [(__bridge(MetalDevice*) dev) createGPUPrivateBuffer:data length:len];
}

void* metal_device::copyBuffer(void* data, size_t len) {
  return [(__bridge(MetalDevice*) dev) copyBuffer:data length:len];
}

void metal_device::runThreadsWidth(size_t width, size_t height) {
  [(__bridge(MetalDevice*) dev) runThreadsWidth:width height:height];
}

void metal_device::runThreads3d(size_t width, size_t height, size_t depth) {
  [(__bridge(MetalDevice*) dev) runThreads3d:width height:height depth:depth];
}

void metal_device::copyContentsOn(void* data, void* buffer, size_t length) {
  [(__bridge(MetalDevice*) dev) copyContentsOn:data buffer:(id<MTLBuffer>)buffer length:length];
}

void metal_device::setBufferParam(void* buffer, uint index) {
  [(__bridge(MetalDevice*) dev) setBufferParam:(id<MTLBuffer>)buffer index:index];
}

void metal_device::setBytesParam(void* data, uint length, int index) {
  [(__bridge(MetalDevice*) dev) setBytesParam:data length:length index:index];
}

void metal_device::setIntParam(void* data, int index) {
  [(__bridge(MetalDevice*) dev) setIntParam:data index:index];
}

void metal_device::setFloatParam(void* data, int index) {
  [(__bridge(MetalDevice*) dev) setFloatParam:data index:index];
}
