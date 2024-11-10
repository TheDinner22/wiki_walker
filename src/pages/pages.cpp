#include "pages.hpp"
#include "../wiki_search/wiki_search.hpp"
#include "dependencies/httplib.h"
#include "search_algorithms/search_algorithms.hpp"

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
    std::basic_string<char> start_page;
    std::basic_string<char> end_page;
};
ThisIsBadCode get_start_and_end_page(const httplib::Request& req, httplib::Response &res);
void pages::perform_search(const httplib::Request& req, httplib::Response &res, const std::string& f_name){
    // get start and end page
    auto results = get_start_and_end_page(req, res);
    if(results.valid == false){ return; }

    // perform search
    ParseResults algo_results;
    if(f_name.size() == 0){
        algo_results = keep_picking_random(results.start_page, results.end_page);
    }
    else if(f_name == "r"){
        algo_results = rai_algo(results.start_page, results.end_page);
    }

    else if(f_name == "h"){
        algo_results = hubert_algo(results.start_page, results.end_page);
    }
    else {
        std::cout << "INVALID" << std::endl;
    }

    // send html down
    res.set_content(algo_results.as_html_card(), "text/html");
}

ThisIsBadCode get_start_and_end_page(const httplib::Request& req, httplib::Response &res){
    // get start and end page
    std::basic_string<char> start_page;
    std::basic_string<char> end_page;
    bool p1_exists = false;
    bool p2_exists = false;
    for(auto param: req.params){
        if(param.first == "input1"){
            start_page = param.second;
            p1_exists = true;
        }

        else if(param.first == "input2"){
            end_page = param.second;
            p2_exists = true;
        }

        if(p1_exists && p2_exists){break;}
    }

    // bad Request
    if(!p1_exists || !p2_exists){
        res.status = httplib::BadRequest_400;
        res.set_content("buddy never send me a raw HTTP reqeest again! Use the web interface", "text/plain");
        ThisIsBadCode r;
        r.valid = false;
        return r;
    }
    ThisIsBadCode results;
    results.valid = p1_exists && p2_exists;
    results.start_page = start_page;
    results.end_page = end_page;

    if(results.valid == false){
        std::cout << "was false" << std::endl;
    }
    
    return results;
}
