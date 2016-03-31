//Scott Nedvesky 104976030
//CSC 2312
//PA3
//March 25, 2016
//Point.cpp

#include <cmath>
#include <iostream>
#include <cassert>

#include "Point.h"
#include "Exceptions.h"
#include "Cluster.h"


namespace Clustering
{
    
    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';
    
    void Point::rewindIdGen()
    {
        __idGen--;
    }
    
    //One argument constructor
    Point::Point(unsigned int __nDimension)
    {
        if (__nDimension == 0)
        {
            throw ZeroDimensionsEx();
        }
        __dim = __nDimension;
        __values = new double[__dim];
        for (int i = 0; i < __dim; i++)
            __values[i] = 0.0;
        __id=__idGen++;
    }
    
    
    // Big three: cpy ctor, overloaded operator=, dtor
    
    Point::Point(const Point &copy)
    {
        __dim = copy.__dim;
        __values = new double[__dim];
        for (int i = 0; i < __dim; i++)
            __values[i] = copy.__values[i];
        __id = copy.__id;
    }
    
    
    Point& Point::operator=(const Point &p)
    {
        
        if (__dim != p.__dim)
        {
            throw DimensionalityMismatchEx(__dim, p.__dim);
        }
        __id = ++__idGen;
        __dim = p.getDims();
        delete[]__values;
        __values = new double[__dim];
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] = p.getValue(i);
        }
        return *this;
    }
    
    Point::~Point()
    {
        delete [] __values;
    }
    
    // Accessors & Mutators
    
    int Point::getId() const
    {
        return __id;
    }
    
    unsigned int Point::getDims() const
    {
        return __dim;
    }
    
    void Point::setValue(unsigned int idx, double nValue)
    {
        if (idx >= __dim)
        {
            throw OutOfBoundsEx(__dim, idx);
        }
        __values[idx] = nValue;
    }
    
    double Point::getValue(unsigned int idx) const
    {
        if (idx >= __dim)
        {
            throw OutOfBoundsEx(__dim, idx);
        }
        return __values[idx];
    }
    
    // Functions
    double Point::distanceTo(const Point &dPoint) const
    {
        if (__dim != dPoint.getDims())
        {
            throw DimensionalityMismatchEx(__dim, dPoint.getDims());
        }
        double SqrOfD =0;
        double i1 = 0;
        double i2 = 0;
        
        for (int i = 0; i < __dim; ++i)
        {
            i1 = dPoint.__values[i] - __values[i];
            i2 = dPoint.__values[i] - __values[i];
            SqrOfD +=(i1 * i2);
        }
        return (sqrt(SqrOfD));
    }
    
    //Members
    
    Point& Point::operator*=(double right)
    {
        for (int i = 0; i < __dim; i++)
            __values[i] *= right;
        return *this;
    }
    
    Point& Point::operator/=(double Q)
    {
        for (int i = 0; i < __dim; i++)
            __values[i] /= Q;
        return *this;
    }
    
    const Point Point::operator*(double Q) const
    {
        Point p(*this);
        p *= Q;
        return p;
    }
    
    const Point Point::operator/(double Q) const
    {
        Point p(*this);
        p /= Q;
        return p;
    }
    
    double& Point::operator[](unsigned int idx)
    {
        if (idx >= __dim)
        {
            throw OutOfBoundsEx(__dim, idx);
        }
        return __values[idx];
    }
    const double& Point::operator[](unsigned int idx) const
    {
        if (idx >= __dim)
        {
            throw OutOfBoundsEx(__dim, idx);
        }
        return __values[idx];
    }
    
    //Friends
    
    Point& operator+=(Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        for (int i = 0; i < left.__dim; ++i) {
            left.__values[i] += right.__values[i];
        }
        return left;
    }
    
    Point& operator-=(Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        for (int i = 0; i < left.__dim; ++i) {
            left.__values[i] -=right.__values[i];
        }
        return left;
    }
    
    const Point operator+(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        Clustering::Point tempP(left);
        tempP +=right;
        return tempP;
    }
    
    const Point operator-(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        Clustering::Point tempP(left);
        tempP -=right;
        return tempP;
    }
    
    bool operator==(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        
        for (int i = 0; i < left.__dim; ++i)
        {
            if (left.__values[i] != right.__values[i])
                return false;
        }
        return true;
    }
    
    bool operator!=(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        
        if (left == right)
        {
            return false;
        }
        return true;
    }
    
    bool operator<(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        bool L = false;
        for (int i = 0; i < left.__dim; i++)
        {
            if (left.__values[i] < right.__values[i])
                return true;
            if(left.__values[i]>right.__values[i])
                return false;
        }
        return L;
    }
    
    bool operator>(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        for (int i = 0; i < left.__dim; ++i)
        {
            if (left.__values[i] > right.__values[i])
                return true;
        }
        return false;
    }
    
    bool operator<=(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        
        if (left > right)
            return false;
        else
            return true;
    }
    
    bool operator>=(const Point &left, const Point &right)
    {
        if (left.__dim != right.__dim)
        {
            throw DimensionalityMismatchEx(left.__dim, right.__dim);
        }
        
        if (left < right)
            return false;
        else
            return true;
    }
    
    std::ostream& operator<<(std::ostream &os, const Point &right)
    {
        int i = 0;
        for (; i < right.__dim - 1; ++i)
        {
            os << right.__values[i] << ", ";
        }
        os << right.__values[i];
        return os;
    }
    
    std::istream& operator>>(std::istream &is, Point &right)
    {
        std::string value;
        double d;
        int i = 0;
        
        while (getline(is, value, Point::POINT_VALUE_DELIM))
        {
            d = stod(value);
            right.__values[i++] = d;
        }
        if (i != right.__dim)
        {
            Point::rewindIdGen();
            throw DimensionalityMismatchEx(right.__dim, i);
        }
        return is;
    }
}