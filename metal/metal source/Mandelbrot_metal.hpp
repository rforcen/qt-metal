//
//  Mandelbrot.hpp
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Mandelbrotmetal_hpp
#define Mandelbrotmetal_hpp

#include "ColorScale_metal.h"
#include "complex.h"
#include "zVM.h"

typedef uint32_t color; // aa bb gg rr  32 bit color
typedef uint8_t byte;
typedef float4 range;

constant const int clRed=0xff, clBlue=0xff0000, maxColors=4096;

class Mandelbrot {
    int width=0, height=0;
    int iter=150;
    
    
    float xstart = -1, ystart = -1, xend = 1, yend = 1;
    
    // generate in 'poly'a rad n sized polygon
    void polygon(int n, float rad, float thread *poly) {
        float pi2n = 6.28318530718f / n;
        for (int i = 0; i < n; i++) {
            poly[i * 2 + 0] = rad * sin(i * pi2n);
            poly[i * 2 + 1] = rad * cos(i * pi2n);
        }
    }
    color interpolateColor(color from, color to, float ratio) {
        return 0xff000000 | ColorScaleHSL(from, to, ratio) ;
    }
    
    public:
    
    Mandelbrot() {}
    Mandelbrot(range range, int w, int h, int iter) {
        setRange(range);
        setSize(w,h);
        setIter(iter);
    }
    
    void setSize(int w, int h) { this->width=w; this->height=h; }
    void setIter(int iter) { this->iter=iter; }
    void setRange(float x0, float y0, float x1, float y1) {
        xstart=x0; ystart=y0; xend=x1; yend=y1;
    }
    void setRangeSize(range range, int w, int h) {
        xstart=range.x; ystart=range.y; xend=range.z; yend=range.w;
        width=w; height=h;
    }
    void setRange(range range) {
        xstart=range.x; ystart=range.y; xend=range.z; yend=range.w;
    }
    
    float iterate(float x, float y) { // return iteration for x,y
        float niter=0;
        float z = 0, zi = 0;
        
        bool inset = true;
        for (int k = 0; k < iter; k++) {
            // z^2 = (a+bi)(a+bi) = a^2 + 2abi - b^2
            float newz  = (z * z) - (zi * zi) + x,
            newzi = 2 * z * zi + y;
            
            z = newz;
            zi = newzi;
            
            if (((z * z) + (zi * zi)) > 4) { // sqrt(x*x + y*y)>2
                inset = false;
                niter = k;
                break;
            }
        }
        return (inset) ? 0 : niter/iter;
    }
    
    color genInterpolate(int i, int j) {
        float xstep = (xend - xstart) / width,
        ystep = (yend - ystart) / height;
        
        const int n=9;
        float poly[n*2];
        polygon(n, (xstep+ystep)/2, poly);
        
        float x = xstart + xstep*i,
        y = ystart + ystep*j;
        
        color intColors[n], iColor=0;
        for (int i=0; i<n; i++) {
            float ratio = iterate(x+poly[i*2+0], y+poly[i*2+1]);
            intColors[i] = ratio ? interpolateColor(clRed, clBlue, ratio) : 0;
        }
        for (int i=0; i<n-1; i++)
        if (intColors[i]!=0 && intColors[i+1]!=0)
        iColor = interpolateColor(intColors[i], intColors[i+1], 0.5);
        
        return iColor | 0xff000000;
    }
    
    color generate(int i, int j) { //  using floats
        float col=0;
        
        // these are used for calculating the points corresponding to the pixels
        float xstep = (xend - xstart) / width,
        ystep = (yend - ystart) / height;
        
        float x = xstart + xstep*i,
        y = ystart + ystep*j;
        
        float z = 0, zi = 0;
        
        bool inset = true;
        for (int k = 0; k < iter; k++) {
            // z^2 = (a+bi)(a+bi) = a^2 + 2abi - b^2
            float newz  = (z * z) - (zi * zi) + x,
            newzi = 2 * z * zi + y;
            
            z = newz;
            zi = newzi;
            
            if (((z * z) + (zi * zi)) > 4) { // sqrt(x*x + y*y)>2
                inset = false;
                col = k;
                break;
            }
        }
        
        return 0xff000000 | ((inset) ? 0 : ColorScaleHSL(clBlue, clRed, 20. *col/iter) );
    }
    
    inline complexf zformula(complexf Z) {
        return Z*Z*Z*Z;
    }
    
    color generateZ(int x, int y, int C) { //  using complex
        float col=0;
        complexf Z, Zinc=complexf(xstart + ((xend - xstart) / width) * x,
                                  ystart + ((yend - ystart) / height) * y);
        
        bool inset = true;
        for (int k = 0; k < iter; k++) {
            Z = zformula(Z) + Zinc;
            
            if (Z.sqmag() > C) {
                inset = false;
                col = k;
                break;
            }
        }
        
        return 0xff000000 | ((inset) ? 0 : ColorScaleHSL(clBlue, clRed, 20. *col/iter) );
    }
    
    color generateZfunc(int x, int y, int C, const device byte*code, const device float*consts) { //  using complex
        zVM zvm(code, consts); // init VM w/code & consts values
        
        float col=0;
        complexf Zinc=complexf(xstart + ((xend - xstart) / width)  * x,
                               ystart + ((yend - ystart) / height) * y),
        Z=Zinc;
        
        bool inset = true;

        for (int k = 1; k < iter; k++) {
            Z = zvm.execute(Z) + Zinc;
            
            if (Z.sqmag() > C) {
                inset = false;
                col = k;
                break;
            }
        }
        
        return 0xff000000 | ((inset) ? 0 : ColorScaleHSL(clBlue, clRed, 20. *col/iter) );
    }
    
};



#endif /* Mandelbrot_hpp */
