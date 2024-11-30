#include "search_algorithms.hpp"
#include "the_graph/the_graph.hpp"

ParseResults hubert_algo_dijkstra(const std::string& start, const std::string& end, const Graph& g){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "hubert button dijkstra";
    r.shortest_path = {"a", "b", "c"};

    return r;
}

ParseResults hubert_algo_a_star(const std::string& start, const std::string& end, const Graph& g){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "hubert button a star";
    r.shortest_path = {"a", "b", "c"};

    return r;
}
