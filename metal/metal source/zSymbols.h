//
//  zSymbols.h
//  Vect
//
//  Created by asd on 11/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef zSymbols_h
#define zSymbols_h

typedef struct { uint x, y, width, height; } Geo;
typedef uint32_t color; // aa bb gg rr  32 bit color
typedef uint8_t byte;


enum {
    SNULL = 0, NUMBER = 1, IDENT_i = 2, IDENT_z = 3, PLUS = 5, MINUS = 6,
    MULT = 7, DIV = 8, OPAREN = 9, CPAREN = 10, POWER = 12, PERIOD = 13,
    
    // function names
    FSIN = 90, FCOS = 91, FTAN = 92, FEXP = 93, FLOG = 94, FLOG10 = 95,
    FINT = 96, FSQRT = 97, FASIN = 98, FACOS = 99, FATAN = 100, FABS = 101,
    FC = 102, SPI = 103, SPHI = 104,
    PUSHC = 112, PUSHZ = 113, PUSHI = 114, PUSHCC = 115,
    NEG = 115,
    
    END = 200
};

#endif /* zSymbols_h */
