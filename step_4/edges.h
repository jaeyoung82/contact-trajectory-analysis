//
// edge.h
//
#ifndef edge_h
#define edge_h
//
#include <cstdio>
#include <algorithm>
//
class Edge{
    private:
        //
        int id_i;
        int id_j;
        int frame_start;
        int frame_end;
        int n_repeat;

    public:
        //
        Edge();
        ~Edge();

};
//
#endif
