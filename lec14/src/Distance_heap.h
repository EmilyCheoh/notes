#pragma once

#include "WU_graph.h"

namespace ipd
{

struct known_distance
{
    WU_graph::vertex v;
    WU_graph::weight w;
};

class Distance_heap
{
public:
    void insert(known_distance);
    known_distance remove_min();
    known_distance peek_min() const;

    bool empty() const;
    size_t size() const;

private:
    std::vector<known_distance> heap_;
};

}
