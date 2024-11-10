#include "search_algorithms.hpp"

ParseResults hubert_algo(const std::string& start, std::string& end){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "hubert button";
    r.shortest_path = {"a", "b", "c"};

    return r;
}
