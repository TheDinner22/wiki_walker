#include "../dependencies/simdjson.h"
#include "./ez_json.hpp"
#include <ostream>
#include <string>

using namespace simdjson;

// there are certain things that we want from the wikipedia API
// 
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

    for(auto item: result["query"].get_object()){
        std::cout << item.key() << std::endl;
    }
    return;

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

    // keep looping as long as the request implies that there
    // is more data
    while(result.find_field("continue").error() == SUCCESS){
        auto plcontinue = result["continue"]["plcontinue"];
    }



}


