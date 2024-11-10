#include "./wiki_api.hpp"
#include "../requests/requests.hpp" // get request
#include "../dependencies/simdjson.h" //json

#include <string_view>
#include <vector>

#include <curl/curl.h>

using namespace simdjson;

std::vector<std::basic_string_view<char>>get_links(const std::basic_string_view<char>& page_name){
    CURL* curl = curl_easy_init();
    std::vector<std::basic_string_view<char>> parsed_links;

    // URL encode the page name
    // TODO alloc maybe?
    std::string encoded_page_name = curl_easy_escape(curl, page_name.data(), page_name.size());

    // send get request 
    std::string url = "https://en.wikipedia.org/w/api.php?action=query&format=json&titles=" + encoded_page_name + "&prop=links&pllimit=max";
    auto raw_json = send_get_request(url.c_str());

    // parse json
    padded_string raw_padded_json(raw_json);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document result = parser.iterate(raw_padded_json);

    auto pages = result["query"]["pages"].get_object();
    if(pages.error() != SUCCESS){
        std::cout << pages.error() << std::endl;
        return parsed_links;
    }

    for (auto item: pages){
        auto value = item.value();
        auto links = value["links"];
        if(links.error() != SUCCESS){
            std::cout << "there were no links!" << std::endl;
            continue;
        }
        for (auto link : links){
            auto raw_name = link["title"].get_string().take_value();
            // url decode the thing
            int _output_length; // ignore this
            char* decoded_name = curl_easy_unescape(curl, raw_name.data(), raw_name.size(), &_output_length);
            parsed_links.push_back(decoded_name);
        }
    }

    // keep looping as long as the response implies that there
    // is more data
    while(result["continue"].error() == SUCCESS){
        auto plcontinue = result["continue"]["plcontinue"];
        auto raw_plcontinue = plcontinue.get_string().take_value();
        const char* url_encoded_plcontinue = curl_easy_escape(curl, raw_plcontinue.data(), raw_plcontinue.size());

        std::string next_url = url;
        next_url += "&plcontinue=";
        next_url += url_encoded_plcontinue;

        raw_json = send_get_request(next_url.c_str());
        raw_padded_json = padded_string(raw_json);
        result = parser.iterate(raw_padded_json);
        pages = result["query"]["pages"].get_object();
        if(pages.error() != SUCCESS){
            std::cout << pages.error() << std::endl;
            return parsed_links;
        }

        for (auto item: pages){
            auto value = item.value();
            auto links = value["links"];
            for (auto link : links){
                auto raw_name = link["title"].get_string().take_value();
                // url decode the thing
                int _output_length; // ignore this
                char* decoded_name = curl_easy_unescape(curl, raw_name.data(), raw_name.size(), &_output_length);
                parsed_links.push_back(decoded_name);
            }
        }
    }
    curl_easy_cleanup(curl);
    return parsed_links;
}

