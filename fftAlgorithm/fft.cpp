//   fft.cpp - impelementation of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

//   Include declaration file
//#include "fft.h"
//   Include math library
#include <math.h>
#include "complex.h"
#include "fft.h"

#define M_PI 3.14159265358979

//   FORWARD FOURIER TRANSFORM
//     Input  - input data
//     Output - transform result
//     N      - length of both input data and result

bool CFFT::fftAlgorithm(complex *const Input, double *const Output, const unsigned int N)
{
	//   Check input parameters
    if (!Input || !Output || N < 1 || N & (N - 1))
		return false;
	//   Rearrange
    complex fftOutput[N];

    Rearrange(Input, fftOutput, N);
	//   Call FFT implementation

    Perform(fftOutput, N);
    countAmplitude(fftOutput, Output, N);

	//   Succeeded
	return true;
}


 double CFFT::Hamming(unsigned int i, unsigned int N)
{
    double a = 0;
    a =  0.54-0.46*cos(2*M_PI*i/(N-1));
    return a;

}

 void CFFT::applyWindow(complex *Data, unsigned int N)
 {
     for (unsigned int i = 0; i < N; i++){
         Data[i]*=Hamming(i, N);
         }
 }


//
void CFFT::countAmplitude(complex *const Input, double *const Output, const unsigned int N)
{
    unsigned int i = 0;
    for (i = 0; i < N; i++) {
            Output[i] = Input[i].countAmplitude(); //Fill amplitude array with data
        }

}

//   Rearrange function
void CFFT::Rearrange(const complex *const Input, complex *const Output, const unsigned int N)
{
	//   Data entry position
	unsigned int Target = 0;
	//   Process all positions of input signal
	for (unsigned int Position = 0; Position < N; ++Position)
	{
		//  Set data entry
		Output[Target] = Input[Position];
		//   Bit mask
		unsigned int Mask = N;
		//   While bit is set
		while (Target & (Mask >>= 1))
			//   Drop bit
			Target &= ~Mask;
		//   The current bit is 0 - set it
		Target |= Mask;
	}
}


//   FFT implementation
void CFFT::Perform(complex *const Data, const unsigned int N, const bool Inverse /* = false */)
{
	const double pi = Inverse ? 3.14159265358979323846 : -3.14159265358979323846;
	//   Iteration through dyads, quadruples, octads and so on...
	for (unsigned int Step = 1; Step < N; Step <<= 1)
	{
		//   Jump to the next entry of the same transform factor
		const unsigned int Jump = Step << 1;
		//   Angle increment
		const double delta = pi / double(Step);
		//   Auxiliary sin(delta / 2)
		const double Sine = sin(delta * .5);
		//   Multiplier for trigonometric recurrence
		const complex Multiplier(-2. * Sine * Sine, sin(delta));
		//   Start value for transform factor, fi = 0
		complex Factor(1.);
		//   Iteration through groups of different transform factor
		for (unsigned int Group = 0; Group < Step; ++Group)
		{
			//   Iteration within group 
			for (unsigned int Pair = Group; Pair < N; Pair += Jump)
			{
				//   Match position
				const unsigned int Match = Pair + Step;
				//   Second term of two-point transform
				const complex Product(Factor * Data[Match]);
				//   Transform for fi + pi
				Data[Match] = Data[Pair] - Product;
				//   Transform for fi
				Data[Pair] += Product;
			}
			//   Successive transform factor via trigonometric recurrence
			Factor = Multiplier * Factor + Factor;
		}
	}
}




