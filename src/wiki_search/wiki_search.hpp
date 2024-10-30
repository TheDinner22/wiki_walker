#pragma once

#include <curl/curl.h> // url encoding and de-encoding

#include <string_view>
#include <vector>

std::vector<std::basic_string_view<char>>search_wiki(const std::basic_string_view<char>& search_term, size_t max_terms=5);
