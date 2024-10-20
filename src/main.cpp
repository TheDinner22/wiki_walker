#include <lexbor/dom/interfaces/document.h>
#include <lexbor/dom/interfaces/element.h>
#include <lexbor/dom/interfaces/node.h>
#include <lexbor/html/parser.h>
#include <lexbor/tag/const.h>
#include <string>
#include <iostream>
#include <vector>

#include <curl/curl.h>
#include <dependencies/httplib.h>

#include <lexbor/html/html.h>
#include <lexbor/core/types.h>

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

std::vector<std::string> find_links(const std::string& raw_html){
    std::vector<std::string> v;

    // Initialize the liblexbor HTML parser
    lxb_html_document_t *document;
    lxb_html_parser_t *parser;

    // Create a new HTML document object
    document = lxb_html_document_create();

    // Initialize the HTML parser
    parser = lxb_html_parser_create();

    // Parse the HTML content | this line seg faults
    document = lxb_html_parse(parser, (const unsigned char *)raw_html.data(), raw_html.size());

    std::cout << "------------------------------------------------" << std::endl;

    lxb_dom_node_t* root = &document->body->element.element.node;
    while(root != nullptr){
        if(root->type == LXB_DOM_NODE_TYPE_ELEMENT && lxb_dom_node_tag_id(root) == LXB_TAG_A){
            lxb_dom_element_t *root_but_as_an_element = (lxb_dom_element_t *)root;

            // Get the "href" attribute
            const lxb_char_t *attr_name = (const lxb_char_t *)"href";
            size_t attr_name_len = 4;
            size_t attr_value_len;
            
            const lxb_char_t *href_attr = lxb_dom_element_get_attribute(root_but_as_an_element, attr_name, attr_name_len, &attr_value_len);

            std::string link;
            link.reserve(attr_value_len);
            for(size_t i = 0; i< attr_value_len; i++){
                link.push_back(*(href_attr+i));
            }
            v.push_back(link);
        }

        root = lxb_dom_node_next(root);
    }

    // Clean up
    lxb_html_parser_destroy(parser);
    lxb_html_document_destroy(document);

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
