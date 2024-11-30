#include "search_algorithms.hpp"
#include "the_graph/the_graph.hpp"
#include <cstdlib>
#include <vector>

// the get_links function 
// returns all page names pointed to by a page name
ParseResults keep_picking_random(const std::string& start, const std::string& end, const Graph& g){
    std::vector<std::string> path;
    path.push_back(start);
    int req_sent = 0;
    int pv = 0;

    auto adj = g.getAdjacent(start);
    pv+= adj.size();
    req_sent++;

    // TODO this has loops but that dudnt matter
    while(path.size() < 10 && adj.size() >0){
        std::string next = adj[rand() % adj.size()];
        path.push_back(next);
        adj = g.getAdjacent(next);
        pv+= adj.size();
        req_sent++;
    }

    ParseResults r;
    r.num_requests_sent = req_sent;
    r.pages_visited = pv;
    r.algo_name = "example algo that picks random links";
    r.shortest_path = path;

    return r;
}
