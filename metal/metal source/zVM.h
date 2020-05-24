//
//  zVM.h
//  Vect
//
//  Created by asd on 11/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef zVM_h
#define zVM_h

#include "zSymbols.h"
#include "complex.h"

class zVM {
    
    static constant int maxStack=16;
    
    typedef uint8_t byte;
    const device byte*code;
    const device float*consts;
    
public:
    
    zVM(const device byte*code,  const device float *consts) : code(code), consts(consts) {}
    
    complexf execute(complexf z) {
        bool err=false, end=false;
        complexf stack[maxStack];
        complexf CI = complexf(0., 1.);
        int pc=0, sp=0;
        
        while (!end) {
            switch (code[pc]) {
                case END: end=true; break;
                    
                case PUSHC:   pc++;    stack[sp] = complexf(consts[code[pc]], 0.);     sp++;     pc++;       break;
                case PUSHZ:   pc++;    stack[sp] = z;                                      sp++;                 break;
                case PUSHI:   pc++;    stack[sp] = CI;                                     sp++;                 break;
                case PLUS:    sp--;    stack[sp - 1] += stack[sp];                         pc++;                 break;
                case MINUS:   sp--;    stack[sp - 1] -= stack[sp];                         pc++;                 break;
                case MULT:    sp--;    stack[sp - 1] *= stack[sp];                         pc++;                 break;
                case DIV:     sp--;    stack[sp - 1] /= stack[sp];                         pc++;                 break;
                case POWER:   sp--;    stack[sp - 1] = stack[sp - 1].pow(stack[sp]);       pc++;                 break;
                case NEG:     stack[sp - 1] = -stack[sp - 1];                              pc++;                 break;
                case FSIN:    stack[sp - 1] = stack[sp - 1].sin();                pc++;                break;
                case FCOS:    stack[sp - 1] = stack[sp - 1].cos();                pc++;                break;
                case FTAN:    stack[sp - 1] = stack[sp - 1].tan();                pc++;                break;
                case FASIN:   stack[sp - 1] = stack[sp - 1].asin();               pc++;                break;
                case FACOS:   stack[sp - 1] = stack[sp - 1].acos();               pc++;                break;
                case FATAN:
                //                stack[sp - 1] = atan(stack[sp - 1]);
                pc++;
                break;
                case FEXP:
                //                stack[sp - 1] = exp(stack[sp - 1]);
                pc++;
                break;
                case FLOG:    stack[sp - 1] = stack[sp - 1].log();                pc++;                break;
                case FLOG10:
                //                stack[sp - 1] = log10(stack[sp - 1]);
                pc++;
                break;
                case FSQRT:   stack[sp - 1] = stack[sp - 1].sqrt();                    pc++;                break;
                case FABS:    stack[sp - 1] = complexf(stack[sp - 1].abs(), 0);    pc++;                break;
                case FINT:    stack[sp - 1] = complexf((int)stack[sp - 1].re, (int)stack[sp - 1].im);   pc++;   break;
                case FC:      sp--;       pc++;    stack[sp - 1] = complexf(stack[sp - 1].re, stack[sp].re);    break;
                    
                default:      end=true;    err = true;           break;
            }
        }
        return (sp != 0) ? stack[sp - 1] : complexf(0, 0);
    }
};

#endif /* zVM_h */
