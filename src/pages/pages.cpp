#include "pages.hpp"
#include "../wiki_search/wiki_search.hpp"
#include "dependencies/httplib.h"
#include "search_algorithms/search_algorithms.hpp"
#include "the_graph/the_graph.hpp"
#include "wiki_api/wiki_api.hpp"
#include <queue>
#include <string>
#include <unordered_set>

void pages::search_hint(const httplib::Request& req, httplib::Response &res){
    // all this code and I'm just parsing an input
    std::basic_string<char> input_value;
    std::basic_string<char> input_key;
    bool input_exists = false;

    // the web is stringly typed and it's a fucking travesty
    for(auto param: req.params){
        if(param.first == "input2" || param.first == "input3" || param.first == "input4"){
            input_key = param.first;
            input_value = param.second;
            input_exists = true;
            break;
        }
    }

    // early return for invalid requests
    if(!input_exists){
        res.status = httplib::BadRequest_400;
        res.set_content("buddy never send me a raw HTTP reqeest again! Use the web interface", "text/plain");
        return;
    }

    // actually interesting stuff after this line
    
    // search wiki for near hits
    auto results = search_wiki(input_value);
    
    // if no results, sorry message
    if(results.size() == 0 || 1){
        res.set_content("<ul id=\"search-results\"><li>No Results</li></ul>", "text/html");
    }
    
    // if results, format them and return
    std::string final_html = "<ul id=\"search-results\">";
    for (auto result: results){
        final_html += "<li onclick=\"li_got_clicked(event, '" + input_key + "')\">";
        final_html += result;
        final_html += "</li>";
    }
    final_html += "</ul>";
    res.set_content(final_html, "text/html");
}

struct ThisIsBadCode{
    bool valid;
    // TODO BIG CHANGE HERE
    std::basic_string<char> start_page;
    std::basic_string<char> end_page;
};
ThisIsBadCode get_start_page(const httplib::Request& req, httplib::Response &res);
ThisIsBadCode get_start_end(const httplib::Request& req, httplib::Response &res);
void pages::perform_search(const httplib::Request& req, httplib::Response &res, const std::string& f_name, const Graph& g){
    // get start and end page
    auto results = get_start_end(req, res);
    if(results.valid == false){ return; }

    // perform search
    ParseResults algo_results;
    if(f_name.size() == 0){
        algo_results = keep_picking_random(results.start_page);
    }
    else if(f_name == "bfs"){
        algo_results = rai_algo_bfs(results.start_page, results.end_page, g);
    }

    else if(f_name == "dfs"){
        algo_results = rai_algo_dfs(results.start_page, results.end_page, g);
    }
    else if(f_name == "d"){
        algo_results = hubert_algo_dijkstra(results.start_page, results.end_page, g);
    }
    else if(f_name == "a"){
        algo_results = hubert_algo_a_star(results.start_page, results.end_page, g);
    }
    else {
        std::cout << "INVALID" << std::endl;
    }

    // send html down
    res.set_content(algo_results.as_html_card(), "text/html");
}

ThisIsBadCode get_start_page(const httplib::Request& req, httplib::Response &res){
    // get start and end page
    std::basic_string<char> start_page;
    bool p2_exists = false;
    for(auto param: req.params){
        if(param.first == "input2"){
            start_page = param.second;
            p2_exists = true;
            break;
        }
    }

    // bad Request
    if(!p2_exists){
        res.status = httplib::BadRequest_400;
        res.set_content("buddy never send me a raw HTTP reqeest again! Use the web interface", "text/plain");
        ThisIsBadCode r;
        r.valid = false;
        return r;
    }
    ThisIsBadCode results;
    results.valid = p2_exists;
    results.start_page = start_page;

    if(results.valid == false){
        std::cout << "was false" << std::endl;
    }
    
    return results;
}

ThisIsBadCode get_start_end(const httplib::Request& req, httplib::Response &res){
    // get start and end page
    std::basic_string<char> start_page;
    std::basic_string<char> end_page;
    bool p1_exists = false;
    bool p2_exists = false;
    for(auto param: req.params){
        if(param.first == "input3"){
            start_page = param.second;
            p1_exists = true;
        }
        if(param.first == "input4"){
            end_page = param.second;
            p2_exists = true;
        }
    }

    // bad Request
    if(!p2_exists || !p1_exists){
        res.status = httplib::BadRequest_400;
        res.set_content("buddy never send me a raw HTTP reqeest again! Use the web interface", "text/plain");
        ThisIsBadCode r;
        r.valid = false;
        return r;
    }
    ThisIsBadCode results;
    results.valid = p2_exists && p1_exists;
    results.start_page = start_page;
    results.end_page = end_page;

    if(results.valid == false){
        std::cout << "was false" << std::endl;
    }
    
    return results;
}

void pages::create_graph(const httplib::Request& req, httplib::Response &res, Graph& g){
    g.reset();

    // parse shit from request
    auto parsed_req = get_start_page(req, res);
    std::basic_string_view<char> start_page_name = parsed_req.start_page;
    if (parsed_req.valid == false){ return; }

    // TODO what todo if the thing is empty (nice error msg)

    const int NUMBER_OF_NODES = 1000;
    std::queue<std::basic_string_view<char>> q;
    std::unordered_set<std::basic_string_view<char>> viewed_pages;
    q.push(start_page_name);
    viewed_pages.insert(start_page_name);

    while(g.num_nodes() < NUMBER_OF_NODES && q.size() != 0){
        auto current_node = q.front();
        q.pop();

        auto links = get_links(current_node);

        // add edges between current_node and links
        // also keep track of the links that we have already visited
        // also enque new links
        for(auto link : links){
            if(viewed_pages.count(link) == 0){
                g.insertEdge(current_node.data(), link.data()); // disgusting copy slowing my code down
                viewed_pages.insert(link);
                q.push(link);
            }
        }
    }

    std::string msg = "<p>Traversed <strong>" + std::to_string(g.num_nodes()) + "</strong> nodes!</p>";
    res.set_content(msg, "text/plain");
}
