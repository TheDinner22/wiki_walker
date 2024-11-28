#pragma once

#include <string>
#include <unordered_map>
#include "../dependencies/httplib.h"
#include "the_graph/the_graph.hpp"

class ReqParams{
public:
    bool start_page_exists;
    bool end_page_exists;
    bool graph_name_exists;
    std::basic_string<char> start_page;
    std::basic_string<char> end_page;
    std::basic_string<char> graph_name;

    ReqParams(const httplib::Request& req);
};

namespace pages {
    // validate and parse req
    // send get request
    // format response as html
    // setup response
    void search_hint(const httplib::Request& req, httplib::Response &res);

    void perform_search(const httplib::Request& req, httplib::Response &res, const std::string& f_name, const Graph& g);

    void search_for_5_pages(const httplib::Request& req, httplib::Response &res);

    bool create_graph(const httplib::Request& req, Graph& g);

    bool cache_search(const httplib::Request& req, const std::unordered_map<std::string, Graph> graphs);
}

