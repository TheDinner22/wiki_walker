#include "the_graph.hpp"

#include <vector>
#include <unordered_map>

void Graph::insertEdge(std::string from, std::string to) {
    if(from == to){return;}

    // ensure from and to exists
    if(graph.count(from) == 0){ graph[from] = {}; }
    if(graph.count(to) == 0){ graph[to] = {}; }

    // insert edge
    graph[from].insert(to);
}
        
bool Graph::isEdge(std::string from, std::string to) 
{
    return graph[from].count(to);
}

std::vector<std::string> Graph::getAdjacent(std::string vertex) 
{
    std::vector<std::string> nodes;
    nodes.reserve(graph[vertex].size());
    for(auto s : graph[vertex]){
        nodes.push_back(s);
    }
    return nodes;
}

uint Graph::out_degree(const std::string& vertex){
    if (graph.count(vertex) == 0){ return 0; }

    return graph[vertex].size();
}

uint Graph::num_nodes(){
    return graph.size();
}
