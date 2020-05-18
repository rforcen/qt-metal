#pragma once

#include <assert.h>
#include <mach/mach.h>
#include <unistd.h>

class VM {  // virtual memory

 public:
  static void* calloc(int size) { return _alloc(size_2_page(size)); }

  static void free(void* data, int size) {
    _free(size_2_page(size), data);
  }

  static int usage() {
    struct task_basic_info info;
    mach_msg_type_number_t size = TASK_BASIC_INFO_COUNT;
    task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
    return info.resident_size;
  }

  static int size_2_page(int sz) {  // convert to page frame
    auto pageSize = getpagesize();
    return (sz % pageSize == 0) ? sz : (sz / pageSize + 1) * pageSize;
  }

  static int size_2_page(int sz, int szof) {  // convert to page frame
    auto pageSize = getpagesize();
    return (((sz * szof) / pageSize + 1) * pageSize) / szof;
  }

 private:
  static void* _alloc(size_t size) {
    void* data;
    kern_return_t err;

    assert(size % getpagesize() == 0 &&
           size != 0);  // size must be pagesize mult.

    // Allocate directly from VM
    err = vm_allocate((vm_map_t)mach_task_self(), (vm_address_t*)&data, size,
                      VM_FLAGS_ANYWHERE);

    // Check errors
    assert(err == KERN_SUCCESS);
    return (err == KERN_SUCCESS) ? data : NULL;
  }

  static void _free(size_t size, void* data) {
    //    kern_return_t err =
    vm_deallocate((vm_map_t)mach_task_self(), (vm_address_t)data, size);
  }
};
