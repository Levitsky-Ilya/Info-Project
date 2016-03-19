//   fft.h - declaration of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _FFT_H_
#define _FFT_H_

//   Include complex numbers header
#include "complex.h"

class CFFT
{
public:
	//   FORWARD FOURIER TRANSFORM
	//     Input  - input data
	//     Output - transform result
	//     N      - length of both input data and result
     bool fftAlgorithm(complex *const Input, double *const Output, const unsigned int N);
	//   FORWARD FOURIER TRANSFORM, INPLACE VERSION
	//     Data - both input data and output
	//     N    - length of input data
    //static bool Forward(complex *const Data, const unsigned int N);


protected:
	//   Rearrange function and its inplace version
	static void Rearrange(const complex *const Input, complex *const Output, const unsigned int N);
	static void Rearrange(complex *const Data, const unsigned int N);

	//   FFT implementation
	static void Perform(complex *const Data, const unsigned int N, const bool Inverse = false);

	//   Scaling of inverse FFT result
	static void Scale(complex *const Data, const unsigned int N);
    void countAmplitude(complex *const Input, double *const Output, const unsigned int N);
};

#endif
