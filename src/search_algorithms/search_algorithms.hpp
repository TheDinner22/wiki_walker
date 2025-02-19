#pragma once

#include <string>
#include <vector>
#include "the_graph/the_graph.hpp"
#include "wiki_api/wiki_api.hpp"

class Searcher{
public:
    virtual std::vector<std::basic_string_view<char>> get_links(const std::basic_string_view<char>& start) = 0;
    virtual ~Searcher() = default;
};

class RealWebSearcher : public Searcher {
public:
    std::vector<std::basic_string_view<char>> get_links(const std::basic_string_view<char>& start){
        return ::get_links(start);
    }
};

// just make functions that return this
struct ParseResults {
    int num_requests_sent = 0;
    int pages_visited = 0;
    std::string algo_name;
    std::vector<std::string> shortest_path;

    /*
    <div class="card">
        <strong>Sample Input 1</strong>
        <p>Sample Input 2</p>
        <p>Additional information can be displayed here to provide more context.</p>
    </div>
    */
    std::string as_html_card(){
        std::string msg;
        msg += "<div class=\"card\"><strong>";
        msg += algo_name;
        msg += "</strong>";
        if(this->shortest_path.size() == 0){
            msg += "<p>One of the nodes you entered isn't in the graph! Double check your start and end input boxes and try again.</p>";
        }
        msg = msg + "<p> Requests sent:" + std::to_string(num_requests_sent) +"</p>";
        msg = msg + "<p> Pages visited:" + std::to_string(pages_visited) +"</p>";
        msg += "<ul>";
        for(auto page_name: shortest_path){
            msg += "<li>";
            msg += page_name;
            msg += "</li>";
        }
        msg += "</ul></div>";
        msg += "<script type=\"application/json\" class=\"my-script\" id=\""+ this->algo_name + "\">[";
        
        for(auto node_name : this->shortest_path){
            std::string temp = node_name;

            while(temp.find('"') != std::string::npos){
                temp.replace(temp.find('"'), 1, 1, '\'');
            }
            msg += '"';
            msg += temp;
            msg += "\",";
        }
        msg.pop_back();
        msg += "]</script>";

        return msg;
    }
};

// here's an example that traverses the 20th link (assuming it exists) ten times
// this IS NOT USED IN PROD AND IS JUST AN EXAMPLE
ParseResults keep_picking_random(const std::string& start, const std::string& end, const Graph& g);

ParseResults hubert_algo_dijkstra(const std::string& start, const std::string& end, const Graph& g);

ParseResults rai_algo_bfs(const std::string& start, const std::string& end, const Graph& graph);
ParseResults rai_algo_dfs(const std::string& start, const std::string& end, const Graph& graph);

