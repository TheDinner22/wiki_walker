#include "search_algorithms.hpp"
#include "the_graph/the_graph.hpp"

ParseResults hubert_algo(const std::string& start, std::string& end, const Graph& g){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "hubert button";
    r.shortest_path = {"a", "b", "c"};

    return r;
}
