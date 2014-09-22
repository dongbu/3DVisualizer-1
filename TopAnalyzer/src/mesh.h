#ifndef MESH_H
#define MESH_H

#include "global.h"
#include "data.h"

class Mesh 
{
public:
    Data& data;
    Mesh(Data & d) : data(d) {}

    void getNeighbors(size_t i, std::vector<size_t>& n);
    size_t getNeighbors(size_t idx, size_t* nbrs, size_t nbrs_size);
    void createGraph(std::vector<size_t>& order);
    uint numVerts();

    void find6Neighbors( uint x, uint y, uint z, std::vector<size_t>& neighbors);
    void find6Neighbors( uint x, uint y, uint z, size_t* nbrs);
    void find18Neighbors( uint x, uint y, uint z, std::vector<size_t>& neighbors);
    void find18Neighbors( uint x, uint y, uint z, size_t* nbrs);
};

#endif
