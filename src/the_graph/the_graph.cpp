#include "the_graph.hpp"

#include <stdexcept>
#include <vector>
#include <unordered_map>

void Graph::insertEdge(std::string from, std::string to) {
    if(from == to){return;}

    // insert edge
    // replace all '"' with \" or smt idk
    while(from.find('"') != std::string::npos){
        from.replace(from.find('"'), 1, 1, '\'');
    }

    while(to.find('"') != std::string::npos){
        to.replace(to.find('"'), 1, 1, '\'');
    }

    // ensure from and to exists
    if(graph.count(from) == 0){ graph[from] = {}; }
    if(graph.count(to) == 0){ graph[to] = {}; }

    graph[from].insert(to);
}
        
bool Graph::isEdge(std::string from, std::string to) const
{
    try {
        return graph.at(from).count(to);
    } catch (std::out_of_range) {
        return false;
    }
}

std::vector<std::string> Graph::getAdjacent(std::string vertex) const 
{
    try {
        std::vector<std::string> nodes;
        nodes.reserve(graph.at(vertex).size());
        for(auto s : graph.at(vertex)){
            nodes.push_back(s);
        }
        return nodes;
    } catch (std::out_of_range) {
        return {};
    }

}

uint Graph::out_degree(const std::string& vertex) const{
    if (graph.count(vertex) == 0){ return 0; }

    try {
        return graph.at(vertex).size();
    } catch (std::out_of_range) {
        return 0;
    }
}

uint Graph::num_nodes() const{
    return graph.size();
}
