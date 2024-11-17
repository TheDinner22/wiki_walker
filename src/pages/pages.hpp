#pragma once

#include <string>
#include "../dependencies/httplib.h"
#include "the_graph/the_graph.hpp"

namespace pages {
    // validate and parse req
    // send get request
    // format response as html
    // setup response
    void search_hint(const httplib::Request& req, httplib::Response &res);

    void perform_search(const httplib::Request& req, httplib::Response &res, const std::string& f_name, const Graph& g);

    void search_for_5_pages(const httplib::Request& req, httplib::Response &res);

    void create_graph(const httplib::Request& req, httplib::Response &res, Graph& g);
}

