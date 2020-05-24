#ifndef _COLOR_SCALEmetal
#define _COLOR_SCALEmetal

typedef unsigned int COLORREF;	// 0 to 0xFFFFFF and 0xFFFFFFFF for transparencies

// Create a 0xRRGGBB from separate 8 bits colors - no error checking
#define MakeRGB(r,g,b) ( ((r)<<16) | ((g)<<8) | (b))
#define _getRED(c)   ((((c)&0xFF0000)>>16))
#define _getGREEN(c) ((((c)&0x00FF00)>>8))
#define _getBLUE(c)  ((((c)&0x0000FF)))

#define _getREDf(c)   (_getRED(c)/256.)
#define _getGREENf(c) (_getGREEN(c)/256.)
#define _getBLUEf(c)  (_getBLUE(c)/256.)
#define Max(x,y) (((x)>(y))?(x):(y))
#define Min(x,y) (((x)<(y))?(x):(y))

extern COLORREF ColorScaleRGB( COLORREF Col1,  COLORREF Col2,  float Ratio);
extern COLORREF ColorScaleHSL( COLORREF Col1,  COLORREF Col2,  float Ratio);
extern COLORREF ColorScaleRGB3( COLORREF Col1,  COLORREF Col2,  COLORREF Col3,
                                float Ratio1, float Ratio2, float Ratio3);
extern COLORREF ColorScaleHSL3( COLORREF Col1,  COLORREF Col2,  COLORREF Col3,
                                float Ratio1, float Ratio2, float Ratio3);

extern void     RGBtoHLS( COLORREF rgb, float thread &H, float  thread  &L, float thread  &S );
extern COLORREF	HLStoRGB( float H,  float L,  float S );

#endif
