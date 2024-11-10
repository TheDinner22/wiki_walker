#pragma once

#include <vector>
#include <unordered_map>

typedef std::unordered_map<int, std::vector<int>> AdjacencyList;

class Graph {
   private:
       std::unordered_map<int, AdjacencyList> graph;

    public:
      void insertEdge(int from, int to, int weight);  
      bool isEdge(int from, int to);  
      int sumEdge(int from, int to); 
      std::vector<int> getWeight(int from, int to); 
      std::vector<int> getAdjacent(int vertex); 
};

