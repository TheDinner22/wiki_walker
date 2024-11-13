#include "pages.hpp"
#include "../wiki_search/wiki_search.hpp"
#include "dependencies/httplib.h"
#include "search_algorithms/search_algorithms.hpp"
#include "the_graph/the_graph.hpp"
#include <queue>

std::string pages::landing_page(){
    return "hello world";
}

void pages::search_hint(const httplib::Request& req, httplib::Response &res){
    // all this code and I'm just parsing an input
    std::basic_string<char> input_value;
    bool input_exists = false;
    for(auto param: req.params){
        if(param.first == "input"){
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
        final_html += "<li onclick=\"alert('hi')\">";
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
};
ThisIsBadCode get_start_page(const httplib::Request& req, httplib::Response &res);
void pages::perform_search(const httplib::Request& req, httplib::Response &res, const std::string& f_name){
    // get start and end page
    auto results = get_start_page(req, res);
    if(results.valid == false){ return; }

    // perform search
    ParseResults algo_results;
    if(f_name.size() == 0){
        algo_results = keep_picking_random(results.start_page);
    }
    else if(f_name == "r"){
        Graph g;
        algo_results = rai_algo(results.start_page, g);
    }

    else if(f_name == "h"){
        Graph g;
        algo_results = hubert_algo(results.start_page, g);
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

void pages::create_graph(const httplib::Request& req, httplib::Response &res){
    // parse shit from request
    auto parsed_req = get_start_page(req, res);
    std::string start_page_name = parsed_req.start_page;
    if (parsed_req.valid == false){ return; }

    // TODO what todo if the thing is empty (nice error msg)

    const int NUMBER_OF_NODES = 1000;
    Graph g;
    std::queue<std::string> q;

    while(g.num_nodes() < NUMBER_OF_NODES){

    }

    res.set_content("hi i got here", "text/plain");
}
