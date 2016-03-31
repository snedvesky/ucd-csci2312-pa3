//Scott Nedvesky 104976030
//CSC 2312
//PA3
//March 25, 2016
//Kmeans.cpp

#include <fstream>
#include <iostream>
#include <limits>

#include "Cluster.h"
#include "Exceptions.h"
#include "KMeans.h"

namespace Clustering
{
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {
        if(k != 0) {
            __iFileName = filename;
            std::ifstream file(__iFileName);
            if(__iFileName == "") {
                throw DataFileOpenEx(__iFileName);
            }
            if (file) {
                __dimensionality = dim;
                __k = k;
                __maxIter = maxIter;
                __numNonempty = 1;
                __clusters = new Cluster *[__k];
                for (int i = 0; i < __k; i++) {
                    __clusters[i] = new Cluster(__dimensionality);
                }
                file >> *(__clusters[0]);
                file.close();
                __initCentroids = new Point *[__k];
                for (int i = 0; i < __k; i++) {
                    __initCentroids[i] = new Point(__dimensionality);
                }
                __clusters[0]->pickCentroids(__k, __initCentroids);
            } else {
                throw DataFileOpenEx(__iFileName);
            }
        } else {
            throw ZeroClustersEx();
        }
    }

    KMeans::~KMeans()
    {
        for(int count = 0; count < __k; count++) {
            delete __clusters[count];
            delete __initCentroids[count];
        }
        delete []__clusters;
        delete []__initCentroids;
        __clusters = nullptr;
        __initCentroids = nullptr;
    }
    
    // accessors
    
    unsigned int KMeans::getMaxIter()
    {
        return __maxIter;
    }
    unsigned int KMeans::getNumIters()
    {
        return __numIter;
    }
    unsigned int KMeans::getNumNonemptyClusters()
    {
        return __numNonempty;
    }
    unsigned int KMeans::getNumMovesLastIter()
    {
        return __numMovesLastIter;
    }
    
    // element access (for testing, no bounds checking)
    
    Cluster& KMeans::operator[](unsigned int u)
    {
        return *__clusters[u];
    }
    const Cluster& KMeans::operator[](unsigned int u) const
    {
        return *__clusters[u];
    }
    
    // write out the results to a file
    
    std::ostream& operator<<(std::ostream &os, const KMeans &kmeans)
    {
        Point p(kmeans.__dimensionality);
        for (int i = 0; i < kmeans.__dimensionality; ++i)
        {
            p[i] = std::numeric_limits<double>::max();
        }
        for (int i = 0; i < kmeans.__k; ++i)
        {
            if (!kmeans.__clusters[i]->centroid.equal(p))
            {
                os << *kmeans.__clusters[i] << std::endl;
            }
        }
        return os;
    }
    
    // clustering functions
    
    void KMeans::run()
    {
        int m = 100;
        int iterator = 0;
        int nEmpty = 0;
        
        while (m > 0 && iterator < __maxIter)
        {
            m = 0;
            for (int i = 0; i < __k; ++i)
            {
                for (int j = 0; j < __clusters[i]->getSize(); ++j)
                {
                    Cluster &clust = *(__clusters[i]);
                    Point curr_point(__dimensionality);
                    curr_point = clust[j];
                    int sDistanceIdx = 0;
                    double sDistance = curr_point.distanceTo(*__initCentroids[0]);
                    for (int e = 0; e < __k; e++)
                    {
                        if (curr_point.distanceTo(*__initCentroids[e]) < sDistance)
                        {
                            sDistance = curr_point.distanceTo(*__initCentroids[e]);
                            sDistanceIdx = e;
                        }
                    }
                    Cluster::Move change_clusters(curr_point, *__clusters[i], *__clusters[sDistanceIdx]);
                    change_clusters.perform();
                    for (int c = 0; c < __k; ++c)
                    {
                        __clusters[c]->centroid.compute();
                    }
                    if (*__clusters[i] != *__clusters[sDistanceIdx])
                    {
                        m++;
                    }
                }
            }
            iterator++;
        }
        Point inf(__dimensionality);
        for (int i = 0; i < __dimensionality; ++i)
        {
            inf[i] = std::numeric_limits<double>::max();
        }
        
        for (int i = 0; i < __k; ++i)
        {
            if (__clusters[i]->centroid.get() != inf)
            {
                ++nEmpty;
            }
        }
        __numIter = iterator;
        __numMovesLastIter = m;
        __numNonempty = nEmpty;
    }
}