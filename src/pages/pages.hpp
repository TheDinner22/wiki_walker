#pragma once

#include <string>
#include "../dependencies/httplib.h"

namespace pages {
    // just grab some html
    std::string landing_page();

    // validate and parse req
    // send get request
    // format response as html
    // setup response
    void search_hint(const httplib::Request& req, httplib::Response &res);

    void perform_search(const httplib::Request& req, httplib::Response &res, const std::string& f_name);

    void search_for_5_pages(const httplib::Request& req, httplib::Response &res);
}

