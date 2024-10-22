
//#include <dependencies/httplib.h>

#include "./json/ez_json.hpp"
#include <curl/curl.h>
#include "requests/requests.hpp"

int main(void)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    const char* msg = "https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Albert_Einstein&prop=links&pllimit=max";
    std::string response = send_get_request(msg);
    parse_wiki_api_json(response);

    curl_global_cleanup();

    return 0;
}
