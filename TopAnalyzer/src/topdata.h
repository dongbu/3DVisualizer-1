#ifndef TopData_H
#define TopData_H

#include <vector>
#include <iostream>
#include <cmath>

struct TopData 
{
    DataType* TopData; //the Data array
    size_t size[3]; //dimensions
    size_t totalSize; //product of the above 3 integers

    DataType maxValue, minValue; //max and min values occuring in the TopData

    std::vector< DataType > saddles; //we will fill this up with all of the saddles that exist in the TopData

    TopData() : TopData(0),totalSize(0)
    {
        size[0] = size[1] = size[2] = 0;
    }
    
    TopData(DataType* arr, size_t* dims);

    

    ~TopData()
    {
        if (TopData) delete[] TopData;
    }

    //1 dimensional referencing
    DataType & operator[](size_t i)
    {
        if (i < totalSize) return TopData[i];
        else return saddles[i - totalSize];
    }

    size_t convertIndex( size_t i, size_t j, size_t k )
    {
        return (k * size[1] + j) * size[0] + i;
    }

    void convertIndex( size_t id, size_t & x, size_t & y, size_t & z )
    {
        if (id >= totalSize) {
            std::cerr << "Error: trying to convert the index of a saddle into xyz" << std::endl;
            x = y = z = 0xffffffff;
            return;
        }

        int size01 = size[0] * size[1];

        z = id / size01;
        y = (id - z*size01) / size[0];
        x = id - z*size01 - y*size[0];
    }

    bool greater(size_t a, size_t b);
    bool less(size_t a, size_t b);
    bool load( const char * filename, char * prefix, bool * compressed ); //prefix and compressed are written to

};

#endif // TopData_H
