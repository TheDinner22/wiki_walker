#include "search_algorithms.hpp"
#include <cstdlib>
#include <vector>

// the get_links function 
// returns all page names pointed to by a page name
ParseResults keep_picking_random(const std::string& start, std::string& end){
    RealWebSearcher searcher;

    std::basic_string_view<char> next = start;
    std::vector<std::basic_string_view<char>> the_path;
    for(int i = 0; i< 10; i++){
        the_path.push_back(next);
        auto links = searcher.get_links(next);
        if(links.size() == 0){ break; }
        next = links[rand() % links.size()];
    }

    ParseResults r;
    r.num_requests_sent = 10;
    r.pages_visited = 10;
    r.algo_name = "example algo that picks random links";
    r.shortest_path = the_path;

    return r;
}
