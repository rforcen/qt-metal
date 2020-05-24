//
//  Mandelbrot.metal
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

#include "zSymbols.h"
#include "complex.h"
#include "zVM.h"
#include "Mandelbrot_metal.hpp"

kernel void Mandelbrot( device color*pixels[[buffer(0)]], //  fixed coded func
                       const device range &rng[[buffer(1)]],    // range
                       const device uint &width[[buffer(2)]],   // width
                       const device uint &height[[buffer(3)]],  // height
                       const device uint &iters[[buffer(4)]],  // iters
                       
                       uint2 position [[thread_position_in_grid]] )
{
    class Mandelbrot mb(rng, width, height, iters);
    
    pixels[position.x + position.y * width] = mb.generateZ(position.x, position.y, 4);
}

kernel void MandelbrotzFunc( device color*pixels[[buffer(0)]], // compiled z func
                            const device range &rng[[buffer(1)]],    // range
                            const device uint &width[[buffer(2)]],   // width
                            const device uint &height[[buffer(3)]],  // height
                            const device uint &iters[[buffer(4)]],  // iters
                            
                            const device byte*code[[buffer(5)]],    // z code
                            const device float* consts[[buffer(6)]], // expression consts
                            
                            uint2 position [[thread_position_in_grid]] )
{
    class Mandelbrot mb(rng, width, height, iters);
    
    pixels[position.x + position.y * width] = mb.generateZfunc(position.x, position.y, 4, code, consts);
}


// test backprop metal performance

float sigmoid(float z) { return 1. / (1. + exp(-z)); }
float sigmoid_prime(float z) {  // Derivative of the sigmoid function
    auto sg = sigmoid(z);
    return sg * (1. - sg);
}

kernel void testFlops(
                      device float*res[[buffer(0)]],
                      const device uint&w[[buffer(1)]],
                      uint2 pos[[thread_position_in_grid]]
                      ) {
    uint sz=w*w;
    float a=.0123,b=.0345,r=0, z;
    for (uint i=0; i<sz; i++) r+=a*b+b;
    for (uint i=0; i<sz; i++) z=sigmoid(res[i]);
    for (uint i=0; i<sz; i++) z=sigmoid_prime(r);
    for (uint i=0; i<sz; i++) z+=res[i];
    z=0;
    res[pos.x + pos.y*w]=r;
}


kernel void matrix_mult(
                        device float* C [[ buffer(0) ]],
                        device float* A [[ buffer(1) ]],
                        device float* B [[ buffer(2) ]],
                        constant uint& n [[ buffer(3) ]],
                        
                        uint blockIdx [[threadgroup_position_in_grid]],
                        uint threadIdx [[thread_position_in_threadgroup]]
                        ) {
    if ((blockIdx < (((n - 32) / 32) + 1)) && (blockIdx < (((n + 31) / 32) - 1))) {
        C[(blockIdx * 32) + threadIdx] = A[(blockIdx * 32) + threadIdx] * B[(blockIdx * 32) + threadIdx];
    } else {
        if ( (blockIdx * 32) + threadIdx < n ) {
            if ((blockIdx * 32) + threadIdx < n) {
                C[(blockIdx * 32) + threadIdx] = A[(blockIdx * 32) + threadIdx] * B[(blockIdx * 32) + threadIdx];
            }
        }
    }
}
