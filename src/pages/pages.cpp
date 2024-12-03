#include "pages.hpp"
#include "../wiki_search/wiki_search.hpp"
#include "dependencies/httplib.h"
#include "search_algorithms/search_algorithms.hpp"
#include "the_graph/the_graph.hpp"
#include "wiki_api/wiki_api.hpp"
#include <queue>
#include <string>
#include <unordered_set>

// take in some request and then get the expected query and or body
// we expect both input boxes to be included (even if they are empty)
ReqParams::ReqParams(const httplib::Request& req){
    // get start and end page
    std::basic_string<char> start_page;
    std::basic_string<char> end_page;
    std::basic_string<char> graph_name;
    bool start_page_exists = false;
    bool end_page_exists = false;
    bool graph_name_exists = false;
    for(auto param: req.params){
        if(param.first == "input2"){
            graph_name = param.second;
            graph_name_exists = true;
        }
        if(param.first == "input3"){
            start_page = param.second;
            start_page_exists = true;
        }
        if(param.first == "input4"){
            end_page = param.second;
            end_page_exists = true;
        }
    }

    this->start_page = start_page;
    this->end_page = end_page;
    this->graph_name = graph_name;
    this->start_page_exists = start_page_exists;
    this->end_page_exists = end_page_exists;
    this->graph_name_exists = graph_name_exists;
}


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

void pages::perform_search(const httplib::Request& req, httplib::Response &res, const std::string& f_name, std::unordered_map<std::string, Graph>& graphs){
    // get start and end page
    ReqParams results(req);
    if(!results.graph_name_exists || !results.start_page_exists || !results.end_page_exists){ res.set_content("invalid request", "text/plain"); res.status = 400; return; }

    // make sure that the graph actually exists
    if(graphs.count(results.graph_name) == 0){ res.set_content("invalid request, graph miss", "text/plain"); res.status = 400; return;}

    const Graph& g = graphs[results.graph_name];

    // perform search
    ParseResults algo_results;
    if(f_name == "bfs"){
        algo_results = rai_algo_bfs(results.start_page, results.end_page, g);
    }

    else if(f_name == "dfs"){
        algo_results = rai_algo_dfs(results.start_page, results.end_page, g);
    }
    else if(f_name == "d"){
        algo_results = hubert_algo_dijkstra(results.start_page, results.end_page, g);
    }
    else {
        std::cout << "INVALID" << std::endl;
    }

    // send html down
    res.set_content(algo_results.as_html_card(), "text/html");
}

bool pages::create_graph(const httplib::Request& req, std::unordered_map<std::string, Graph>& graphs){
    // parse shit from request
    ReqParams result(req);
    // TODO what todo if the thing is empty (nice error msg)
    if(!result.graph_name_exists || !result.start_page_exists || !result.end_page_exists){ return false; }

    // if graph already in cache, do nothing
    if(graphs.count(result.graph_name) == 1){return true;}

    // create the graph with bfs
    std::basic_string_view<char> start_page_name = result.graph_name;
    const int NUMBER_OF_NODES = 1000;
    Graph g;
    std::queue<std::basic_string_view<char>> q;
    std::unordered_set<std::basic_string_view<char>> viewed_pages;
    q.push(start_page_name);
    viewed_pages.insert(start_page_name);

    while(g.num_nodes() < NUMBER_OF_NODES && q.size() != 0){
        std::cout << g.num_nodes() << std::endl;
        auto current_node = q.front();
        q.pop();

        auto links = get_links(current_node);

        // add edges between current_node and links
        // also keep track of the links that we have already visited
        // also enque new links
        int i = 0;
        for(auto link : links){
            g.insertEdge(current_node.data(), link.data()); // disgusting copy slowing my code down
            if(viewed_pages.count(link) == 0){
                q.push(link);
                viewed_pages.insert(link);
                if(i++ > 15){break;}
            }
        }
    }

    // add the graph to the cache, make sure the cahce isn't too big
    const int MAX_CACHE_SIZE = 20;
    if(graphs.size() >= MAX_CACHE_SIZE){
        auto dropped_name = graphs.begin()->first;
        std::cout << "dropped:" << dropped_name << std::endl;
        graphs.erase(dropped_name);
    }
    graphs[result.graph_name] = g;

    std::cout << "handled" << std::endl;
    return true;
}

