#include <algorithm>
#include <dependencies/httplib.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <vector>
#include <regex>

// I have no idea how use libcurl!!!
// this code is from here: https://stackoverflow.com/questions/44994203/how-to-get-the-http-response-string-using-curl-in-c
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string get_wiki_page(const char* URL){
  CURL* curl = curl_easy_init();
  std::string buffer;
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, URL);
 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 604800L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res) << std::endl;
    }
 
    curl_easy_cleanup(curl);
  }

  return buffer;
}

std::vector<std::string> find_links(const std::string raw_html){
    std::regex rgx("<a.*>.*</a>");
    std::vector<std::string> v;

    // following code straight from cpp reference: https://en.cppreference.com/w/cpp/regex/regex_iterator
    auto words_begin = std::sregex_iterator(raw_html.begin(), raw_html.end(), rgx);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        std::smatch match = *i;
        std::string match_str = match.str();
        v.push_back(match_str);
    }

    return v;
}

int main(void)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);

  std::string s = get_wiki_page("https://en.wikipedia.org/wiki/Lady_Gaga");
  std::vector<std::string> v = find_links(s);

  for (auto link : v){
      std::cout << link << std::endl;
  }

  curl_global_cleanup();
 
  return 0;
}
