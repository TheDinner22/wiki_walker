#include "wiki_search.hpp"
#include "../requests/requests.hpp" // get request
#include "../dependencies/simdjson.h" //json
#include <curl/curl.h>

using namespace simdjson;

std::vector<std::basic_string_view<char>>search_wiki(const std::basic_string_view<char>& search_term, size_t max_terms){
    CURL* curl = curl_easy_init();
    std::vector<std::basic_string_view<char>> parsed_links;
    parsed_links.reserve(5);

    // URL encode the page name
    // TODO alloc maybe?
    std::string encoded_page_name = curl_easy_escape(curl, search_term.data(), search_term.size());

    std::string url = "https://en.wikipedia.org/w/api.php?action=query&format=json&list=search&srsearch=" + encoded_page_name;
    auto raw_json = send_get_request(url.c_str());

    // parse json
    padded_string raw_padded_json(raw_json);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document result = parser.iterate(raw_padded_json);

    auto search_results = result["query"]["search"].get_array();
    if(search_results.error() != SUCCESS){
        std::cout << search_results.error() << std::endl;
        return parsed_links;
    }

    for(auto result : search_results){
        auto raw_title = result["title"].get_string();
        int _out_length;
        auto title = curl_easy_unescape(curl, raw_title.value().data(), raw_title.value().size(), &_out_length);
        parsed_links.push_back(title);
        if(parsed_links.size() == max_terms){ break; }
    }

    curl_easy_cleanup(curl);
    return parsed_links;
}

