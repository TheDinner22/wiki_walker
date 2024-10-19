#include <string>
#include <iostream>
#include <vector>

#include <curl/curl.h>
#include <dependencies/httplib.h>
#include <lexbor/html/parser.h>

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
    std::vector<std::string> v;
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

  std::cout << "did it" << std::endl;

  curl_global_cleanup();
 
  return 0;
}
