#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H
/*
   exception c plus plus
   author junjie wang
   date   2014/9/28
   refer: http://www.cnblogs.com/LeftNotEasy/archive/2010/10/30/1865364.html
*/


#include <stdexcept>
#include <string>
#include <iostream>
using namespace  std;

#pragma warning(disable:4290)

#define LOCATION Location(__FILE__,__LINE__)

inline std::string Location(std::string file,int lineNo)
{
    char buf[64];
    sprintf(buf,"line no.%d",lineNo);
    return file + buf;
}

class BaseException : public std::runtime_error
{
public:
    BaseException() throw():runtime_error("error"){}
    BaseException(std::string message) throw():runtime_error(message){}
    BaseException(std::string message, std::string location) throw() : std::runtime_error(message+location) {}
    void Log(std::ostream& os) throw() { os << what() << std::endl; }
};

#endif // BASEEXCEPTION_H
