// metal capable vector class
// uses virtual memory as allocator

#pragma once

#include <metal.h>
#include <vm.h>

template <class T>
class VM_Vect : public VM {
 public:
  VM_Vect() {}
  VM_Vect(int size) { alloc(size); }
  VM_Vect(VM_Vect const& cpy) { copy(cpy); }
  VM_Vect(VM_Vect&& mv) { move(mv); }
  VM_Vect& operator=(const VM_Vect& cpy) { copy(cpy); }
  VM_Vect& operator=(VM_Vect&& mv) { move(mv); }

  ~VM_Vect() { free(); }

  void resize(int new_size) {
    if (new_size != size && new_size != 0) {
      int nbs = new_size * sizeof(T);
      T* _d = (T*)VM::calloc(nbs);
      if (d) {
        memcpy(_d, d, min(size_bytes, nbs));
        free();
      }
      d = _d;
      size = new_size;
      size_bytes = nbs;
    }
  }

  inline T& operator[](int index) {  // index mutator
    assert(index >= 0 && index < size && "VM_Vect: index error");
    return d[index];
  }

  inline const T& operator[](int index) const {  // index accessor
    assert(index >= 0 && index < size && "Vect: index error");
    return d[index];
  };

  void create_buffer(Metal* metal) {
    this->metal = metal;
    buffer = metal->create_buffer(d, size_bytes);
  }
  void set_buffer(int index) { metal->set_buffer(buffer, index); }

  T* data() { return d; }

 private:
  void alloc(int size) {
    free();
    alloc(size);
  }
  void free() {
    if (d != nullptr && size != 0) VM::free(d, size_bytes);
    d = nullptr;
    size = size_bytes = 0;
  }
  void copy(VM_Vect& cpy) {  // deep copy
    free();
    d = VM::calloc(cpy.size_bytes);
    memcpy(d, cpy.d, cpy.size_bytes);
    size = cpy.size;
    size_bytes = cpy.size_bytes;
    buffer = cpy.buffer;
    metal = cpy.metal;
  }
  void move(VM_Vect& mv) { memcpy(this, &mv, sizeof(VM_Vect)); }
  int min(int a, int b) { return a < b ? a : b; }

  T* d = nullptr;
  int size = 0, size_bytes = 0;

  mtlpp::Buffer buffer;
  Metal* metal = nullptr;
};
