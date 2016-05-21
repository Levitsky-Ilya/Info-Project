//   complex.h - declaration of class
//   of complex number
//
//   The code is property of LIBROW with Ira Nikulina's additions
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <iostream>
#include <math.h>
class complex
{
protected:
	//   Internal presentation - real and imaginary parts
    float m_re;
    float m_im;

public:
	//   Imaginary unity
	static const complex i;
	static const complex j;

	//   Constructors
	complex(): m_re(0.), m_im(0.) {}
    complex(float re, float im): m_re(re), m_im(im) {}
    complex(float val): m_re(val), m_im(0.) {}

	//   Assignment
    complex& operator= (const float val) {
		m_re = val;
		m_im = 0.;
		return *this;
	}

	//   Basic operations - taking parts
    float re() const { return m_re; }
    float im() const { return m_im; }

	//   Conjugate number
	complex conjugate() const{
		return complex(m_re, -m_im);
	}

	//   Norm   
    float norm() const{
		return m_re * m_re + m_im * m_im;
	}

    //   findMagnitude
    float countMagnitude() const{
        return sqrt(m_re * m_re + m_im * m_im);
    }

    //   findAmplitude
    float countAmplitude() const{
        return 20*log10(sqrt(m_re * m_re + m_im * m_im));
    }


	//   Arithmetic operations
    complex operator+ (const complex& number) const{
        return complex(m_re + number.m_re, m_im + number.m_im);
	}

    complex operator- (const complex& number) const{
        return complex(m_re - number.m_re, m_im - number.m_im);
	}

    complex operator* (const complex& number) const{
        return complex(m_re * number.m_re - m_im * number.m_im,
            m_re * number.m_im + m_im * number.m_re);
	}

    complex operator/ (const complex& number) const{
        const float denominator = number.m_re * number.m_re + number.m_im * number.m_im;
        return complex((m_re * number.m_re + m_im * number.m_im) / denominator,
            (m_im * number.m_re - m_re * number.m_im) / denominator);
	}

    complex& operator+= (const complex& number){
        m_re += number.m_re;
        m_im += number.m_im;
		return *this;
	}

    complex& operator-= (const complex& number){
        m_re -= number.m_re;
        m_im -= number.m_im;
		return *this;
	}

    void  print();

    complex& operator*= (const complex& number){
        const float temp = m_re;
        m_re = m_re * number.m_re - m_im * number.m_im;
        m_im = m_im * number.m_re + temp * number.m_im;
		return *this;
	}

    complex& operator/= (const complex& number){
        const float denominator = number.m_re * number.m_re + number.m_im * number.m_im;
        const float temp = m_re;
        m_re = (m_re * number.m_re + m_im * number.m_im) / denominator;
        m_im = (m_im * number.m_re - temp * number.m_im) / denominator;
		return *this;
	}

	complex& operator++ ()
	{
		++m_re;
		return *this;
	}

	complex operator++ (int)
	{
		complex temp(*this);
		++m_re;
		return temp;
	}

	complex& operator-- ()
	{
		--m_re;
		return *this;
	}

	complex operator-- (int)
	{
		complex temp(*this);
		--m_re;
		return temp;
	}

    complex operator+ (const float val) const
	{
		return complex(m_re + val, m_im);
	}

    complex operator- (const float val) const
	{
		return complex(m_re - val, m_im);
	}

    complex operator* (const float val) const
	{
		return complex(m_re * val, m_im * val);
	}

    complex operator/ (const float val) const
	{
		return complex(m_re / val, m_im / val);
	}

    complex& operator+= (const float val)
	{
		m_re += val;
		return *this;
	}

    complex& operator-= (const float val)
	{
		m_re -= val;
		return *this;
	}

    complex& operator*= (const float val)
	{
		m_re *= val;
		m_im *= val;
		return *this;
	}

    complex& operator/= (const float val)
	{
		m_re /= val;
		m_im /= val;
		return *this;
	}

    friend complex operator+ (const float left, const complex& right)
	{
		return complex(left + right.m_re, right.m_im);
	}

    friend complex operator- (const float left, const complex& right)
	{
		return complex(left - right.m_re, -right.m_im);
	}

    friend complex operator* (const float left, const complex& right)
	{
		return complex(left * right.m_re, left * right.m_im);
	}

    friend complex operator/ (const float left, const complex& right)
	{
        const float denominator = right.m_re * right.m_re + right.m_im * right.m_im;
		return complex(left * right.m_re / denominator,
			-left * right.m_im / denominator);
	}

	//   Boolean operators
    bool operator== (const complex &number) const
	{
        return m_re == number.m_re && m_im == number.m_im;
	}

    bool operator!= (const complex &number) const
	{
        return m_re != number.m_re || m_im != number.m_im;
	}

    bool operator== (const float val) const
	{
		return m_re == val && m_im == 0.;
	}

    bool operator!= (const float val) const
	{
		return m_re != val || m_im != 0.;
	}

    friend bool operator== (const float left, const complex& right)
	{
		return left == right.m_re && right.m_im == 0.;
	}

    friend bool operator!= (const float left, const complex& right)
	{
		return left != right.m_re || right.m_im != 0.;
	}
};

#endif
