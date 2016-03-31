//Scott Nedvesky 104976030
//CSC 2312
//PA3
//March 25, 2016
//Cluster.cpp

#include <iostream>
#include <sstream>
#include <cassert>
#include <limits>

#include "Cluster.h"
#include "Exceptions.h"

using std::cout;
using std::endl;

namespace Clustering
{
    //2 argument constructor
    LNode::LNode(const Point & nPoint, LNodePtr nxtPtr):point(nPoint)
    {
        next=nxtPtr;
    }
    
    // Centroid
    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c): __dimensions(d), __c(c), __p(d)
    {
        __valid = false;
    }
    
    // Get/Set
    const Point Cluster::Centroid::get() const
    {
        return __p;
    }
    void Cluster::Centroid::set(const Point &p1)
    {
        __p = p1;
        __valid = true;
    }
    bool Cluster::Centroid::isValid() const
    {
        return __valid;
    }
    void Cluster::Centroid::setValid(bool valid)
    {
        __valid = valid;
    }
    
    // Functions
    
    void Cluster::Centroid::compute()
    {
        if(__c.__size > 0)
        {
            Point p1(__dimensions);
            LNodePtr temp = __c.__points;
            int iterator = 0;
            while (temp != nullptr)
            {
                p1 += temp->point / __c.getSize();
                ++iterator;
                temp = temp->next;
            }
            set(p1);
        }
        else if (__c.__size == 0)
        {
            assert(__c.__points == nullptr);
            __valid = true;
            toInfinity();
        }
    }
    bool Cluster::Centroid::equal(const Point &p1) const
    {
        if (p1 == __p)
            return true;
        return false;
    }
    void Cluster::Centroid::toInfinity()
    {
        for (int i = 0; i < __p.getDims(); ++i)
        {
            __p[i] = std::numeric_limits<double>::max();
        }
    }
    
    
    
    // Cluster
    
    unsigned int Cluster::__idGenerator = 0;
    
    // Delete
    
    void Cluster::__del()
    {
        while (__points != nullptr && __points->next != nullptr)
        {
            int i = 1;
            LNodePtr tempPtr;
            tempPtr = __points->next;
            delete __points;
            __points = tempPtr;
            ++i;
        }
        if (__points != nullptr && __points->next == nullptr)
        {
            delete __points;
        }
    }
    
    // Copy
    
    void Cluster::__cpy(LNodePtr points)
    {
        if (points == nullptr)
            return;
        this->__points = new LNode(points->point, nullptr);
        this->__size++;
        
        points = points->next;
        while (points != nullptr)
        {
            this->add(points->point);
            points = points->next;
        }
    }
    
    // Point Compare
    
    bool Cluster::__in(const Point &p1) const
    {
        if (__dimensionality != p1.getDims())
        {
            throw DimensionalityMismatchEx(__dimensionality, p1.getDims());
        }
        if (__size == 0)
        {
            return false;
        }
        LNodePtr current = __points;
        while (current->next != nullptr)
        {
            if (current->point.getId() == p1.getId())
            {
                return true;
            }
            else
            {
                current = current->next;
            }
        }
        if (current->point.getId() == p1.getId())
        {
            return true;
        }
        return false;
    }
    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';
    
    // One Arg Constructor
    
    Cluster::Cluster(unsigned int d): __size(0), __points(nullptr), __dimensionality(d), centroid(d, *this)
    {
        __id = __idGenerator;
        __idGenerator++;
    }
    
    // Copy Constructor
    
    Cluster::Cluster(const Cluster &c) :__size(0), __dimensionality(c.__dimensionality), centroid(c.__dimensionality, c)
    {
        if (c.__size == 0)
        {
            __points = nullptr;
            __id = c.__id;
        }
        else if (c.__size == 1)
        {
            __points = new LNode(c.__points->point, nullptr);
            __size++;
            __id = c.__id;
        }
        else if (c.__size > 1)
        {
            LNodePtr temp = c.__points;
            __cpy(temp);
            __id = c.__id;
        }
        centroid.compute();
        assert(__size == c.__size);
    }
    
    // Overloaded Assignment Operator
    
    Cluster& Cluster::operator=(const Cluster &c1)
    {
        
        if (this == &c1)
        {
            return *this;
        }
        else
        {
            __del();
            LNodePtr temp = c1.__points;
            __size = 0;
            __cpy(temp);
            centroid.compute();
            assert(this->__size == c1.__size);
            __id = c1.__id;
            return *this;
        }
    }
    
    // Destructor
    Cluster::~Cluster()
    {
        if (__points != NULL)
        {
            LNodePtr previousPtr = __points;
            LNodePtr currentPtr = NULL;
            while (previousPtr != NULL)
            {
                currentPtr = previousPtr->next;
                delete  previousPtr;
                previousPtr = currentPtr;
            }
        }
    }
    
    // Get/Set
    unsigned int Cluster::getSize() const
    {
        return __size;
    }
    unsigned int Cluster::getDimensionality() const
    {
        return __dimensionality;
    }
    unsigned int Cluster::getId() const
    {
        return __id;
    }
    
    // Set functions
    void Cluster::add(const Point &point)
    {
        if (point.getDims() != __dimensionality)
        {
            throw DimensionalityMismatchEx(__dimensionality, point.getDims());
        }
        LNodePtr temp = new LNode(point, nullptr);
        LNodePtr prev = __points;
        LNodePtr curr = __points;
        
        if (__size == 0)
        {
            __points = temp;
            __size++;
        }
        else if (__size == 1)
        {
            if (temp->point < curr->point)
            {
                LNodePtr foo = __points;
                temp->next = foo;
                __points = temp;
                __size++;
            }
            else
            {
                __points->next = temp;
                __size++;
            }
        }
        else if (__size > 1)
        {
            int i = 0;
            
            while (curr->next != nullptr)
            {
                if (temp->point < curr->point)
                {
                    if (curr == __points)
                    {
                        LNodePtr foo = __points;
                        temp->next = foo;
                        __points = temp;
                        break;
                    }
                    else
                    {
                        temp->next = curr;
                        prev->next = temp;
                        break;
                    }
                }
                
                curr = curr->next;
                
                if (i > 0)
                {
                    prev = prev->next;
                }
                i++;
            }
            if (curr->next == nullptr)
            {
                curr->next = temp;
            }
            __size++;
            centroid.setValid(false);
        }
    }
    
    const Point& Cluster::remove(const Point &p)
    {
        if (p.getDims() != __dimensionality)
        {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        LNodePtr prev = nullptr, del = nullptr;
        if (__points == nullptr) {
            assert(__size == 0);
        } else if (__points->point == p) {
            del = __points;
            __points = __points->next;
            delete del;
            __size--;
        } else {
            prev = __points;
            del = __points->next;
            while (del != nullptr) {
                if (del->point == p) {
                    prev->next = del->next;
                    delete del;
                    __size--;
                    break;
                }
                prev = del;
                del = del->next;
            }
        }
        if (__size == 0)
        {
            centroid.setValid(false);
        }
        return p;
    }
    
    bool Cluster::contains(const Point &p) const
    {
        if (__dimensionality != p.getDims())
        {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        for (LNodePtr pCursor = this->__points; pCursor != nullptr; pCursor = pCursor->next)
        {
            if (pCursor->point == p){
                return true;
            }
        }
        return false;
    }
    
    
    void Cluster::pickCentroids(unsigned int k, Point **pArray)
    {
        LNodePtr curr = __points;
        Point inf(__dimensionality);
        for (int i = 0; i < __dimensionality; ++i)
        {
            inf[i] = std::numeric_limits<double>::max();
        }
        if (k >= __size)
        {
            for (int i = 0; i < __size; ++i)
            {
                *pArray[i] = curr->point;
                curr = curr->next;
            }
            for (int i = __size; i < k; ++i)
            {
                *pArray[i] = inf;
            }
        }
        else if (k < __size)
        {
            for (int i = 0; i < k; ++i)
            {
                *pArray[i] = curr->point;
                curr = curr->next;
            }
        }
    }
    
    // Members: Subscript
    const Point& Cluster::operator[](unsigned int idx) const
    {
        if (__size <= 0)
        {
            throw EmptyClusterEx();
        }
        if (idx >= __size)
        {
            throw OutOfBoundsEx(__size, idx);
        }
        
        LNodePtr pCursor=__points;
        for(int i=0;(i<idx) && (pCursor->next!=nullptr); i++)
            pCursor=pCursor->next;
        return pCursor->point;
    }
    
    //Members: Compound
    Cluster& Cluster::operator+=(const Point &point1)
    {
        if (point1.getDims() != __dimensionality)
        {
            throw DimensionalityMismatchEx(__dimensionality, point1.getDims());
        }
        add(point1);
        centroid.setValid(false);
        return *this;
    }
    
    Cluster& Cluster::operator-=(const Point &point1)
    {
        if (point1.getDims() != __dimensionality)
        {
            throw DimensionalityMismatchEx(__dimensionality, point1.getDims());
        }
        if (this->contains(point1))
        {
            remove(point1);
            centroid.setValid(false);
        }
        
        return *this;
    }
    
    // Members: Compound assignment (Cluster argument)
    Cluster& Cluster::operator+=(const Cluster &clust)
    {
        LNodePtr temp = clust.__points;
        while (temp != nullptr)
        {
            if (!this->__in(temp->point))
            {
                this->add(temp->point);
            }
            temp = temp->next;
        }
        return *this;
    }
    
    Cluster& Cluster::operator-=(const Cluster &clust)
    {
        LNodePtr temp = clust.__points;
        while (temp != nullptr)
        {
            if (__in(temp->point))
            {
                remove(temp->point);
                centroid.setValid(false);
            }
            temp = temp->next;
        }
        if (__size == 0)
        {
            centroid.setValid(false);
        }
        return *this;
    }
    
    // Friends: IO
    std::ostream& operator<<(std::ostream &oStream, const Cluster &clust)
    {
        LNodePtr curr = clust.__points;
        if (clust.__size == 0)
        {
            return oStream;
        }
        else
        {
            while (curr->next != nullptr)
            {
                oStream << curr->point << ' ' << Cluster::POINT_CLUSTER_ID_DELIM << ' ' << clust.__id << endl;
                curr = curr->next;
            }
            oStream << curr->point;
            oStream << ' ' << Cluster::POINT_CLUSTER_ID_DELIM << ' ' << clust.__id;
            return oStream;
        }
    }
    
    std::istream& operator>>(std::istream &iStream, Cluster &clust)
    {
        
        std::string wholeLine;
        
        while (getline(iStream,wholeLine))
        {
            Point p(clust.__dimensionality);
            std::stringstream lineStream(wholeLine);
            std::string value;
            int i = 0;
            double d = 0.0;
            while (getline(lineStream, value, Point::POINT_VALUE_DELIM))
            {
                try
                {
                    if (i >= clust.__dimensionality)
                    {
                        p.rewindIdGen();
                        throw OutOfBoundsEx(clust.__dimensionality, i);
                    }
                    d = stod(value);
                    p.setValue(i, d);
                }
                catch (OutOfBoundsEx &ex)
                {
                    std::cerr << "Caught Exception: " << ex << endl;
                    p.rewindIdGen();
                }
                ++i;
            }
            try
            {
                if (i != clust.__dimensionality)
                {
                    throw DimensionalityMismatchEx(clust.__dimensionality, i);
                }
                clust.add(p);
            }
            catch (DimensionalityMismatchEx &ex)
            {
                std::cerr << "Caught Exception: " << ex << endl;
                
            }
        }
        
        return iStream;
    }
    
    // Friends: Comparison
    bool operator==(const Cluster &c1, const Cluster &c2)
    {
        if (c1.__dimensionality != c2.__dimensionality)
        {
            throw DimensionalityMismatchEx(c1.__dimensionality, c2.__dimensionality);
        }
        
        if (c1.__size != c2.__size)
        {
            return false;
        }
        
        LNodePtr curr1 = c1.__points;
        LNodePtr curr2 = c2.__points;
        bool same = true;
        
        while (curr1 != NULL && curr2 != NULL)
        {
            if (curr1->point != curr2->point)
            {
                same = false;
            }
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        return same;
    }
    
    bool operator!=(const Cluster &c1, const Cluster &c2)
    {
        if (c1.__dimensionality != c2.__dimensionality)
        {
            throw DimensionalityMismatchEx(c1.__dimensionality, c2.__dimensionality);
        }
        
        return (!(c1==c2));
    }
    
    // Friends: Arithmetic (Cluster and Point)
    const Cluster operator+(const Cluster &clust, const Point &point)
    {
        if (clust.__dimensionality != point.getDims())
        {
            throw DimensionalityMismatchEx(clust.__dimensionality, point.getDims());
        }
        
        Cluster clust2(clust);
        clust2+=point;
        
        return clust2;
    }
    
    const Cluster operator-(const Cluster &clust1, const Point &point1)
    {
        if (clust1.__dimensionality != point1.getDims())
        {
            throw DimensionalityMismatchEx(clust1.__dimensionality, point1.getDims());
        }
        
        Cluster clust2(clust1);
        clust2-=point1;
        return clust2;
    }
    
    // Friends: Arithmetic
    
    const Cluster operator+(const Cluster &clust1, const Cluster &clust2)
    {
        if (clust1.__dimensionality != clust2.__dimensionality)
        {
            throw DimensionalityMismatchEx(clust1.__dimensionality, clust2.__dimensionality);
        }
        
        Cluster k(clust1);
        k+=clust2;
        return k;
    }
    
    const Cluster operator-(const Cluster &clust1, const Cluster &clust2)
    {
        if (clust1.__dimensionality != clust2.__dimensionality)
        {
            throw DimensionalityMismatchEx(clust1.__dimensionality, clust2.__dimensionality);
        }
        
        Cluster k(clust1);
        k-=clust2;
        return k;
    }
    
    // Constructor
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to)
    {
    }
    
    // Methods
    void Cluster::Move::perform()
    {
        __to.add(__from.remove(__p));
    }
}