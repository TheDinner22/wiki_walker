#pragma once

#include <string>
#include <string_view>
#include <vector>

std::vector<std::basic_string_view<char>> get_links(const std::string& page_name);
