#ifndef ColorIndexHmetal
#define ColorIndexHmetal

#include "ColorScale_metal.h"

constant const int clRed=0xff, clBlue=0xff0000, maxColors=4096;

class ColorIndex {
public:
    int colorIndex[maxColors];
    int nColors, colorFrom, colorTo;
    float color4f[3]; // rgb in float 0..1
    
    
    ColorIndex() {
        createColorIndex(maxColors, clRed, clBlue);
    }
    ColorIndex(int maxColors) {
        createColorIndex(maxColors, clRed, clBlue);
    }
    
    ColorIndex(int nColors, int colorFrom, int colorTo) {
        createColorIndex(nColors, colorFrom, colorTo);
    }
    
    void createColorIndex(int nColors, int colorFrom, int colorTo) {
        this->nColors = nColors;
        this->colorFrom = colorFrom;
        this->colorTo = colorTo;
        for (int i = 0; i < nColors; i++)
            colorIndex[i] = ColorScaleHSL(colorFrom, colorTo, (float)i / nColors);
    }
    
    int getColor(float r) {
        return colorIndex[(int)(nColors * r) % nColors];
    }
    int getColorA(float r) {
        return 0xff000000 | colorIndex[(int)(nColors * r) % nColors];
    }
    
    int getRed(int color) {
        color &= 0x00ffffff;
        return (color >> 16) & 0xff;
    }
    
    int getGreen(int color) {
        color &= 0x00ffffff;
        return (color >> 8) & 0xff;
    }
    
    int getBlue(int color) {
        color &= 0x00ffffff;
        return (color) & 0xff;
    }
    
    float getRedf(int color) {
        return getRed(color) / 255.f;
    }
    
    float getGreenf(int color) {
        return getGreen(color) / 255.f;
    }
    
    float getBluef(int color) {
        return getBlue(color) / 255.f;
    }
};
#endif
