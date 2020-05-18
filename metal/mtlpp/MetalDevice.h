//
//  MBEContext.h
//  MetalKernel
//
//  Created by asd on 28/03/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef MBEContext_h
#define MBEContext_h

#import <Metal/Metal.h>

@interface Encoder : NSObject {
 @public
  id<MTLComputePipelineState> pipeline;
  id<MTLCommandQueue> commandQueue;
}
+ (Encoder *)init:(id<MTLCommandQueue>)commandQueue pipeline:(id<MTLComputePipelineState>)pipeline;
@end

@interface MetalDevice : NSObject {  // single metal device object
 @public
  id<MTLDevice> device;
  id<MTLLibrary> library;
  id<MTLCommandQueue> commandQueue;
  id<MTLFunction> kernelFunction;
  id<MTLCommandBuffer> commandBuffer;
  id<MTLComputeCommandEncoder> commandEncoder;
  id<MTLComputePipelineState> pipeline;
  NSMutableDictionary<NSString *, Encoder *> *funcDict;
  NSUInteger maxMem;
}

+ (instancetype)init;
- (void)compileFunc:(NSString *)func;
- (void)prepFunc:(NSString *)func;
- (void)genEncoder;
- (void)run;
- (id<MTLBuffer>)createBuffer:(void *)data length:(NSInteger)len;
- (NSUInteger)sz2Page:(NSUInteger)sz szof:(NSUInteger)szof;
- (NSInteger)sz2Page:(NSInteger)sz;
- (void *)newData:(NSUInteger)size;
- (void)deleteData:(NSUInteger)size data:(void *)data;
+ (NSUInteger)memoryUsage;
- (id<MTLBuffer>)createGPUPrivateBuffer:(void *)data length:(NSInteger)len;
- (id<MTLBuffer>)copyBuffer:(void *)data length:(NSInteger)len;
- (void)runThreadsWidth:(NSUInteger)width height:(NSUInteger)height;
- (void)runThreads3d:(NSUInteger)width height:(NSUInteger)height depth:(NSUInteger)depth;
//-(void)copyContentsOn:(void*)data buffer:(id<MTLBuffer>)buffer;
- (void)copyContentsOn:(void *)data buffer:(id<MTLBuffer>)buffer length:(NSUInteger)length;
- (void)setBufferParam:(id<MTLBuffer>)buffer index:(uint)index;
- (void)setBytesParam:(void *)data length:(uint)length index:(int)index;
- (void)setIntParam:(void *)data index:(int)index;
- (void)setFloatParam:(void *)data index:(int)index;
+ (NSTimeInterval)timeIt:(void (^)(void))block;

@end


#endif /* MBEContext_h */
