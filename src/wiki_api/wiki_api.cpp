#include "./wiki_api.hpp"
#include "../requests/requests.hpp" // get request
#include "../dependencies/simdjson.h" //json

#include <string_view>
#include <vector>

using namespace simdjson;

std::vector<std::basic_string_view<char>>get_links(const std::string& page_name){
    std::vector<std::basic_string_view<char>> parsed_links;

    // send get request
    std::string url = "https://en.wikipedia.org/w/api.php?action=query&format=json&titles=" + page_name + "&prop=links&pllimit=max";
    auto base_url_len = url.size();
    auto raw_json = send_get_request(url.c_str());

    // parse json
    padded_string raw_padded_json(raw_json);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document result = parser.iterate(raw_padded_json);

    auto pages = result["query"]["pages"].get_object();
    if(pages.error() != SUCCESS){ std::cout << pages.error() << std::endl;}

    for (auto item: pages){
        auto value = item.value();
        auto links = value["links"];
        if(links.error() != SUCCESS){continue;}
        for (auto link : links){
            auto s = link["title"].get_string().take_value();
            parsed_links.push_back(s);
        }
    }

    // keep looping as long as the request implies that there
    // is more data
    while(result["continue"].error() == SUCCESS){
        auto plcontinue = result["continue"]["plcontinue"];
        std::cout << plcontinue << std::endl;
        std::string next_url = url;
        next_url += "&plcontinue=";
        next_url += plcontinue.get_string().take_value().data();
        std::cout << next_url << std::endl;

        raw_json = send_get_request(next_url.c_str());
        raw_padded_json = padded_string(raw_json);
        result = parser.iterate(raw_padded_json);
        pages = result["query"]["pages"].get_object();
        if(pages.error() != SUCCESS){ std::cout << pages.error() << std::endl;}

        for (auto item: pages){
            auto value = item.value();
            auto links = value["links"];
            if(links.error() != SUCCESS){continue;}
            for (auto link : links){
                auto s = link["title"].get_string().take_value();
                parsed_links.push_back(s);
            }
        }
    }
    return parsed_links;
}

