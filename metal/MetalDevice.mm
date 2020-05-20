//
//  MBEContext.m
//  MetalKernel
//
//  Created by asd on 28/03/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#import "MetalDevice.h"
#import <math.h>

@implementation Encoder
+ (Encoder*)init:(id<MTLCommandQueue>)commandQueue pipeline:(id<MTLComputePipelineState>)pipeline {
  Encoder* e = [[super alloc] init];
  e->commandQueue = commandQueue;
  e->pipeline = pipeline;
  return e;
}
@end

@implementation MetalDevice

+ (instancetype)init {
  MetalDevice* md = [[super alloc] init];
  md->device = MTLCreateSystemDefaultDevice();
//  md->library = [md->device newDefaultLibrary];
  md->library = [md->device newLibraryWithFile:@"default.metallib" error:nil];
  md->funcDict = [[NSMutableDictionary alloc] init];
  return md;
}

- (NSUInteger)sz2Page:(NSUInteger)sz szof:(NSUInteger)szof {  // convert to page frame
  auto pageSize = getpagesize();
  return ((sz * szof / pageSize + 1) * pageSize) / szof;
}

- (NSInteger)sz2Page:(NSInteger)sz {  // convert to page frame
  auto pageSize = getpagesize();
  return (sz % pageSize == 0) ? sz : (sz / pageSize + 1) * pageSize;
}

- (void*)AllocateVirtualMemory:(size_t)size {
  void* data;
  kern_return_t err;

  assert(size % getpagesize() == 0 && size != 0);  // size must be pagesize mult.

  // Allocate directly from VM
  err = vm_allocate((vm_map_t)mach_task_self(), (vm_address_t*)&data, size, VM_FLAGS_ANYWHERE);

  // Check errors
  assert(err == KERN_SUCCESS);
  return (err == KERN_SUCCESS) ? data : NULL;
}
- (void)DeallocateVirtualMemory:(size_t)size data:(void*)data {
  //    kern_return_t err =
  vm_deallocate((vm_map_t)mach_task_self(), (vm_address_t)data, size);
}

+ (NSUInteger)memoryUsage {
  struct task_basic_info info;
  mach_msg_type_number_t size = TASK_BASIC_INFO_COUNT;
  task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
  return info.resident_size;
}

- (void*)newData:(NSUInteger)size {
  return [self AllocateVirtualMemory:[self sz2Page:size]];
}
- (void)deleteData:(NSUInteger)size data:(void*)data {
  [self DeallocateVirtualMemory:[self sz2Page:size] data:data];
}

- (id<MTLBuffer>)createBuffer:(void*)data length:(NSInteger)len {
  return [device newBufferWithBytes:data length:len options:MTLResourceStorageModeShared];
}
- (id<MTLBuffer>)copyBuffer:(void*)data length:(NSInteger)len {
  return [device newBufferWithBytesNoCopy:data
                                   length:[self sz2Page:len]
                                  options:MTLResourceStorageModeShared
                              deallocator:nil];
}

- (id<MTLBuffer>)createGPUPrivateBuffer:(void*)data length:(NSInteger)len {
  return [device newBufferWithBytes:data length:len options:MTLResourceStorageModePrivate];
}

- (void)setBufferParam:(id<MTLBuffer>)buffer index:(uint)index {
  [commandEncoder setBuffer:buffer offset:0 atIndex:index];
}
- (void)setBytesParam:(void*)data length:(uint)length index:(int)index {
  [commandEncoder setBytes:data length:length atIndex:index];
}
- (void)setIntParam:(void*)data index:(int)index {
  [commandEncoder setBytes:data length:sizeof(int) atIndex:index];
}
- (void)setFloatParam:(void*)data index:(int)index {
  [commandEncoder setBytes:data length:sizeof(float) atIndex:index];
}

- (void)runThreadsWidth:(NSUInteger)width height:(NSUInteger)height {  // for a w x h grid
  MTLSize threadsPerGrid = MTLSizeMake(width, height, 1);
  NSUInteger _w = pipeline.threadExecutionWidth;
  NSUInteger _h = pipeline.maxTotalThreadsPerThreadgroup / _w;
  MTLSize threadsPerThreadgroup = MTLSizeMake(_w, _h, 1);
  [commandEncoder dispatchThreads:threadsPerGrid threadsPerThreadgroup:threadsPerThreadgroup];

  [self run];
}

- (void)runThreads3d:(NSUInteger)width
              height:(NSUInteger)height
               depth:(NSUInteger)depth {  // for a w x h x d grid
  MTLSize threadsPerGrid = MTLSizeMake(width, height, depth);
  NSUInteger _w = pipeline.threadExecutionWidth;
  NSUInteger _h = pipeline.maxTotalThreadsPerThreadgroup / _w;
  MTLSize threadsPerThreadgroup = MTLSizeMake(_w, _h, 1);
  [commandEncoder dispatchThreads:threadsPerGrid threadsPerThreadgroup:threadsPerThreadgroup];

  [self run];
}

- (void)copyContentsOn:(void*)data buffer:(id<MTLBuffer>)buffer {
  memcpy(data, [buffer contents], buffer.length);
}
- (void)copyContentsOn:(void*)data buffer:(id<MTLBuffer>)buffer length:(NSUInteger)length {
  memcpy(data, [buffer contents], length);
}

- (void)prepFunc:(NSString*)func {  // generate a pipeline & commandQueue
  pipeline = [device newComputePipelineStateWithFunction:[library newFunctionWithName:func]
                                                   error:nil];
  commandQueue = [device newCommandQueue];
}

- (void)genEncoder {
  commandBuffer = [commandQueue commandBuffer];
  commandEncoder = [commandBuffer computeCommandEncoder];
  [commandEncoder setComputePipelineState:pipeline];
}

- (void)compileFunc:(NSString*)func {  // prepare shader
  if (!funcDict[func])  // not used, create dict entry for this func (commandQueue, pipeline)
    funcDict[func] =
        [Encoder init:[device newCommandQueue]
             pipeline:[device newComputePipelineStateWithFunction:[library newFunctionWithName:func]
                                                            error:nil]];

  pipeline = funcDict[func]->pipeline;
  commandBuffer = [funcDict[func]->commandQueue commandBuffer];
  commandEncoder = [commandBuffer computeCommandEncoder];
  [commandEncoder setComputePipelineState:pipeline];
}

- (void)compileFunc__noDIct:(NSString*)func {  // prepare shader
  pipeline = [device newComputePipelineStateWithFunction:[library newFunctionWithName:func]
                                                   error:nil];
  commandBuffer = [[device newCommandQueue] commandBuffer];
  commandEncoder = [commandBuffer computeCommandEncoder];
  [commandEncoder setComputePipelineState:pipeline];
}

- (void)run {
  [commandEncoder endEncoding];
  [commandBuffer commit];
  [commandBuffer waitUntilCompleted];

  commandEncoder = nil;  // no longer valid
  commandBuffer = nil;
}

+ (NSTimeInterval)timeIt:(void (^)(void))block {
  NSDate* start = [NSDate date];
  block();
  return -[start timeIntervalSinceNow] * 1000.;
}

@end
