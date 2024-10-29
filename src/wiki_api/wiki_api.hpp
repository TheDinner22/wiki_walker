#pragma once

#include <string_view>
#include <vector>

std::vector<std::basic_string_view<char>> get_links(const std::basic_string_view<char>& page_name);
