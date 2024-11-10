#include <dependencies/httplib.h>
#include <curl/curl.h>
#include <iostream>

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
        pages::perform_search(req, res, "");
    });

    // rai button
    svr.Get("/api/perform_r_search", [](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "r");
    });

    // hubert button
    svr.Get("/api/perform_h_search", [](const httplib::Request &req, httplib::Response &res) {
        pages::perform_search(req, res, "h");
    });

    svr.listen("0.0.0.0", 8000);
}

