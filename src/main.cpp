//#include <dependencies/httplib.h>

#include <curl/curl.h>
#include "./wiki_api/wiki_api.hpp"
#include <iostream>

int main(void)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    auto v1 = get_links("Theodore Roosevelt");
    std::cout << v1[10] << std::endl;
    auto v2 = get_links(v1[10]);
    std::cout << v2[10] << std::endl;
    auto v3 = get_links(v2[10]);
    std::cout << v3[10] << std::endl;


    curl_global_cleanup();

    return 0;
}
