#include <dependencies/httplib.h>
#include <curl/curl.h>
#include <iostream>

#include "./wiki_api/wiki_api.hpp"
#include "./pages/pages.hpp"

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
        std::string page_html = landing_page();
        //res.set_content(page_html, "text/html");
        res.set_redirect("/public/html/index.html");
    });

    svr.Get("/api/perform_search", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "hi" << std::endl;
        for(auto param: req.params){
            std::cout << param.first << ":" << param.second << std::endl;
        }
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
