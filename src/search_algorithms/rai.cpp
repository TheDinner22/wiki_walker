#include "search_algorithms.hpp"
#include "../the_graph/the_graph.hpp"

ParseResults rai_algo(const std::string& start, std::string& end, const Graph& graph){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "rai button";
    r.shortest_path = {"a", "b", "c"};

    return r;
}
