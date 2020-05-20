#pragma once

#include <vm.h>

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <iterator>
#include <mtlpp/mtlpp.hpp>

class Metal {
 public:
  mtlpp::Device device;
  mtlpp::Library library;
  mtlpp::Function kFunc;
  mtlpp::ComputePipelineState pipeline;
  mtlpp::CommandQueue commandQueue;
  mtlpp::CommandBuffer commandBuffer;
  mtlpp::ComputeCommandEncoder commandEncoder;

  typedef struct {
    mtlpp::Function kFunc;
    mtlpp::ComputePipelineState pipeline;
    mtlpp::CommandQueue commandQueue;
  } func_stat;

  QMap<QString, func_stat> funcs;

  bool ok = true;

  typedef struct {
    mtlpp::Buffer buff;
    void* vb;
    int size;
    int index;
  } multi_buffer;
  QVector<multi_buffer> vbuff;

  typedef struct {
    void* buff;
    int byte_len;
  } buffer_st;

  Metal() {
    device = mtlpp::Device::CreateSystemDefaultDevice();
    load_default_library(); // load 'default.metallib'
  }

  Metal(QString program) {
    device = mtlpp::Device::CreateSystemDefaultDevice();
    compile_resource(program);
  }
  ~Metal() {}

  // compiled .metallib file, use full path or copy to executable folder
  bool load_library(QString file) {
    ns::Error error;
    library = device.NewLibrary(
        ns::String((file + ".metallib").toUtf8().data()), &error);

    return ok = (error.GetCode() == 0);
  }
  bool load_default_library() {
    load_library("default");
    return ok;
  }
  bool compile_resource(QString metal_resource_file) {
    ok = false;

    QFile f(metal_resource_file);

    if (f.open(QIODevice::ReadOnly)) {
      QString s;
      QTextStream s1(&f);
      s.append(s1.readAll());
      ok = compile(s.toUtf8().data());
    }
    return ok;
  }

  bool compile_resource(QString metal_resource_file, QString func) {
    ok = false;

    QFile f(metal_resource_file);

    if (f.open(QIODevice::ReadOnly)) {
      QString s;
      QTextStream s1(&f);
      s.append(s1.readAll());
      ok = compile(s.toUtf8().data(), func.toUtf8().data());
    }
    return ok;
  }

  bool compile(const char* program) {
    library = device.NewLibrary(program, mtlpp::CompileOptions(), nullptr);

    return ok;
  }

  bool compile_func(QString func) {
    if (funcs.contains(func)) {
      auto& f = funcs[func];

      kFunc = f.kFunc;
      pipeline = f.pipeline;
      commandQueue = f.commandQueue;
    } else {
      kFunc = library.NewFunction(func.toUtf8().data());
      pipeline = device.NewComputePipelineState(kFunc, nullptr);
      commandQueue = device.NewCommandQueue();

      funcs[func] = {kFunc, pipeline, commandQueue};
    }

    commands();

    clear_buffers();
    return ok;
  }
  bool set_func(QString func) {
    if (funcs.contains(func)) {
      auto& f = funcs[func];

      kFunc = f.kFunc;
      pipeline = f.pipeline;
      commandQueue = f.commandQueue;

      commands();

      clear_buffers();
      ok = true;
    } else
      ok = false;
    return ok;
  }
  bool compile(const char* program, const char* func) {
    if (funcs.contains(func)) {
      auto& f = funcs[func];

      kFunc = f.kFunc;
      pipeline = f.pipeline;
      commandQueue = f.commandQueue;
    } else {
      library = device.NewLibrary(program, mtlpp::CompileOptions(), nullptr);
      kFunc = library.NewFunction(func);
      pipeline = device.NewComputePipelineState(kFunc, nullptr);
      commandQueue = device.NewCommandQueue();

      funcs[func] = {kFunc, pipeline, commandQueue};
    }

    commands();

    clear_buffers();
    return ok;
  }

  void commands() {
    commandBuffer = commandQueue.CommandBuffer();
    commandEncoder = commandBuffer.ComputeCommandEncoder();
    commandEncoder.SetComputePipelineState(pipeline);
  }

  mtlpp::Buffer create_buffer(void* buff, int byte_len) {
    return device.NewBuffer(buff, VM::size_2_page(byte_len),
                            mtlpp::ResourceOptions::StorageModeShared, nullptr);
  }

  void add_buffer(void* buff, int byte_len) {
    vbuff.push_back(
        {create_buffer(buff, byte_len), buff, byte_len, vbuff.size()});
  }

  void add_buffers(QVector<buffer_st> vb) {
    vbuff.clear();
    for (auto& v : vb) add_buffer(v.buff, v.byte_len);
    set_buffer();
  }

  template <class Iterable>
  void add_buffer(const Iterable& iterable) {
    add_buffer((void*)iterable.begin(), bytes(iterable));
  }

  void clear_buffers() { vbuff.clear(); }

  void add_int(int i) { add_buffer(&i, sizeof(i)); }

  void set_buffer() {
    for (int index = 0; index < vbuff.size(); index++)
      set_buffer(vbuff[index].buff, index);
  }

  void set_buffer(mtlpp::Buffer buffer, int index) {
    commandEncoder.SetBuffer(buffer, 0, index);
  }

  void set_int(int i, int index) {
    commandEncoder.SetBytes(&i, sizeof(int), index);
  }

  void run(int w, int h = 1) {
    // run
    set_buffer();

    mtlpp::Size threadsPerGrid(w, h, 1);
    auto _w = pipeline.GetThreadExecutionWidth(),
         _h = pipeline.GetMaxTotalThreadsPerThreadgroup() / _w;
    mtlpp::Size threadsPerThreadgroup(_w, _h, 1);
    commandEncoder.DispatchThread(threadsPerGrid, threadsPerThreadgroup);

    // go
    commandEncoder.EndEncoding();
    commandBuffer.Commit();
    commandBuffer.WaitUntilCompleted();
  }

  void copy_to(void* data, mtlpp::Buffer buffer, int byte_len) {
    memcpy(data, buffer.GetContents(), byte_len);
  }

  void copy_to(void* data, int index, int size) {
    memcpy(data, vbuff[index].buff.GetContents(), size);
  }

  template <class Iterable>
  void copy_to(const Iterable& iterable, int index) {
    copy_to((void*)iterable.begin(), index, bytes(iterable));
  }

  template <class Iterable>
  void copy_to(const Iterable& iterable) {
    for (int ix = 0; ix < vbuff.size(); ix++)
      if ((void*)iterable.begin() == vbuff[ix].vb)
        copy_to((void*)iterable.begin(), ix, bytes(iterable));
  }

  template <class Iterable>
  inline int bytes(const Iterable& iterable) {
    return std::distance(iterable.begin(), iterable.end()) *
           sizeof(iterable[0]);
  }
};
