// metal capable vector class
// uses virtual memory as allocator

#pragma once

#include <metal.h>
#include <vm.h>

template <class T>
class VM_Vect : public VM {
 public:
  VM_Vect() {}
  VM_Vect(int size) { resize(size); }
  VM_Vect(void* data, int size) {
    resize(size);
    memcpy(d, data, _size_bytes);
  }
  VM_Vect(VM_Vect const& cpy) { copy(cpy); }
  VM_Vect(VM_Vect&& mv) { move(mv); }
  VM_Vect& operator=(const VM_Vect& cpy) {
    copy(cpy);
    return *this;
  }
  VM_Vect& operator=(VM_Vect&& mv) {
    move(mv);
    return *this;
  }

  ~VM_Vect() { free(); }

  void clear() { free(); }
  void resize(int new_size) {
    if (new_size != _size && new_size != 0) {
      int nbs = new_size * sizeof(T);
      T* _d = (T*)VM::calloc(nbs);
      if (d) {
        memcpy(_d, d, min(_size_bytes, nbs));
        free();
      }
      d = _d;
      _size = new_size;
      _size_bytes = nbs;
    }
  }

  inline T& operator[](int index) {  // index mutator
    assert(index >= 0 && index < _size && "VM_Vect: index error");
    return d[index];
  }

  inline const T& operator[](int index) const {  // index accessor
    assert(index >= 0 && index < _size && "Vect: index error");
    return d[index];
  };

  T* begin() { return d; }
  T* end() { return d + size(); }

  bool is_empty() { return _size == 0; }

  void create_buffer(metal_device* metal_device) {
    this->metal_device = metal_device;
    metal_buffer = metal_device->createBuffer(d, _size_bytes);
  }

  void create_copy_buffer(metal_device* metal_device) {
    this->metal_device = metal_device;
    metal_buffer = metal_device->copyBuffer(d, _size_bytes);
  }

  void set_buffer(int index) {
    if (metal_device) metal_device->setBufferParam(metal_buffer, index);
  }

  inline T* data() { return d; }
  uchar* udata() { return (uchar*)d; }

  inline int length() { return _size_bytes; }
  inline int size() { return _size; }

  void copy_buffer() {
    if (metal_device)
      metal_device->copyContentsOn(d, metal_buffer, _size_bytes);
  }

 private:
  void alloc(int size) {
    free();
    d = (T*)VM::calloc(size);
  }
  void free() {
    if (d != nullptr && _size != 0) VM::free(d, _size_bytes);
    d = nullptr;
    _size = _size_bytes = 0;
  }

  void copy(const VM_Vect& cpy) {  // deep copy
    free();
    d = (T*)VM::calloc(cpy._size_bytes);
    memcpy(d, cpy.d, cpy._size_bytes);
    _size = cpy._size;
    _size_bytes = cpy._size_bytes;
    metal_buffer = cpy.metal_buffer;
    metal_device = cpy.metal_device;
  }
  void move(VM_Vect& mv) { memcpy(this, &mv, sizeof(VM_Vect)); }
  int min(int a, int b) { return a < b ? a : b; }

 private:
  T* d = nullptr;
  int _size = 0, _size_bytes = 0;
  void* metal_buffer = nullptr;
  metal_device* metal_device = nullptr;
};
