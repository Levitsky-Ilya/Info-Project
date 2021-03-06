#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

#define DECLARE_EXC(name)                                \
class name : public std::runtime_error {                 \
public:                                                  \
    name(const std::string &str) : runtime_error(str) {} \
}
// define DECLARE_EXC

namespace NotesExceptions
{
DECLARE_EXC(OpenFile);
DECLARE_EXC(ReadFile);
DECLARE_EXC(FalseFormat);
DECLARE_EXC(FileDamaged);
DECLARE_EXC(UnexpDepth);
DECLARE_EXC(BadAlloc);
DECLARE_EXC(Connect);
}

#endif // EXCEPTIONS_H

