#include "search_algorithms.hpp"
#include "../the_graph/the_graph.hpp"

ParseResults rai_algo_dfs(const std::string& start, const std::string& end, const Graph& graph){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "rai DFS button";
    r.shortest_path = {"a", "b", "c"};

    std::cout << "start: " << start << std::endl;
    std::cout << "end: " << end << std::endl;

    return r;
}

ParseResults rai_algo_bfs(const std::string& start, const std::string& end, const Graph& graph){
    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "rai BFS button";
    r.shortest_path = {"a", "b", "c"};

    return r;
}
