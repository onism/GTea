#ifndef INVERSEDIST_H
#define INVERSEDIST_H
/*
   Interpolater c plus plus
   author junjie wang
   date   2014/9/28
   refer: http://www.docin.com/p-378263427.html
http://www.docin.com/p-256291450.html?bsh_platform=renren
http://ke.baidu.com/view/155359a0f121dd36a32d8292.html
*/
#include "Interpolater.h"
#include <cassert>
#include <cmath>
#include <vector>
struct WeightAndZ
{
    WeightAndZ(double w,double zz):weight(w), z(zz) {}
    double weight;
    double z;
};
//template<class ForwardIterator>
//class TInverseDist:public TInterpolater<ForwardIterator>
//{
//public:
//    TInverseDist(double dRadius=200,double dExponent=8)
//    throw() : m_dRadius(dRadius), m_dExponent(dExponent) {}
//    double GetInterpolatedZ(double xpos, double ypos,  ForwardIterator  first,  ForwardIterator last)
//    throw(InterpolaterException)
//    {
//        //check if Data is valid
//        assert(first != last);

//        double distance    = 0;
//        double weight      = 0;
//        double totalWeight = 0;
//        std::vector<WeightAndZ> vecWeight;
//        ForwardIterator start = first;
//        while(start != last){
//            distance = sqrt((::pow((xpos - (*start).x), 2) + ::pow((ypos - (*start).y), 2)));
//            if(distance == 0)
//                 return (*start).z;
//            if(distance > m_dRadius) {
//                  ++start;
//                  continue;
//              }
//            weight = 1 / ::pow(distance, m_dExponent);
//            vecWeight.push_back(WeightAndZ(weight, (*start).z));

//            totalWeight += weight;
//            ++start;
//        }
//        for(int i=0; i<vecWeight.size(); i++)
//                vecWeight[i].weight /= totalWeight;

//            double nodeValue = 0;
//            for(int i=0; i<vecWeight.size(); i++) {
//                nodeValue += vecWeight[i].weight * vecWeight[i].z;
//                ++first;
//            }
//            return nodeValue;
//    }

//private:
//    double m_dRadius;
//    double m_dExponent;
//};

//typedef TInverseDist<  Point3D*> InverseDist;

class TInverseDist
{
public:
    TInverseDist(double dRadius=200,double dExponent=8)
    throw() : m_dRadius(dRadius), m_dExponent(dExponent) {}
    double GetInterpolatedZ(double xpos, double ypos,  vector<Point3D> input )
    throw(InterpolaterException)
    {
        //check if Data is valid
      //  assert(first != last);

        double distance    = 0;
        double weight      = 0;
        double totalWeight = 0;
        std::vector<WeightAndZ> vecWeight;
        vector<Point3D>::iterator  start = input.begin();
        vector<Point3D>::iterator  first = input.begin();
        while(start != input.end()){
            distance = sqrt((::pow((xpos - (*start).x), 2) + ::pow((ypos - (*start).y), 2)));
            if(distance == 0)
                 return (*start).z;
            if(distance > m_dRadius) {
                  ++start;
                  continue;
              }
            weight = 1 / ::pow(distance, m_dExponent);
            vecWeight.push_back(WeightAndZ(weight, (*start).z));

            totalWeight += weight;
            ++start;
        }
        for(int i=0; i<vecWeight.size(); i++)
                vecWeight[i].weight /= totalWeight;

            double nodeValue = 0;
            for(int i=0; i<vecWeight.size(); i++) {
                nodeValue += vecWeight[i].weight * vecWeight[i].z;
                ++first;
            }
            return nodeValue;
    }

private:
    double m_dRadius;
    double m_dExponent;
};


#endif // INVERSEDIST_H
