#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    // TODO store the indegree for each vertex in the graph in the indegrees map
    for (const auto& a : graph) {
        indegrees[a.first] = 0;
    }

    for (const auto& a : graph) {
        const auto& b = a.second;
        for (const auto& c : b) {
            indegrees[c.first] +=1 ;
        }
    }
}
