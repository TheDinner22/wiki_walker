#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>


// SIMPLE DIRECTED UNWEIGHTED GRAPH! nodes cannot point to themselves and there are no parallel edges
class Graph {
private:
   std::unordered_map<std::string, std::unordered_set<std::string>> graph;

public:
    
    // insert an edge (and the nodes if they don't already exist)
    void insertEdge(std::string from, std::string to);

    // check if there is an edge between two nodes
    bool isEdge(std::string from, std::string to);

    // int sumEdge(int from, int to); // not weighted so we don't need this
    // std::vector<int> getWeight(int from, int to); // not weighted so we don't need this
    
    // get all the nodes that are adjacent to a node
    std::vector<std::string> getAdjacent(std::string vertex); 

    // out degree of a given node
    uint out_degree(const std::string& vertex);
};

