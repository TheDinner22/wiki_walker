#include <dependencies/httplib.h>
#include <curl/curl.h>
#include <iostream>

#include "./wiki_api/wiki_api.hpp"
#include "./pages/pages.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
int main(void){
    // HTTP
    httplib::Server svr;

    // landing page
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        std::string page_html = landing_page();
        res.set_content(page_html, "text/html");
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
