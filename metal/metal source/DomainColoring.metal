//
//  DomainColoring.metal
//  MetalKernelSum
//
//  Created by asd on 01/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

#include "zSymbols.h"
#include "complex.h"
#include "zVM.h"

typedef Complex<float> ComplexFloat;

color HSV2int(float h, const float s, const float v);
color pixelColor(uint x, uint y, uint w, uint h);
inline color argbf2uint(uint8_t alpha, float r, float g, float b);
color pixelColorzFunc(Geo geo, const device byte*code, const device float*consts) ;
inline float pow2(float x) { return x*x; }
inline float pow3(float x) { return x*x*x; }
inline uint pos2index(uint2 position, uint width) { return position.x + width * position.y; }
inline uint geo2index(Geo geo) { return geo.x + geo.y * geo.width; }

// zVM section
kernel void domainColorzFunc( // pixel wise generation with z compiled func
                             device color*colors[[buffer(0)]], // buffer per device
                             
                             device const Geo&_geo[[buffer(1)]], // geo (0,0,w,h)
                             
                             const device byte* code[[buffer(2)]],
                             const device float* consts[[buffer(3)]],
                             
                             uint2 position [[thread_position_in_grid]] ) // x,y
{
    Geo geo={position.x, position.y, _geo.width, _geo.height};
    
    colors[ geo2index(geo) ] = pixelColorzFunc(geo, code, consts);
}

color pixelColorzFunc(Geo geo, const device byte*code, const device float*consts) {
    
    const float E = 2.7182818284590452353602874713527;
    const float M_PI = 3.141592653589793238462643383;
    const float PI = M_PI, PI2 = PI * 2.;
    const float limit=PI,  rmi = -limit, rma = limit, imi = -limit, ima = limit;
    
    auto z=ComplexFloat( ima - (ima - imi) * geo.y / (geo.height - 1),  rma - (rma - rmi) * geo.x / (geo.width - 1) );
    auto v = zVM(code, consts).execute(z); // evaluate zCode func
    
    auto ang = v.arg();
    while (ang < 0) ang += PI2;
    ang /= PI2;
    
    float m = v.abs(), ranges = 0., rangee = 1.; //  prop. e^n < m < e^(n-1)
    while (m > rangee) {
        ranges = rangee;
        rangee *= E;
    }
    float k = (m - ranges) / (rangee - ranges);
    float kk = (k < 0.5 ? k * 2. : 1. - (k - 0.5) * 2);
    
    float sat = 0.4 + (1. - pow3(1. - (kk)))     * 0.6;
    float val = 0.6 + (1. - pow3(1. - (1 - kk))) * 0.4;
    
    return HSV2int(ang, sat, val);
}


// local func based services

inline ComplexFloat func(const ComplexFloat z)  { // main function evaluator
    auto z1 = (z.pow4()+1.)/(z.pow3()-1.);
    z1=z.pow4()*z.cos() + z.pow2() + z/2 * z.sin();
    return z1;
}


kernel void domainColor( // pixel wise generation
     device color*colors[[buffer(0)]], // buffer per device
                        
     const device int &dev[[buffer(1)]], // current device
     const device int &nDevices[[buffer(2)]],
     const device int &width[[buffer(3)]],
     const device int &height[[buffer(4)]],
                     
     uint2 position [[thread_position_in_grid]] )
{
    uint szdev=height/nDevices, fromDevice=dev*szdev; // n. device calcs
    uint index = position.x + width * position.y; // x,y -> index
    
    colors[index] = pixelColor(position.x, position.y + fromDevice, width, height);
}



color pixelColor(uint x, uint y, uint w, uint h) {

    const float E = 2.7182818284590452353602874713527;
    const float M_PI = 3.141592653589793238462643383;
    const float PI = M_PI, PI2 = PI * 2.;
    const float limit=PI,  rmi = -limit, rma = limit, imi = -limit, ima = limit;
    
    auto z=Complex<float>( ima - (ima - imi) * y / (h - 1),  rma - (rma - rmi) * x / (w - 1) );
    auto v = func(z); // evaluate
    
    float ang=fmod(fabs(v.arg()),PI2) / PI2; // -> hsv
    
    float m = v.abs(), ranges = 0., rangee = 1.; //  prop. e^n < m < e^(n-1)
    while (m > rangee) {
        ranges = rangee;
        rangee *= E;
    }
    float k = (m - ranges) / (rangee - ranges);
    float kk = (k < 0.5 ? k * 2. : 1. - (k - 0.5) * 2);
    
    float sat = 0.4 + (1. - pow3(1. - (kk)))     * 0.6;
    float val = 0.6 + (1. - pow3(1. - (1 - kk))) * 0.4;
    
    return HSV2int(ang, sat, val);
}

color HSV2int(float h, const float s, const float v) { // convert hsv to int with alpha 0xff00000
    float r = 0, g = 0, b = 0;
    
    if (s == 0) r = g = b = v;
    else {
        if (h == 1)  h = 0;
        
        float z = floor(h * 6.);
        int i = z;
        
        float   f = h * 6 - z,
        p = v * (1 - s), q = v * (1 - s * f),
        t = v * (1 - s * (1 - f));
        
        switch (i) {
            case 0:          r = v;   g = t;   b = p;             break;
            case 1:          r = q;   g = v;   b = p;             break;
            case 2:          r = p;   g = v;   b = t;             break;
            case 3:          r = p;   g = q;   b = v;             break;
            case 4:          r = t;   g = p;   b = v;             break;
            case 5:          r = v;   g = p;   b = q;             break;
        }
    }
    return argbf2uint(0xff, r,g,b);
}

inline color argbf2uint(uint8_t alpha, float r, float g, float b) { // alpha 0xff
    return (((color)alpha) << 24) |
    ( ( (color)(255.*r) & 0xff ) |  ( ((color)(255.*g)&0xff)<<8 ) | ( ((color)(255.*b)&0xff)<<16 ) );
}
