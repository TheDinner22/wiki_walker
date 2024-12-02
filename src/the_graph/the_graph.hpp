#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>

// SIMPLE DIRECTED UNWEIGHTED GRAPH! nodes cannot point to themselves and there are no parallel edges
class Graph {
private:
   std::unordered_map<std::string, std::unordered_set<std::string>> graph;

public:
    void reset(){ graph.clear(); }

    std::string as_json(){
        /*
        {
            "endpoint": "function name",
            "nodes": [a list of all node names],
            "edges": [ [], [], []] // each sub list contains to, from pairs that define edges
        }
         **/
        std::string json_str = "{\"endpoint\": \"make_graph\",\"nodes\":[";
        std::unordered_map<std::string, std::unordered_set<std::string>> m;

        // add node edges
        for(auto s: graph){
            json_str += '"' + s.first + '"';
            json_str += ',';
        }
        json_str.pop_back();
        json_str += "],\"edges\":[";

        // add edges
        for(auto key_val: graph){
            for(auto to: key_val.second){
                bool from_to_exists = m[key_val.first].count(to) == 1;
                bool to_from_exists = m[to].count(key_val.first) == 1;
                if(from_to_exists || to_from_exists){ continue; }

                json_str += "[\"" + key_val.first + "\",\"" + to + "\"],";
                m[key_val.first].insert(to);
                m[to].insert(key_val.first);
            }
        }

        json_str.pop_back();
        json_str += "]}";

        std::cout << "as str done" << std::endl;
        return json_str;
    }

    void print() const {
        for(auto node: graph){
            std::cout << "-------------------------" << std::endl;
            std::cout << node.first << std::endl;
            for(auto next: node.second){
                std::cout << next << " ";
            }
            std::cout << std::endl;
        }
        std::cout << num_nodes() << std::endl;
    }

    int num_edges() const {
        int count = 0;
        for(auto node: this->graph){
            count += node.second.size();
        }
        return count;
    }
    
    // insert an edge (and the nodes if they don't already exist)
    void insertEdge(std::string from, std::string to);

    // check if there is an edge between two nodes
    bool isEdge(std::string from, std::string to) const ;

    // int sumEdge(int from, int to); // not weighted so we don't need this
    // std::vector<int> getWeight(int from, int to); // not weighted so we don't need this
    
    // get all the nodes that are adjacent to a node
    std::vector<std::string> getAdjacent(std::string vertex) const ; 

    // out degree of a given node
    uint out_degree(const std::string& vertex) const ;

    uint num_nodes() const ;

    const std::unordered_map<std::string, std::unordered_set<std::string>>& get_graph() const;

};

