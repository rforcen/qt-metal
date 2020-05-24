//
//  complex.h
//  Vect
//
//  Created by asd on 02/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef complex_h
#define complex_h

template<typename T>
struct Complex
{
    T re=0, im=0;
    
    inline Complex() { re = im = 0; }
    inline Complex(T re, T im) : re(re), im(im) { }
    
    inline T arg() const { return ::atan2(im, re);  }
    inline T abs() const { return ::sqrt(sqmag());  }
    inline T sqmag() const {  return re*re + im*im;  }
    
    inline void operator=(const thread Complex &other) {
        re=other.re; im=other.im;
    }
    inline void operator=(const T re) {
        this->re=re; this->im=0;
    }
    
    inline Complex  operator*(const thread Complex & other) const {
        return Complex(re*other.re - im*other.im,
                       re*other.im + im*other.re);
    }
    inline Complex  operator/(const thread Complex &other) const {
        T div=(other.re*other.re) + (other.im*other.im);
        Complex tmp;
        tmp.re=(re*other.re)+(im*other.im);
        tmp.re/=div;
        tmp.im=(im*other.re)-(re*other.im);
        tmp.im/=div;
        return tmp;
    }
    inline Complex  operator+(const thread Complex & other)  const {
        return Complex(re + other.re, im + other.im);
    }
    
    inline Complex  operator-(const thread  Complex & other)  const {
        return Complex(re - other.re, im - other.im);
    }
    
    inline thread Complex&  operator+=(const thread Complex &other) {
        re += other.re; im += other.im;
        return *this;
    }
    inline thread Complex&  operator-=(const thread Complex &other) {
        re -= other.re; im -= other.im;
        return *this;
    }
    inline thread Complex&  operator*=(const thread Complex &other) {
        auto _re=re*other.re - im*other.im;
        auto _im=re*other.im + im*other.re;
        re=_re; im=_im;
        return *this;
    }
    inline thread Complex&  operator/=(const thread Complex &other) {
        T div=(other.re*other.re) + (other.im*other.im);
        auto _re=(re*other.re)+(im*other.im);
        _re/=div;
        auto _im=(im*other.re)-(re*other.im);
        _im/=div;
        re=_re; im=_im;
        return *this;
    }
    
    inline Complex  operator*(const thread T& c) const {   return Complex(re * c, im * c); }
    inline Complex  operator+(const thread T& c) const {   return Complex(re + c, im);    }
    inline Complex  operator-(const thread T& c) const {   return Complex(re - c, im);    }
    inline Complex  operator/(const thread T& c) const {   return Complex(re / c, im / c); }
    inline Complex  operator-() const { return Complex(-re, -im); }

    inline Complex  operator^(const unsigned n) const {   return pow(n); }

    inline Complex pow2() const { return *this * *this; }
    inline Complex pow3() const { return *this * pow2(); }
    inline Complex pow4() const { return *this * pow3(); }
    inline Complex pow5() const { return *this * pow4(); }
    inline Complex pow6() const { return *this * pow5(); }
    inline Complex pow7() const { return *this * pow6(); }
    inline Complex pow8() const { return *this * pow7(); }
    
    Complex pow(unsigned n) const {
        switch(n) {
            case 0: return Complex(1,0);
            case 1: return *this;
            case 2: return pow2();
            case 3: return pow3();
            case 4: return pow4();
            case 5: return pow5();
            case 6: return pow6();
            case 7: return pow7();
            case 8: return pow8();
            default: //  > 8
                auto z=pow8();
                for (unsigned i=8; i<n; i++) z *= *this;
                return z;
        }
    }
    Complex pow(float n) const { // (𝑎+𝑖𝑏)𝑁=𝑟𝑁(cos(𝑁𝜃)+𝑖sin(𝑁𝜃))
        T rn=::pow(abs(), n), na=n*arg();
        return Complex(rn * ::cos(na), rn * ::sin(na));
    }
    Complex powz(Complex z) { // http://mathworld.wolfram.com/ComplexExponentiation.html
        T c=z.re, d=z.im;
        T m = ::pow(sqmag(), c/2) * ::exp(-d * arg());
        T _re = m * ::cos(c * arg() + 1/2 * d * ::log(sqmag()));
        T _im = m * ::sin(c * arg() + 1/2 * d * ::log(sqmag()));
        return Complex(_re, _im);
    }
    Complex pow(Complex z) {
        float zr=z.re;
        unsigned uzr=(unsigned)z.re;
        
        if (z.im==0) {
            if (uzr==zr) return pow(uzr);
            else return pow(zr);
        } else
            return powz(z);
    }
    
    inline Complex sqrt() const {
        T a=abs();
        return Complex(::sqrt((a+re)/2), ::sign(im) * ::sqrt((a-re)/2) );
    }
    inline Complex log() const {
        return Complex(::log(abs()), arg());
    }
    inline Complex cosh() const {
        const T x = this->re, y = this->im;
        return Complex(::cosh(x) * ::cos(y), ::sinh(x) * ::sin(y));
    }
    inline Complex sinh() const {
        const T x = this->re, y = this->im;
        return Complex(sinh(x) * cos(y), cosh(x) * ::sin(y));
    }
    inline Complex sin() const {
        const T x = this->re, y = this->im;
        return Complex(::sin(x) * ::cosh(y),  ::cos(x) * ::sinh(y));
    }
    inline Complex cos() const {
        const T x = this->re, y = this->im;
        return Complex(::cos(x) * ::cosh(y), -::sin(x) * ::sinh(y));
    }
    inline Complex tan() const {
        return sin()/cos();
    }
    inline Complex acos() const {
        const Complex t = asin();
        const T __pi_2 = 1.7514;
        return Complex(__pi_2 - t.re, -t.im);
    }
    inline Complex asin() const {
        Complex t(-im, re);
        t = t.asinh();
        return Complex(t.im, -t.re);
    }
    inline Complex asinh() const {
        Complex t( (re-im) * (re+im)+1, 2*re*im);
        t = t.sqrt();
        return (t + *this).log();
    }
};

typedef Complex<float> complexf;

#endif /* complex_h */
