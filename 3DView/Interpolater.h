#ifndef INTERPOLATER_H
#define INTERPOLATER_H
/*
   Interpolater c plus plus
   author junjie wang
   date   2014/9/28

*/
#include "BaseData.h"
#include "BaseException.h"

class InterpolaterException : public BaseException
{
public:
    InterpolaterException(string message) throw() : BaseException(message) {}
    InterpolaterException(string message, string location) throw() : BaseException(message, location) {}
};
template<class ForwardIterator>
class TInterpolater
{
public:
    virtual ~TInterpolater() {}
    virtual double GetInterpolatedZ(double xpos, double ypos, ForwardIterator first, ForwardIterator last)
        throw(InterpolaterException) = 0;
};

typedef TInterpolater<Point3D*> Interpolater;

#endif // INTERPOLATER_H
