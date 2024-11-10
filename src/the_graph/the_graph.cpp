#include "the_graph.hpp"

// I worked on this quiz with my buddy, Sammy Li

#include <algorithm>
#include <vector>
#include <unordered_map>

void Graph::insertEdge(int from, int to, int weight) 
{
    // ensure from and to exists
    if(graph.count(from) == 0){ graph[from]; }
    if(graph.count(to) == 0){ graph[to]; }

    // insert edge
    graph[from][to].push_back(weight);
}
        
bool Graph::isEdge(int from, int to) 
{
    return graph[from].count(to);
}

int Graph::sumEdge(int from, int to)
{
    int sum = 0;

    if(graph[from].count(to) == 1){
        for(auto wgt : graph[from][to]){
            sum += wgt;
        }
    }

    // TODO add other?

    return sum;
}

std::vector<int> Graph::getWeight(int from, int to)
{
    if(graph[from].count(to) == 0){return {};}
    std::vector<int> weights;
    for(int wgt: graph[from][to]){
        weights.push_back(wgt);
    }

    std::sort(weights.begin(), weights.end());
    return weights;

}

std::vector<int> Graph::getAdjacent(int vertex) 
{
    std::vector<int> vs;
    for(auto edge : graph[vertex]){
        for(int i = 0; i < int(edge.second.size()); i++){
            vs.push_back(edge.first);
        }
    }
    std::sort(vs.begin(), vs.end());
    return vs;
}
