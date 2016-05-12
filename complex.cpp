//   complex.cpp - impelementation of class
//   of complex number
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

//   Include header file
#include "complex.h"
#include <iostream>
//#include <QTextStream>


//   Imaginary unity constants
const complex complex::i(0., 1.);
const complex complex::j(0., 1.);

void complex::print()
{
   std::cout << "re= " << m_re << " im = " << m_im << "\n"  ;
}

