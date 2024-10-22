#include "../dependencies/simdjson.h"
#include "./ez_json.hpp"
#include <ostream>
#include <string>

using namespace simdjson;

// there are certain things that we want from the wikipedia API
// we get those specific things here
//
// this is not a generic JSON parse and should only be passed the reuslt
// of certain WIKI api endpoints
//
// pages = data["query"]["pages"]
void parse_wiki_api_json(const std::string& raw_json){
    padded_string raw_padded_json(raw_json);

    simdjson::ondemand::parser parser;
    simdjson::ondemand::document result = parser.iterate(raw_padded_json);

    auto pages = result["query"]["pages"].get_object();
    if(pages.error() != SUCCESS){ std::cout << pages.error() << std::endl; return; }

    for (auto item: pages){
        auto value = item.value();
        auto links = value["links"];
        if(links.error() != SUCCESS){continue;}
        for (auto link : links){
            std::cout << link["title"].get_string() << std::endl;
        }
    }



}


