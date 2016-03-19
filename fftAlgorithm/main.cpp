//   fft.cpp - impelementation of class
//
//
//   The code is written with use of LIBROW's materilas
//   It converts array of complex numbers using FFT
//   into array of



#include <iostream>
#include "complex.h"
#include "fft.h"
using namespace std;

int main()
{

    complex *pSignal = new complex[1024]; //Allocate memory for signal data
    int i = 0, k = 0, l = 0;
    double smth = 0;
    for (i = 0; i < 1024; i++)
    {
        smth = i* 3.141596/2;
        pSignal[i] = complex(smth, 0.0); //Fill signal array with data
        //pSignal[i] = complex(smth, 0.0); //Fill signal array with data
    }
    double resarr[1024]; //Allocate memory for magnitude data

    CFFT chtoto;

     chtoto.fftAlgorithm(pSignal,resarr, 1024); //Apply FFT
/*
    pSignal[0].print();
    pSignal[1].print();


    for (k = 0; k < 1024; k++)
    {
        resarr[k] = pSignal[k].countMagnitude(); //Fill magnitude array with data
    }
*/
    cout << "after norm resarr = " << resarr[0] <<endl;
    cout << "after norm resarr = " << resarr[1] <<endl;
     cout << "after norm resarr = " << resarr[2] <<endl;
      cout << "after norm resarr = " << resarr[3] <<endl;


     resarr[0] = pSignal[0].countMagnitude();
    cout << "magnitude for  0 resarr = " << resarr[0] <<endl;
                 
    delete [] pSignal; //free memory
   // delete[] resarr; //free memory
    cout << "resarr = " << resarr[0] <<endl;
    cout << "resarr = " << resarr[0] <<endl;



          

    return 0;
}

