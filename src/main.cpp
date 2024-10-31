#include <dependencies/httplib.h>
#include <curl/curl.h>
#include <iostream>

#include "./wiki_api/wiki_api.hpp"
#include "./pages/pages.hpp"
#include "./wiki_search/wiki_search.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
int main(void){
    // HTTP
    httplib::Server svr;

    bool worked = svr.set_mount_point("/public", "./public");
    if(!worked){
        std::cout << "yeah so we are FUCKED" << std::endl;
    }

    // landing page
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        std::string page_html = pages::landing_page();
        //res.set_content(page_html, "text/html");
        res.set_redirect("/public/html/index.html");
    });

    // someone is typing on an input
    svr.Get("/api/search_hint", [](const httplib::Request &req, httplib::Response &res) {
        pages::search_hint(req, res);
    });

    // called when the button is pressed and both inputs are sent in as params
    svr.Get("/api/perform_search", [](const httplib::Request &req, httplib::Response &res) {
        pages::search_for_5_pages(req, res);

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
        if(!p1_exists && !p2_exists){
            res.status = httplib::BadRequest_400;
            res.set_content("buddy never send me a raw HTTP reqeest again! Use the web interface", "text/plain");
        }

        // otherwise, perform search and then
        res.set_content("", "text/plain");
    });

    svr.listen("127.0.0.1", 8000);
}

void chaining_example(){
    curl_global_init(CURL_GLOBAL_DEFAULT);

    auto v1 = get_links("Theodore Roosevelt");
    std::cout << v1[10] << std::endl;
    auto v2 = get_links(v1[10]);
    std::cout << v2[10] << std::endl;
    auto v3 = get_links(v2[10]);
    std::cout << v3[10] << std::endl;

    curl_global_cleanup();
}
