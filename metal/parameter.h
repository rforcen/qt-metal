#pragma once

// different types of metal funcs parameters

#include <VM_Vect.h>

typedef enum {
  pt_int,
  pt_float,
  pt_float2,
  pt_float4,
  pt_int4,
  pt_vbyte,
  pt_vfloat,
  pt_vcolor,
  pt_vpoints
} parameter_type;

class Parameter {
  typedef uchar byte;

 public:
  Parameter(int i) : pt(pt_int), i(i) {}
  Parameter(float f) : pt(pt_float), f(f) {}
  Parameter(simd_float4 *f4) : pt(pt_float4), f4(f4) {}
  Parameter(simd_int4 *i4) : pt(pt_int4), i4(i4) {}
  Parameter(simd_float2 *f2) : pt(pt_float2), f2(f2) {}
  Parameter(VM_Vect<color> *vm_color) : pt(pt_vcolor), vm_color(vm_color) {}
  Parameter(VM_Vect<point> *vm_point) : pt(pt_vpoints), vm_point(vm_point) {}
  Parameter(VM_Vect<byte> *vm_byte) : pt(pt_vbyte), vm_byte(vm_byte) {}
  Parameter(VM_Vect<float> *vm_float) : pt(pt_vfloat), vm_float(vm_float) {}

  parameter_type type() { return pt; }

  void set_buffer(metal_device *metal, int index) {
    switch (type()) {
      case pt_int:
        metal->set_int(i, index);
        break;
      case pt_float:
        metal->set_float(f, index);
        break;
      case pt_float2:
        metal->setBytesParam(f2, sizeof(*f2), index);
        break;
      case pt_float4:
        metal->setBytesParam(f4, sizeof(*f4), index);
        break;
      case pt_int4:
        metal->setBytesParam(i4, sizeof(*i4), index);
        break;

      case pt_vcolor:
        vm_color->create_copy_buffer(metal);
        vm_color->set_buffer(index);
        break;
      case pt_vpoints:
        vm_point->create_copy_buffer(metal);
        vm_point->set_buffer(index);
        break;
      case pt_vbyte:
        vm_byte->create_copy_buffer(metal);
        vm_byte->set_buffer(index);
        break;
      case pt_vfloat:
        vm_float->create_copy_buffer(metal);
        vm_float->set_buffer(index);
        break;
    }
  }

 private:
  parameter_type pt;
  union {
    int i;
    float f;
    simd_float2 *f2;
    simd_float4 *f4;
    simd_int4 *i4;
    VM_Vect<byte> *vm_byte;
    VM_Vect<float> *vm_float;
    VM_Vect<color> *vm_color;
    VM_Vect<point> *vm_point = nullptr;
  };
};
