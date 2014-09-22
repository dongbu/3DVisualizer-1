#include "mesh.h"

#include <tbb/tbb.h>
#include <tbb/parallel_sort.h>
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::sort;

//functor for sorting
class AscendingOrder
{
    Data* data;
public:
    AscendingOrder(Data* d ) : data(d) {}

    bool operator()(const size_t & a, const size_t & b) const {
        if(data->data[a] == data->data[b]) return a < b;
        return data->data[a] < data->data[b];
    }
};

void Mesh::createGraph(std::vector<size_t>& order) 
{
    order.resize(data.totalSize);
    for(unsigned int i = 0; i < data.totalSize; i++) order[i] = i;
    tbb::parallel_sort(order.begin(), order.end(), AscendingOrder(&data));
}

void Mesh::getNeighbors(size_t i, std::vector<size_t>& n) 
{
    uint x, y, z;
    data.convertIndex(i, x, y, z);
    if((x + y + z) % 2 == 1)
        find6Neighbors(x, y, z, n);
    else
        find18Neighbors(x, y, z, n);
}

size_t Mesh::getNeighbors(size_t idx, size_t* nbrs, size_t nbrs_size)
{
    uint x, y, z;
    data.convertIndex(idx, x, y, z);
    std::memset(nbrs, 0, nbrs_size * sizeof(size_t));
    if((x + y + z) % 2 == 1)
        find6Neighbors(x, y, z, nbrs);
    else
        find18Neighbors(x, y, z, nbrs);

    int sz = 0;
    for(size_t i = 0; i < nbrs_size; i++) if(nbrs[i] != 0) sz++;
    return sz;
}

void Mesh::find6Neighbors( uint x, uint y, uint z, size_t* nbrs)
{
    uint nx[6],ny[6],nz[6];

    for (uint i = 0; i < 6; i++) {
        nx[i] = x;
        ny[i] = y;
        nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    for(uint i = 0, j = 0; i < 6; i++) {
        if (nx[i] >= data.size[0]) continue;
        if (ny[i] >= data.size[1]) continue;
        if (nz[i] >= data.size[2]) continue;

        nbrs[j++] = data.convertIndex(nx[i],ny[i],nz[i]);
    }
}

void Mesh::find6Neighbors( uint x, uint y, uint z, std::vector< size_t > & neighbors) 
{
    uint nx[6],ny[6],nz[6];

    for (uint i = 0; i < 6; i++) {
        nx[i] = x;
        ny[i] = y;
        nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    for (uint i = 0; i < 6; i++) {
        if (nx[i] >= data.size[0]) continue;
        if (ny[i] >= data.size[1]) continue;
        if (nz[i] >= data.size[2]) continue;

        neighbors.push_back( data.convertIndex(nx[i],ny[i],nz[i]) );
    }
}

void Mesh::find18Neighbors( uint x, uint y, uint z, std::vector< size_t > & neighbors) 
{
    uint nx[18],ny[18],nz[18];

    for (uint i = 0; i < 18; i++) {
        nx[i] = x;
        ny[i] = y;
        nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    //the rest of the 18
    nx[6] -= 1; ny[6]  -= 1;
    nx[7] += 1; ny[7]  -= 1;
    ny[8] -= 1; nz[8]  -= 1;
    ny[9] += 1; nz[9]  -= 1;
    nz[10]-= 1; nx[10] -= 1;
    nz[11]+= 1; nx[11] -= 1;

    nx[12] -= 1; ny[12] += 1;
    nx[13] += 1; ny[13] += 1;
    ny[14] -= 1; nz[14] += 1;
    ny[15] += 1; nz[15] += 1;
    nz[16] -= 1; nx[16] += 1;
    nz[17] += 1; nx[17] += 1;

    for (uint i = 0; i < 18; i++) {


        if (nx[i] >= data.size[0]) continue;
        if (ny[i] >= data.size[1]) continue;
        if (nz[i] >= data.size[2]) continue;

        neighbors.push_back( data.convertIndex(nx[i],ny[i],nz[i]) );
    }
}

void Mesh::find18Neighbors( uint x, uint y, uint z, size_t* nbrs)
{
    uint nx[18],ny[18],nz[18];

    for (uint i = 0; i < 18; i++) {
        nx[i] = x;
        ny[i] = y;
        nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    //the rest of the 18
    nx[6] -= 1; ny[6]  -= 1;
    nx[7] += 1; ny[7]  -= 1;
    ny[8] -= 1; nz[8]  -= 1;
    ny[9] += 1; nz[9]  -= 1;
    nz[10]-= 1; nx[10] -= 1;
    nz[11]+= 1; nx[11] -= 1;

    nx[12] -= 1; ny[12] += 1;
    nx[13] += 1; ny[13] += 1;
    ny[14] -= 1; nz[14] += 1;
    ny[15] += 1; nz[15] += 1;
    nz[16] -= 1; nx[16] += 1;
    nz[17] += 1; nx[17] += 1;

    for(uint i = 0, j = 0; i < 18; i++) {
        if (nx[i] >= data.size[0]) continue;
        if (ny[i] >= data.size[1]) continue;
        if (nz[i] >= data.size[2]) continue;

        nbrs[j++] = data.convertIndex(nx[i],ny[i],nz[i]);
    }
}
