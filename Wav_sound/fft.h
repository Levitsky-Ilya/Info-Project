//   fft.h - declaration of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   with additions by Nikulina and Kataeva
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _FFT_H_
#define _FFT_H_

//   Include complex numbers header
#include "complex.h"

#define HAM_CONST1 0.54
#define HAM_CONST2 0.46
#define GAUSSE_CONST 0.5

class CFFT
{
public:
	//   FORWARD FOURIER TRANSFORM
	//     Input  - input data
    //     Output - transform result (amplitudes)
	//     N      - length of both input data and result
     bool fftAlgorithm(complex *const Input, float *const Output, const unsigned int N);
     void applyWindowH(complex *const Data, const unsigned int N);
     void applyWindowG(complex *const Data, const unsigned int N);


protected:
	//   Rearrange function and its inplace version
	static void Rearrange(const complex *const Input, complex *const Output, const unsigned int N);
	//   FFT implementation
	static void Perform(complex *const Data, const unsigned int N, const bool Inverse = false);
    //  Calculating amplitude
    void countAmplitude(complex *const Input, float *const Output, const unsigned int N);
    float Hamming(unsigned int i, unsigned int N);
    float Gausse(unsigned int i, unsigned int N);


};

#endif
