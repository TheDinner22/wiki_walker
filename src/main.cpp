//#include <dependencies/httplib.h>

#include <curl/curl.h>
#include "./wiki_api/wiki_api.hpp"
#include <iostream>

int main(void)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    auto v = get_links("Albert_Einstein");

    for (auto link: v){
        std::cout << link << std::endl;
    }
    std::cout << v.size() << std::endl;

    curl_global_cleanup();

    return 0;
}
