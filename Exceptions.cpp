//Scott Nedvesky 104976030
//CSC 2312
//PA3
//March 25, 2016
//Exceptions.cpp

#include <iostream>
#include <string>

#include "Exceptions.h"

namespace Clustering
{
    // Out of Bounds
    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
        __current = c;
        __rhs = r;
        __name = "OutOfBoundsEx";
    }
    unsigned int OutOfBoundsEx::getCurrent() const
    {
        return __current;
    }
    int OutOfBoundsEx::getRhs() const
    {
        return __rhs;
    }
    std::string OutOfBoundsEx::getName() const
    {
        return __name;
    }
    std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx &ex) {
        os << ex.__name << " " << ex.__current <<  " " << ex.__rhs << std::endl;
        return os;
    }
    
    // Dimensionality Mismatch Exception
    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {
        __current = c;
        __rhs = r;
        __name = "DimensionalityMismatchEx";
    }
    
    unsigned int DimensionalityMismatchEx::getCurrent() const
    {
        return __current;
    }
    unsigned int DimensionalityMismatchEx::getRhs() const
    {
        return __rhs;
    }
    std::string DimensionalityMismatchEx::getName() const
    {
        return __name;
    }
    
    std::ostream& operator<<(std::ostream &os, const DimensionalityMismatchEx &ex)
    {
        os << ex.__name << ex.__current << ex.__rhs;
        return os;
    }
    
    // Zero Clusters Exception
    ZeroClustersEx::ZeroClustersEx() {
        __name = "ZeroClustersEx";
    }
    std::string ZeroClustersEx::getName() const
    {
        return __name;
    }
    std::ostream& operator<<(std::ostream &os, const ZeroClustersEx &ex)
    {
        os << ex.__name;
        return os;
    }
    // Data File Open Exception
    DataFileOpenEx::DataFileOpenEx(std::string filename) {
        __filename = filename;
        __name = "DataFileOpenEx";
    }
    std::string DataFileOpenEx::getFilename() const
    {
        return __filename;
    }
    std::string DataFileOpenEx::getName() const
    {
        return __name;
    }
    
    std::ostream& operator<<(std::ostream &os, const DataFileOpenEx &ex)
    {
        os << ex.__name << ", " << ex.__filename;
        return os;
    }
    
    // Zero Dimensions Exception
    ZeroDimensionsEx::ZeroDimensionsEx()
    {
        __name = "ZeroDimensionsEx";
    }
    std::string ZeroDimensionsEx::getName() const
    {
        return __name;
    }
    
    std::ostream& operator<<(std::ostream &os, const ZeroDimensionsEx &ex)
    {
        os << ex.__name;
        return os;
    }
    
    // Empty Cluster Exception
    EmptyClusterEx::EmptyClusterEx() {
        __name = "EmptyClusterEx";
    }
    
    std::string EmptyClusterEx::getName() const
    {
        return __name;
    }
    
    std::ostream& operator<<(std::ostream &os, const EmptyClusterEx &ex)
    {
        os << ex.__name;
        return os;
    }

}
